#include "graphlib/lbfs.hpp"

#include <list>
#include <vector>
#include <algorithm>

namespace graphlib {

namespace {

struct LBBlock {
    NodeId head = -1;
    NodeId tail = -1;
    LBBlock* new_block = nullptr;
    bool touched = false;
};

void push_back(LBBlock& b, NodeId v,
               std::vector<NodeId>& next,
               std::vector<NodeId>& prev,
               std::vector<LBBlock*>& owner) {
    if (b.head == -1) {
        b.head = b.tail = v;
        prev[v] = -1;
        next[v] = -1;
    } else {
        prev[v] = b.tail;
        next[b.tail] = v;
        next[v] = -1;
        b.tail = v;
    }
    owner[v] = &b;
}

void detach(LBBlock& b, NodeId v,
            std::vector<NodeId>& next,
            std::vector<NodeId>& prev,
            std::vector<LBBlock*>& owner) {
    NodeId pv = prev[v];
    NodeId nv = next[v];
    if (pv != -1) next[pv] = nv; else b.head = nv;
    if (nv != -1) prev[nv] = pv; else b.tail = pv;
    prev[v] = next[v] = -1;
    owner[v] = nullptr;
}

}

std::vector<NodeId> lbfs_order(const Graph& g) {
    const NodeId n = g.num_nodes();
    std::vector<NodeId> order(n);
    if (n == 0) return order;

    std::vector<NodeId> next(n, -1);
    std::vector<NodeId> prev(n, -1);
    std::vector<LBBlock*> owner(n, nullptr);
    std::vector<char> mark(n, 0);

    std::list<LBBlock> blocks;
    blocks.emplace_back();
    LBBlock& initial = blocks.back();
    for (NodeId v = 0; v < n; ++v) {
        push_back(initial, v, next, prev, owner);
    }

    std::vector<LBBlock*> touched;
    touched.reserve(n);

    for (NodeId idx = n - 1; idx >= 0; --idx) {
        LBBlock* B = &blocks.front();
        NodeId s = B->head;
        order[idx] = s;

        for (auto it = g.neighbors_begin(s); it != g.neighbors_end(s); ++it) {
            NodeId u = *it;
            mark[u] = 1;
            LBBlock* blk = owner[u];
            if (blk && !blk->touched) {
                blk->touched = true;
                touched.push_back(blk);
            }
        }

        for (auto it = g.neighbors_begin(s); it != g.neighbors_end(s); ++it) {
            NodeId u = *it;
            if (!mark[u] || !owner[u]) continue;

            LBBlock* blk = owner[u];
            LBBlock* bp = blk->new_block;

            if (!bp) {
                auto blk_iter = std::find_if(blocks.begin(), blocks.end(),
                    [blk](const LBBlock& x) { return &x == blk; });
                auto new_iter = blocks.emplace(blk_iter);
                bp = &(*new_iter);
                blk->new_block = bp;
                if (!blk->touched) {
                    blk->touched = true;
                    touched.push_back(blk);
                }
            }
            detach(*blk, u, next, prev, owner);
            push_back(*bp, u, next, prev, owner);
        }

        for (LBBlock* blk : touched) {
            LBBlock* bp = blk->new_block;
            blk->touched = false;
            blk->new_block = nullptr;

            if (bp && bp->head == -1) {
                blocks.remove_if([bp](const LBBlock& x) { return &x == bp; });
            }
            if (blk->head == -1) {
                blocks.remove_if([blk](const LBBlock& x) { return &x == blk; });
            }
        }

        for (auto it = g.neighbors_begin(s); it != g.neighbors_end(s); ++it) {
            mark[*it] = 0;
        }
        touched.clear();

        if (owner[s]) {
            LBBlock* blk = owner[s];
            detach(*blk, s, next, prev, owner);
            if (blk->head == -1) {
                blocks.remove_if([blk](const LBBlock& x) { return &x == blk; });
            }
        }

        if (idx == 0) break;
    }

    return order;
}

}