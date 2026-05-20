#include "graphlib/components.hpp"

#include <queue>

namespace graphlib {

Components connected_components(const Graph& g) {
    std::vector<int> labels(g.num_nodes(), -1);
    int next = 0;

    for (NodeId start = 0; start < g.num_nodes(); ++start) {
        if (labels[start] != -1) {
            continue;
        }

        labels[start] = next;
        std::queue<NodeId> q;
        q.push(start);

        while (!q.empty()) {
            NodeId v = q.front();
            q.pop();
            for (auto it = g.neighbors_begin(v); it != g.neighbors_end(v); ++it) {
                NodeId u = *it;
                if (labels[u] == -1) {
                    labels[u] = next;
                    q.push(u);
                }
            }
        }

        ++next;
    }

    return {next, std::move(labels)};
}

}