#include <gtest/gtest.h>

#include "graphlib/graph.hpp"
#include "graphlib/lbfs.hpp"
#include "graphlib/modular_decomposition.hpp"

using graphlib::assemble_md;
using graphlib::Graph;
using graphlib::GraphBuilder;
using graphlib::lbfs_order;
using graphlib::MDKind;
using graphlib::NodeId;
using graphlib::verify_modules;

TEST(MDAssembly, SingleNode) {
    GraphBuilder b(1);
    Graph g = b.build();
    auto order = lbfs_order(g);
    auto root = assemble_md(g, order);
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->kind, MDKind::Leaf);
}

TEST(MDAssembly, TwoConnectedVertices) {
    GraphBuilder b(2);
    b.add_edge(0, 1);
    Graph g = b.build();
    auto order = lbfs_order(g);
    auto root = assemble_md(g, order);
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->kind, MDKind::Series);
    EXPECT_TRUE(verify_modules(g, *root));
}

TEST(MDAssembly, TwoDisconnectedVertices) {
    GraphBuilder b(2);
    Graph g = b.build();
    auto order = lbfs_order(g);
    auto root = assemble_md(g, order);
    ASSERT_NE(root, nullptr);
    EXPECT_TRUE(verify_modules(g, *root));
}

TEST(MDAssembly, K4IsSeries) {
    GraphBuilder b(4);
    for (NodeId u = 0; u < 4; ++u) {
        for (NodeId v = u + 1; v < 4; ++v) b.add_edge(u, v);
    }
    Graph g = b.build();
    auto order = lbfs_order(g);
    auto root = assemble_md(g, order);
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->kind, MDKind::Series);
    EXPECT_TRUE(verify_modules(g, *root));
}