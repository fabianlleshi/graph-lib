#include "graphlib/dijkstra.hpp"

#include <queue>
#include <utility>

namespace graphlib {

std::vector<Weight> dijkstra(const Graph& g, NodeId source) {
    std::vector<Weight> dist(g.num_nodes(), kInfinity);

    using Item = std::pair<Weight, NodeId>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d > dist[v]) {
            continue;
        }

        std::size_t off = g.neighbor_offset(v);
        for (auto it = g.neighbors_begin(v); it != g.neighbors_end(v); ++it, ++off) {
            NodeId u = *it;
            Weight nd = dist[v] + g.weight_at(off);
            if (nd < dist[u]) {
                dist[u] = nd;
                pq.push({nd, u});
            }
        }
    }

    return dist;
}

}