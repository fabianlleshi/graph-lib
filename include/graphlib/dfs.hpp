#pragma once

#include <vector>

#include "graphlib/graph.hpp"

namespace graphlib {

std::vector<NodeId> dfs(const Graph& g, NodeId source);

}