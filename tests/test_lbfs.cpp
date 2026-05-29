#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "graphlib/graph.hpp"
#include "graphlib/lbfs.hpp"

using graphlib::Graph;
using graphlib::GraphBuilder;
using graphlib::lbfs_order;
using graphlib::NodeId;

namespace {

bool is_permutation_of_0_to_n(const std::vector<NodeId>& order, NodeId n) {
    if ((NodeId)order.size() != n) return false;
    std::vector<NodeId> sorted = order;
    std::sort(sorted.begin(), sorted.end());
    for (NodeId i = 0; i < n; ++i) {
        if (sorted[i] != i) return false;
    }
    return true;
}

}

TEST(LBFS, EmptyGraph) {
    GraphBuilder b(0);
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_TRUE(order.empty());
}

TEST(LBFS, SingleNode) {
    GraphBuilder b(1);
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_EQ(order, (std::vector<NodeId>{0}));
}

TEST(LBFS, K4IsAPermutation) {
    GraphBuilder b(4);
    for (NodeId u = 0; u < 4; ++u) {
        for (NodeId v = u + 1; v < 4; ++v) b.add_edge(u, v);
    }
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_TRUE(is_permutation_of_0_to_n(order, 4));
}

TEST(LBFS, PathIsAPermutation) {
    GraphBuilder b(5);
    b.add_edge(0, 1); b.add_edge(1, 2); b.add_edge(2, 3); b.add_edge(3, 4);
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_TRUE(is_permutation_of_0_to_n(order, 5));
}

TEST(LBFS, DisconnectedIsAPermutation) {
    GraphBuilder b(5);
    b.add_edge(0, 1); b.add_edge(1, 2); b.add_edge(0, 2);
    b.add_edge(3, 4);
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_TRUE(is_permutation_of_0_to_n(order, 5));
}

TEST(LBFS, CycleC5IsAPermutation) {
    GraphBuilder b(5);
    b.add_edge(0, 1); b.add_edge(1, 2); b.add_edge(2, 3);
    b.add_edge(3, 4); b.add_edge(4, 0);
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_TRUE(is_permutation_of_0_to_n(order, 5));
}

TEST(LBFS, K32IsAPermutation) {
    GraphBuilder b(5);
    b.add_edge(0, 3); b.add_edge(0, 4);
    b.add_edge(1, 3); b.add_edge(1, 4);
    b.add_edge(2, 3); b.add_edge(2, 4);
    Graph g = b.build();
    auto order = lbfs_order(g);
    EXPECT_TRUE(is_permutation_of_0_to_n(order, 5));
}