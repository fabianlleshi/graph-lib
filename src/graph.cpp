#include "graphlib/graph.hpp"

#include <algorithm>
#include <stdexcept>

namespace graphlib {

void GraphBuilder::add_edge(NodeId u, NodeId v) {
    if (u < 0 || v < 0 || u >= num_nodes_ || v >= num_nodes_) {
        throw std::out_of_range("edge endpoint out of range");
    }
    if (u == v) {
        return;  // skip self-loops
    }
    edges_.push_back({u, v});
}

Graph GraphBuilder::build() {
    std::vector<std::size_t> row_ptr(num_nodes_ + 1, 0);

    for (const auto& [u, v] : edges_) {
        ++row_ptr[u + 1];
        ++row_ptr[v + 1];
    }
    for (NodeId i = 1; i <= num_nodes_; ++i) {
        row_ptr[i] += row_ptr[i - 1];
    }

    std::vector<NodeId> col_idx(row_ptr.back());
    std::vector<std::size_t> cursor = row_ptr;

    for (const auto& [u, v] : edges_) {
        col_idx[cursor[u]++] = v;
        col_idx[cursor[v]++] = u;
    }

    for (NodeId v = 0; v < num_nodes_; ++v) {
        std::sort(col_idx.begin() + row_ptr[v], col_idx.begin() + row_ptr[v + 1]);
    }

    return Graph(std::move(row_ptr), std::move(col_idx));
}

}