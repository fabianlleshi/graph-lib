#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "graphlib/dfs.hpp"
#include "graphlib/graph.hpp"

using graphlib::dfs;
using graphlib::Graph;
using graphlib::GraphBuilder;
using graphlib::NodeId;

namespace {

bool contains(const std::vector<NodeId>& v, NodeId x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

}

TEST(Dfs, SingleNode) {
    GraphBuilder b(1);
    Graph g = b.build();
    EXPECT_EQ(dfs(g, 0), (std::vector<NodeId>{0}));
}

TEST(Dfs, VisitsAllReachable) {
    GraphBuilder b(5);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    b.add_edge(3, 4);
    Graph g = b.build();

    auto order = dfs(g, 0);
    EXPECT_EQ(order.size(), 5u);
    for (NodeId v = 0; v < 5; ++v) {
        EXPECT_TRUE(contains(order, v));
    }
}

TEST(Dfs, StartsAtSource) {
    GraphBuilder b(4);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    Graph g = b.build();

    auto order = dfs(g, 2);
    EXPECT_EQ(order.front(), 2);
}

TEST(Dfs, StopsAtDisconnected) {
    GraphBuilder b(5);
    b.add_edge(0, 1);
    b.add_edge(3, 4);
    Graph g = b.build();

    auto order = dfs(g, 0);
    EXPECT_EQ(order.size(), 2u);
    EXPECT_TRUE(contains(order, 0));
    EXPECT_TRUE(contains(order, 1));
    EXPECT_FALSE(contains(order, 3));
}

TEST(Dfs, NoDuplicates) {
    GraphBuilder b(4);
    for (NodeId u = 0; u < 4; ++u) {
        for (NodeId v = u + 1; v < 4; ++v) {
            b.add_edge(u, v);
        }
    }
    Graph g = b.build();

    auto order = dfs(g, 0);
    std::vector<NodeId> sorted = order;
    std::sort(sorted.begin(), sorted.end());
    EXPECT_EQ(sorted, (std::vector<NodeId>{0, 1, 2, 3}));
    EXPECT_EQ(order.size(), 4u);
}