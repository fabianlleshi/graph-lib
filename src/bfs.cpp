#include "graphlib/bfs.hpp"

#include <queue>

namespace graphlib {

std::vector<int> bfs(const Graph& g, NodeId source) {
    std::vector<int> dist(g.num_nodes(), -1);
    std::queue<NodeId> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {
        NodeId v = q.front();
        q.pop();

        for (auto it = g.neighbors_begin(v); it != g.neighbors_end(v); ++it) {
            NodeId u = *it;
            if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                q.push(u);
            }
        }
    }

    return dist;
}

}