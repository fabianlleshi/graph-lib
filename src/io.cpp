#include "graphlib/io.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace graphlib {

Graph load_edge_list(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("could not open file: " + path);
    }

    std::vector<Edge> edges;
    NodeId max_id = -1;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream ss(line);
        NodeId u, v;
        if (!(ss >> u >> v)) {
            continue;
        }
        edges.push_back({u, v});
        max_id = std::max(max_id, std::max(u, v));
    }

    GraphBuilder b(max_id + 1);
    for (const auto& [u, v] : edges) {
        b.add_edge(u, v);
    }
    return b.build();
}

}