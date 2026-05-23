#include <chrono>
#include <iostream>
#include <string>

#include "graphlib/components.hpp"
#include "graphlib/graph.hpp"
#include "graphlib/io.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <edge-list-file>\n";
        return 1;
    }

    using clock = std::chrono::steady_clock;

    auto t0 = clock::now();
    graphlib::Graph g = graphlib::load_edge_list(argv[1]);
    auto t1 = clock::now();

    auto c = graphlib::connected_components(g);
    auto t2 = clock::now();

    auto ms = [](auto a, auto b) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
    };

    int largest = 0;
    {
        std::vector<int> sizes(c.count, 0);
        for (int label : c.labels) {
            ++sizes[label];
        }
        for (int s : sizes) {
            largest = std::max(largest, s);
        }
    }

    std::cout << "nodes:              " << g.num_nodes() << "\n";
    std::cout << "edges:              " << g.num_edges() << "\n";
    std::cout << "components:         " << c.count << "\n";
    std::cout << "largest component:  " << largest << "\n";
    std::cout << "load time:          " << ms(t0, t1) << " ms\n";
    std::cout << "components time:    " << ms(t1, t2) << " ms\n";

    return 0;
}