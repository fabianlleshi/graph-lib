#pragma once

#include <string>

#include "graphlib/graph.hpp"

namespace graphlib {

Graph load_edge_list(const std::string& path);

}