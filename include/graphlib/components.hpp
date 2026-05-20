#pragma once

#include <vector>

#include "graphlib/graph.hpp"

namespace graphlib {

struct Components {
    int count;
    std::vector<int> labels;
};

Components connected_components(const Graph& g);

}