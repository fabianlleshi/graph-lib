#pragma once

#include <memory>
#include <vector>

#include "graphlib/graph.hpp"

namespace graphlib {

enum class MDKind {
    Leaf,
    Series,
    Parallel,
    Prime,
};

struct MDNode {
    MDKind kind;
    NodeId leaf_id = -1;
    std::vector<std::unique_ptr<MDNode>> children;

    static std::unique_ptr<MDNode> leaf(NodeId v) {
        auto n = std::make_unique<MDNode>();
        n->kind = MDKind::Leaf;
        n->leaf_id = v;
        return n;
    }

    static std::unique_ptr<MDNode> internal(MDKind k) {
        auto n = std::make_unique<MDNode>();
        n->kind = k;
        return n;
    }
};

std::vector<NodeId> collect_leaves(const MDNode& root);

bool verify_modules(const Graph& g, const MDNode& root);

}