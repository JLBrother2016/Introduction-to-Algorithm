##Graph

<br><br>

####[Graph Traverse](http://en.wikipedia.org/wiki/Graph_traversal)

>  Graph traversal is the problem of visiting all the nodes in a graph in a particular manner, updating and/or checking their values along the way.

<br>

####[Minimum Spanning Tree](http://en.wikipedia.org/wiki/Minimum_spanning_tree)

> Given a connected, undirected graph, a spanning tree of that graph is a subgraph that is a tree and connects all the vertices together.
> A minimum spanning tree (MST) or minimum weight spanning tree is then a spanning tree with weight less than or equal to the weight of every other spanning tree. 

<br>

####[Single Shortest Path](http://en.wikipedia.org/wiki/Shortest_path_problem#Single-source_shortest_paths)

> The single shortest path problem is the problem of finding a path between a source vertex and another vertex (or node) in a graph such that the sum of the weights of its constituent edges is minimized.

<br>

####[All pairs shortest Path](http://en.wikipedia.org/wiki/Shortest_path_problem#All-pairs_shortest_paths)

>  The all pairs shortest path problem is the problem of finding a path between two vertices (or nodes) in a graph such that the sum of the weights of its constituent edges is minimized.

<br>

####[Maximum flow](http://en.wikipedia.org/wiki/Maximum_flow_problem)

> In optimization theory, maximum flow problems involce finding a feasible flow through a single source, single-sink flow network that is maximum.

<br>

    /Graph
    |--- graph traverse
    		|--- graph_traverse.h graph_traverse.c
    		|--- depth_first_search.c
    		|--- breadth_first_search.c
    		|--- topological_search.c
    		|--- graph_traverse.Makefile
    |--- minimum spanning tree
    		|--- minimum_spanning_tree.h minimum_spanning_tree.c
			|--- boruvka.c
    		|--- kruskal.c
			|--- reverse_delete.c
    		|--- prim.c
    		|--- minimum_spanning_tree.Makefile
    |--- single shortest path
    		|--- single_shortest_path.h single_shortest_path.c
    		|--- bellman_ford.c
    		|--- shortestpath_dag.c
    		|--- dijkstra.c
    		|--- single_shortest_path.Makefile
    |--- all pairs shortest path
			|--- all_pairs_shortest_path.h all_pairs_shortest_path.c
			|--- matrix_multiplication.c
			|--- floyd_warshall.c
			|--- johnson.c
			|--- all_pairs_shortest_path.Makefile
	|--- maximum flow
			|--- maximum_flow.h maximum_flow.c
			|--- ford_fulkerson.c
			|--- maximim_flow.Makefile
