#include "graphlib/graph.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace graphlib {

void GraphBuilder::add_edge(NodeId u, NodeId v) {
    add_edge(u, v, Weight{1});
}

void GraphBuilder::add_edge(NodeId u, NodeId v, Weight w) {
    if (u < 0 || v < 0 || u >= num_nodes_ || v >= num_nodes_) {
        throw std::out_of_range("edge endpoint out of range");
    }
    if (u == v) {
        return;
    }
    if (w != Weight{1}) {
        any_weighted_ = true;
    }
    edges_.push_back({u, v, w});
}

Graph GraphBuilder::build() {
    std::vector<std::size_t> row_ptr(num_nodes_ + 1, 0);

    for (const auto& e : edges_) {
        ++row_ptr[e.u + 1];
        ++row_ptr[e.v + 1];
    }
    for (NodeId i = 1; i <= num_nodes_; ++i) {
        row_ptr[i] += row_ptr[i - 1];
    }

    std::vector<NodeId> col_idx(row_ptr.back());
    std::vector<Weight> weights;
    if (any_weighted_) {
        weights.resize(row_ptr.back());
    }
    std::vector<std::size_t> cursor = row_ptr;

    for (const auto& e : edges_) {
        std::size_t pu = cursor[e.u]++;
        std::size_t pv = cursor[e.v]++;
        col_idx[pu] = e.v;
        col_idx[pv] = e.u;
        if (any_weighted_) {
            weights[pu] = e.w;
            weights[pv] = e.w;
        }
    }

    // Sort each neighbor list, carrying weights along if present.
    for (NodeId v = 0; v < num_nodes_; ++v) {
        std::size_t begin = row_ptr[v];
        std::size_t end = row_ptr[v + 1];

        if (!any_weighted_) {
            std::sort(col_idx.begin() + begin, col_idx.begin() + end);
        } else {
            std::vector<std::size_t> perm(end - begin);
            std::iota(perm.begin(), perm.end(), begin);
            std::sort(perm.begin(), perm.end(), [&](std::size_t a, std::size_t b) {
                return col_idx[a] < col_idx[b];
            });

            std::vector<NodeId> sorted_cols(end - begin);
            std::vector<Weight> sorted_w(end - begin);
            for (std::size_t k = 0; k < perm.size(); ++k) {
                sorted_cols[k] = col_idx[perm[k]];
                sorted_w[k] = weights[perm[k]];
            }
            std::copy(sorted_cols.begin(), sorted_cols.end(), col_idx.begin() + begin);
            std::copy(sorted_w.begin(), sorted_w.end(), weights.begin() + begin);
        }
    }

    return Graph(std::move(row_ptr), std::move(col_idx), std::move(weights));
}

}