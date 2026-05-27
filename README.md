# graphlib

CSR-backed graph library in C++17. Loads edge lists, runs the standard
traversals and shortest paths. The real reason it exists is the linear-time
modular decomposition that's coming; I'm porting it from the C version that I wrote at
[modular-decomposition](https://github.com/fabianlleshi/modular-decomposition).

Graphs are immutable once built. You fill a `GraphBuilder`, call `build()`, and
get back a read-only `Graph`. The structure lives in two flat arrays (CSR), so
walking a node's neighbors is just a pointer scan.

Weights, when present, sit in a parallel array indexed the same way. That is a deliberate choice since a struct of arrays is preffered over an array of structs, so the unweighted
algorithms never load weight data that they do not need.

## Build

```
cmake -B build
cmake --build build
ctest --test-dir build
```

CMake 3.16+, a C++17 compiler. GoogleTest is fetched at configure time.

## CLI

```
./build/graphlib_cli data/facebook_combined.txt
```

On the SNAP ego-Facebook graph (4039 nodes, 88234 edges):

```
nodes:              4039
edges:              88234
components:         1
largest component:  4039
load time:          167 ms
components time:    2 ms
```

Loading dominates since it's all string to int parsing. The components pass over
88k edges is simply noise when compared.

## What's here

BFS, DFS (iterative), connected
components, Dijkstra. Each has its own file under `src/` and a matching test.

## Not here yet

Modular decomposition, Tarjan SCC, MST.

## License

MIT