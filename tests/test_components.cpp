#include <gtest/gtest.h>

#include "graphlib/components.hpp"
#include "graphlib/graph.hpp"

using graphlib::connected_components;
using graphlib::Graph;
using graphlib::GraphBuilder;

TEST(Components, AllIsolated) {
    GraphBuilder b(4);
    Graph g = b.build();
    auto c = connected_components(g);
    EXPECT_EQ(c.count, 4);
}

TEST(Components, OneBigComponent) {
    GraphBuilder b(4);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    Graph g = b.build();
    auto c = connected_components(g);
    EXPECT_EQ(c.count, 1);
}

TEST(Components, TwoComponents) {
    GraphBuilder b(6);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(3, 4);
    Graph g = b.build();
    auto c = connected_components(g);

    EXPECT_EQ(c.count, 3);
    EXPECT_EQ(c.labels[0], c.labels[1]);
    EXPECT_EQ(c.labels[1], c.labels[2]);
    EXPECT_EQ(c.labels[3], c.labels[4]);
    EXPECT_NE(c.labels[0], c.labels[3]);
    EXPECT_NE(c.labels[0], c.labels[5]);
}