#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace graphlib {

using NodeId = std::int32_t;
using Edge = std::pair<NodeId, NodeId>;

class Graph {
public:
    NodeId num_nodes() const { return static_cast<NodeId>(row_ptr_.size()) - 1; }
    std::size_t num_edges() const { return col_idx_.size() / 2; }

    std::size_t degree(NodeId v) const {
        return row_ptr_[v + 1] - row_ptr_[v];
    }

    const NodeId* neighbors_begin(NodeId v) const {
        return col_idx_.data() + row_ptr_[v];
    }
    const NodeId* neighbors_end(NodeId v) const {
        return col_idx_.data() + row_ptr_[v + 1];
    }

private:
    friend class GraphBuilder;
    Graph(std::vector<std::size_t> row_ptr, std::vector<NodeId> col_idx)
        : row_ptr_(std::move(row_ptr)), col_idx_(std::move(col_idx)) {}

    std::vector<std::size_t> row_ptr_;
    std::vector<NodeId> col_idx_;
};

class GraphBuilder {
public:
    explicit GraphBuilder(NodeId num_nodes) : num_nodes_(num_nodes) {}

    void add_edge(NodeId u, NodeId v);

    Graph build();

private:
    NodeId num_nodes_;
    std::vector<Edge> edges_;
};

}