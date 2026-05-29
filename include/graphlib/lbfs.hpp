#pragma once

#include <vector>

#include "graphlib/graph.hpp"

namespace graphlib {

std::vector<NodeId> lbfs_order(const Graph& g);

}