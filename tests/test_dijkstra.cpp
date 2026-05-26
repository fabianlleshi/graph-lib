#include <gtest/gtest.h>

#include <vector>

#include "graphlib/dijkstra.hpp"
#include "graphlib/graph.hpp"

using graphlib::dijkstra;
using graphlib::Graph;
using graphlib::GraphBuilder;
using graphlib::kInfinity;
using graphlib::Weight;

TEST(Dijkstra, SingleNode) {
    GraphBuilder b(1);
    Graph g = b.build();
    EXPECT_EQ(dijkstra(g, 0), (std::vector<Weight>{0}));
}

TEST(Dijkstra, UnweightedActsLikeHopCount) {
    GraphBuilder b(4);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    Graph g = b.build();
    EXPECT_EQ(dijkstra(g, 0), (std::vector<Weight>{0, 1, 2, 3}));
}

TEST(Dijkstra, PrefersCheaperPath) {
    GraphBuilder b(4);
    b.add_edge(0, 1, 1.0);
    b.add_edge(1, 3, 1.0);
    b.add_edge(0, 2, 5.0);
    b.add_edge(2, 3, 5.0);
    Graph g = b.build();

    auto d = dijkstra(g, 0);
    EXPECT_DOUBLE_EQ(d[3], 2.0);
    EXPECT_DOUBLE_EQ(d[2], 5.0);
}

TEST(Dijkstra, ShortcutBeatsDirectEdge) {
    GraphBuilder b(3);
    b.add_edge(0, 2, 10.0);
    b.add_edge(0, 1, 1.0);
    b.add_edge(1, 2, 1.0);
    Graph g = b.build();

    auto d = dijkstra(g, 0);
    EXPECT_DOUBLE_EQ(d[2], 2.0);
}

TEST(Dijkstra, UnreachableIsInfinity) {
    GraphBuilder b(3);
    b.add_edge(0, 1, 1.0);
    Graph g = b.build();

    auto d = dijkstra(g, 0);
    EXPECT_EQ(d[2], kInfinity);
}