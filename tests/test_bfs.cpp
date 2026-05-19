#include <gtest/gtest.h>

#include <vector>

#include "graphlib/bfs.hpp"
#include "graphlib/graph.hpp"

using graphlib::bfs;
using graphlib::Graph;
using graphlib::GraphBuilder;

TEST(Bfs, SingleNode) {
    GraphBuilder b(1);
    Graph g = b.build();
    EXPECT_EQ(bfs(g, 0), (std::vector<int>{0}));
}

TEST(Bfs, Path) {
    GraphBuilder b(5);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    b.add_edge(3, 4);
    Graph g = b.build();

    EXPECT_EQ(bfs(g, 0), (std::vector<int>{0, 1, 2, 3, 4}));
    EXPECT_EQ(bfs(g, 2), (std::vector<int>{2, 1, 0, 1, 2}));
}

TEST(Bfs, Disconnected) {
    GraphBuilder b(5);
    b.add_edge(0, 1);
    b.add_edge(3, 4);
    Graph g = b.build();

    EXPECT_EQ(bfs(g, 0), (std::vector<int>{0, 1, -1, -1, -1}));
}

TEST(Bfs, CycleTakesShortestPath) {
    GraphBuilder b(4);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    b.add_edge(3, 0);
    Graph g = b.build();

    EXPECT_EQ(bfs(g, 0), (std::vector<int>{0, 1, 2, 1}));
}