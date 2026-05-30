#include "graphlib/modular_decomposition.hpp"

#include <vector>

namespace graphlib {

namespace {

void collect_leaves_into(const MDNode& node, std::vector<NodeId>& out) {
    if (node.kind == MDKind::Leaf) {
        out.push_back(node.leaf_id);
        return;
    }
    for (const auto& c : node.children) {
        collect_leaves_into(*c, out);
    }
}

bool is_module(const Graph& g, const std::vector<NodeId>& members) {
    std::vector<char> in_module(g.num_nodes(), 0);
    for (NodeId v : members) {
        in_module[v] = 1;
    }

    for (NodeId x = 0; x < g.num_nodes(); ++x) {
        if (in_module[x]) continue;

        std::size_t hits = 0;
        for (auto it = g.neighbors_begin(x); it != g.neighbors_end(x); ++it) {
            if (in_module[*it]) ++hits;
        }
        if (hits != 0 && hits != members.size()) {
            return false;
        }
    }
    return true;
}

bool verify_recursive(const Graph& g, const MDNode& node) {
    if (node.kind == MDKind::Leaf) return true;

    std::vector<NodeId> leaves;
    collect_leaves_into(node, leaves);
    if (!is_module(g, leaves)) return false;

    for (const auto& c : node.children) {
        if (!verify_recursive(g, *c)) return false;
    }
    return true;
}

}

std::vector<NodeId> collect_leaves(const MDNode& root) {
    std::vector<NodeId> out;
    collect_leaves_into(root, out);
    return out;
}

bool verify_modules(const Graph& g, const MDNode& root) {
    return verify_recursive(g, root);
}

namespace {

struct BlockState {
    std::unique_ptr<MDNode> node;
    std::vector<NodeId> leaves;
    NodeId rep;
    long req_out;
};

bool has_neighbor(const Graph& g, NodeId v, NodeId target) {
    for (auto it = g.neighbors_begin(v); it != g.neighbors_end(v); ++it) {
        if (*it == target) return true;
    }
    return false;
}

}

std::unique_ptr<MDNode> assemble_md(const Graph& g, const std::vector<NodeId>& perm) {
    const NodeId n = g.num_nodes();
    if (n == 0) return nullptr;

    std::vector<unsigned char> color_val(n, 0);
    std::vector<unsigned int> color_epoch(n, 0);
    std::vector<unsigned char> mark_val(n, 0);
    std::vector<unsigned int> mark_epoch(n, 0);
    unsigned int epoch_color = 1;
    unsigned int epoch_mark = 1;

    std::vector<BlockState> stack;
    stack.reserve(n);

    auto start_new_block = [&](NodeId v) {
        BlockState s;
        s.node = MDNode::leaf(v);
        s.leaves = {v};
        s.rep = v;
        s.req_out = g.degree(v);

        for (auto it = g.neighbors_begin(v); it != g.neighbors_end(v); ++it) {
            mark_val[*it] = 1;
            mark_epoch[*it] = epoch_mark;
        }
        color_val[v] = 1;
        color_epoch[v] = epoch_color;
        stack.push_back(std::move(s));
    };

    start_new_block(perm[0]);

    for (NodeId i = 1; i < (NodeId)perm.size(); ++i) {
        NodeId v = perm[i];
        BlockState B;
        B.node = MDNode::leaf(v);
        B.leaves = {v};
        B.rep = v;
        B.req_out = g.degree(v);

        color_val[v] = 2;
        color_epoch[v] = epoch_color;

        bool merged = true;
        while (merged && !stack.empty()) {
            merged = false;
            BlockState& S = stack.back();

            long edges_sb = 0;
            for (NodeId w : B.leaves) {
                for (auto it = g.neighbors_begin(w); it != g.neighbors_end(w); ++it) {
                    NodeId x = *it;
                    if (color_epoch[x] == epoch_color && color_val[x] == 1) {
                        ++edges_sb;
                    }
                }
            }
            long full = (long)S.leaves.size() * (long)B.leaves.size();
            int cross;
            if (edges_sb == 0) cross = 0;
            else if (edges_sb == full) cross = 1;
            else break;

            long r_in_b = 0;
            for (NodeId w : B.leaves) {
                if (has_neighbor(g, w, S.rep)) ++r_in_b;
            }
            long expected = S.req_out - r_in_b;
            if (expected < 0) break;

            bool ok = true;
            for (NodeId w : B.leaves) {
                if (!ok) break;
                long dout = 0;
                for (auto it = g.neighbors_begin(w); it != g.neighbors_end(w); ++it) {
                    NodeId x = *it;
                    int c = (color_epoch[x] == epoch_color) ? color_val[x] : 0;
                    if (c == 0) {
                        if (!(mark_epoch[x] == epoch_mark && mark_val[x] == 1)) {
                            ok = false;
                            break;
                        }
                        ++dout;
                    }
                }
                if (ok && dout != expected) ok = false;
            }
            if (!ok) break;

            MDKind kind = (cross == 1) ? MDKind::Series : MDKind::Parallel;
            std::unique_ptr<MDNode> L = std::move(S.node);
            std::unique_ptr<MDNode> R = std::move(B.node);

            if (L->kind == kind && L->leaf_id == -1) {
                L->children.push_back(std::move(R));
                S.node = std::move(L);
            } else {
                auto parent = MDNode::internal(kind);
                parent->children.push_back(std::move(L));
                parent->children.push_back(std::move(R));
                S.node = std::move(parent);
            }

            for (NodeId w : B.leaves) {
                color_val[w] = 1;
                color_epoch[w] = epoch_color;
            }
            S.leaves.insert(S.leaves.end(), B.leaves.begin(), B.leaves.end());
            S.req_out -= r_in_b;

            B.node = nullptr;
            merged = true;
        }

        if (B.node) {
            ++epoch_color;
            ++epoch_mark;
            start_new_block(v);
        }
    }

    if (stack.size() == 1) {
        return std::move(stack[0].node);
    }
    auto root = MDNode::internal(MDKind::Prime);
    for (auto& s : stack) {
        root->children.push_back(std::move(s.node));
    }
    return root;
}

}