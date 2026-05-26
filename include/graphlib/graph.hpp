#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace graphlib {

using NodeId = std::int32_t;
using Weight = double;
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

    bool weighted() const { return !weights_.empty(); }

    // Weight of the edge stored at neighbor slot i, where i is an index into
    // the range [row_ptr_[v], row_ptr_[v+1]). Unweighted graphs report 1.
    Weight weight_at(std::size_t i) const {
        return weights_.empty() ? Weight{1} : weights_[i];
    }

    std::size_t neighbor_offset(NodeId v) const { return row_ptr_[v]; }

private:
    friend class GraphBuilder;
    Graph(std::vector<std::size_t> row_ptr,
          std::vector<NodeId> col_idx,
          std::vector<Weight> weights)
        : row_ptr_(std::move(row_ptr)),
          col_idx_(std::move(col_idx)),
          weights_(std::move(weights)) {}

    std::vector<std::size_t> row_ptr_;
    std::vector<NodeId> col_idx_;
    std::vector<Weight> weights_;
};

class GraphBuilder {
public:
    explicit GraphBuilder(NodeId num_nodes) : num_nodes_(num_nodes) {}

    void add_edge(NodeId u, NodeId v);
    void add_edge(NodeId u, NodeId v, Weight w);

    Graph build();

private:
    struct BuilderEdge {
        NodeId u, v;
        Weight w;
    };
    NodeId num_nodes_;
    std::vector<BuilderEdge> edges_;
    bool any_weighted_ = false;
};

}