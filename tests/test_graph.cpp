#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "graphlib/graph.hpp"

using graphlib::Graph;
using graphlib::GraphBuilder;
using graphlib::NodeId;

namespace {

std::vector<NodeId> neighbors_of(const Graph& g, NodeId v) {
    return std::vector<NodeId>(g.neighbors_begin(v), g.neighbors_end(v));
}

}

TEST(Graph, EmptyGraph) {
    GraphBuilder b(5);
    Graph g = b.build();

    EXPECT_EQ(g.num_nodes(), 5);
    EXPECT_EQ(g.num_edges(), 0u);
    for (NodeId v = 0; v < 5; ++v) {
        EXPECT_EQ(g.degree(v), 0u);
    }
}

TEST(Graph, SingleEdge) {
    GraphBuilder b(3);
    b.add_edge(0, 2);
    Graph g = b.build();

    EXPECT_EQ(g.num_edges(), 1u);
    EXPECT_EQ(g.degree(0), 1u);
    EXPECT_EQ(g.degree(1), 0u);
    EXPECT_EQ(g.degree(2), 1u);
    EXPECT_EQ(neighbors_of(g, 0), (std::vector<NodeId>{2}));
    EXPECT_EQ(neighbors_of(g, 2), (std::vector<NodeId>{0}));
}

TEST(Graph, K4Clique) {
    GraphBuilder b(4);
    for (NodeId u = 0; u < 4; ++u) {
        for (NodeId v = u + 1; v < 4; ++v) {
            b.add_edge(u, v);
        }
    }
    Graph g = b.build();

    EXPECT_EQ(g.num_edges(), 6u);
    for (NodeId v = 0; v < 4; ++v) {
        EXPECT_EQ(g.degree(v), 3u);
    }
    EXPECT_EQ(neighbors_of(g, 0), (std::vector<NodeId>{1, 2, 3}));
    EXPECT_EQ(neighbors_of(g, 2), (std::vector<NodeId>{0, 1, 3}));
}

TEST(Graph, SelfLoopIgnored) {
    GraphBuilder b(2);
    b.add_edge(0, 0);
    b.add_edge(0, 1);
    Graph g = b.build();

    EXPECT_EQ(g.num_edges(), 1u);
    EXPECT_EQ(g.degree(0), 1u);
}

TEST(Graph, OutOfRangeThrows) {
    GraphBuilder b(3);
    EXPECT_THROW(b.add_edge(0, 5), std::out_of_range);
    EXPECT_THROW(b.add_edge(-1, 0), std::out_of_range);
}

TEST(Graph, NeighborsAreSorted) {
    GraphBuilder b(5);
    b.add_edge(0, 3);
    b.add_edge(0, 1);
    b.add_edge(0, 4);
    b.add_edge(0, 2);
    Graph g = b.build();

    EXPECT_EQ(neighbors_of(g, 0), (std::vector<NodeId>{1, 2, 3, 4}));
}