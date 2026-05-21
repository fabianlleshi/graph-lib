#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "graphlib/components.hpp"
#include "graphlib/io.hpp"

using graphlib::connected_components;
using graphlib::load_edge_list;

namespace {

std::string write_temp(const std::string& contents) {
    std::string path = std::tmpnam(nullptr);
    std::ofstream out(path);
    out << contents;
    out.close();
    return path;
}

}

TEST(Io, LoadsSimpleGraph) {
    std::string path = write_temp(
        "# a comment\n"
        "0 1\n"
        "1 2\n"
        "2 3\n");

    auto g = load_edge_list(path);
    EXPECT_EQ(g.num_nodes(), 4);
    EXPECT_EQ(g.num_edges(), 3u);

    std::remove(path.c_str());
}

TEST(Io, SkipsCommentsAndBlanks) {
    std::string path = write_temp(
        "# Nodes: 3\n"
        "\n"
        "0 1\n"
        "\n"
        "1 2\n");

    auto g = load_edge_list(path);
    EXPECT_EQ(g.num_edges(), 2u);

    std::remove(path.c_str());
}

TEST(Io, ComponentsOnLoadedGraph) {
    std::string path = write_temp(
        "0 1\n"
        "2 3\n"
        "3 4\n");

    auto g = load_edge_list(path);
    auto c = connected_components(g);
    EXPECT_EQ(c.count, 2);

    std::remove(path.c_str());
}

TEST(Io, MissingFileThrows) {
    EXPECT_THROW(load_edge_list("this_file_does_not_exist_12345"), std::runtime_error);
}