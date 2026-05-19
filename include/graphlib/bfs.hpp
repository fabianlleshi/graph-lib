#pragma once

#include <vector>

#include "graphlib/graph.hpp"

namespace graphlib {

std::vector<int> bfs(const Graph& g, NodeId source);

}