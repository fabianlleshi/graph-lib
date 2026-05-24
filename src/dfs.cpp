#include "graphlib/dfs.hpp"

#include <stack>

namespace graphlib {

std::vector<NodeId> dfs(const Graph& g, NodeId source) {
    std::vector<NodeId> order;
    std::vector<bool> seen(g.num_nodes(), false);
    std::stack<NodeId> st;

    st.push(source);

    while (!st.empty()) {
        NodeId v = st.top();
        st.pop();

        if (seen[v]) {
            continue;
        }
        seen[v] = true;
        order.push_back(v);

        for (auto it = g.neighbors_begin(v); it != g.neighbors_end(v); ++it) {
            if (!seen[*it]) {
                st.push(*it);
            }
        }
    }

    return order;
}

}