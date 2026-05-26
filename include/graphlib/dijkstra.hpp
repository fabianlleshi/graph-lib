#pragma once

#include <limits>
#include <vector>

#include "graphlib/graph.hpp"

namespace graphlib {

constexpr Weight kInfinity = std::numeric_limits<Weight>::infinity();

std::vector<Weight> dijkstra(const Graph& g, NodeId source);

}