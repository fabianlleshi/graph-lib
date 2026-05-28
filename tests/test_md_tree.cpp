#include <gtest/gtest.h>

#include <algorithm>

#include "graphlib/graph.hpp"
#include "graphlib/modular_decomposition.hpp"

using graphlib::collect_leaves;
using graphlib::Graph;
using graphlib::GraphBuilder;
using graphlib::MDKind;
using graphlib::MDNode;
using graphlib::NodeId;
using graphlib::verify_modules;

namespace {

Graph k4() {
    GraphBuilder b(4);
    for (NodeId u = 0; u < 4; ++u) {
        for (NodeId v = u + 1; v < 4; ++v) b.add_edge(u, v);
    }
    return b.build();
}

Graph k3_plus_k2() {
    GraphBuilder b(5);
    b.add_edge(0, 1); b.add_edge(1, 2); b.add_edge(0, 2);
    b.add_edge(3, 4);
    return b.build();
}

}

TEST(MDTree, CollectLeavesFromLeaf) {
    auto root = MDNode::leaf(7);
    EXPECT_EQ(collect_leaves(*root), (std::vector<NodeId>{7}));
}

TEST(MDTree, CollectLeavesFromInternal) {
    auto root = MDNode::internal(MDKind::Series);
    root->children.push_back(MDNode::leaf(2));
    root->children.push_back(MDNode::leaf(0));
    root->children.push_back(MDNode::leaf(1));

    auto leaves = collect_leaves(*root);
    std::sort(leaves.begin(), leaves.end());
    EXPECT_EQ(leaves, (std::vector<NodeId>{0, 1, 2}));
}

TEST(MDTree, K4VerifiesAsSingleSeries) {
    Graph g = k4();
    auto root = MDNode::internal(MDKind::Series);
    for (NodeId v = 0; v < 4; ++v) {
        root->children.push_back(MDNode::leaf(v));
    }
    EXPECT_TRUE(verify_modules(g, *root));
}

TEST(MDTree, K3PlusK2VerifiesWithTwoChildren) {
    Graph g = k3_plus_k2();
    auto root = MDNode::internal(MDKind::Parallel);

    auto left = MDNode::internal(MDKind::Series);
    left->children.push_back(MDNode::leaf(0));
    left->children.push_back(MDNode::leaf(1));
    left->children.push_back(MDNode::leaf(2));

    auto right = MDNode::internal(MDKind::Series);
    right->children.push_back(MDNode::leaf(3));
    right->children.push_back(MDNode::leaf(4));

    root->children.push_back(std::move(left));
    root->children.push_back(std::move(right));

    EXPECT_TRUE(verify_modules(g, *root));
}

TEST(MDTree, NonModuleFailsVerification) {
    Graph g = k3_plus_k2();
    auto root = MDNode::internal(MDKind::Parallel);

    // {0, 3} is not a module of K3+K2: vertex 1 connects to 0 but not 3.
    auto bad = MDNode::internal(MDKind::Series);
    bad->children.push_back(MDNode::leaf(0));
    bad->children.push_back(MDNode::leaf(3));

    auto rest = MDNode::internal(MDKind::Series);
    rest->children.push_back(MDNode::leaf(1));
    rest->children.push_back(MDNode::leaf(2));
    rest->children.push_back(MDNode::leaf(4));

    root->children.push_back(std::move(bad));
    root->children.push_back(std::move(rest));

    EXPECT_FALSE(verify_modules(g, *root));
}

TEST(MDTree, SingleLeafTrivialllyVerifies) {
    Graph g = k4();
    auto root = MDNode::leaf(0);
    EXPECT_TRUE(verify_modules(g, *root));
}