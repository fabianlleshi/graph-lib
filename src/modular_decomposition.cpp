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

}