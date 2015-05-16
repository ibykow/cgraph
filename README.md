# cgraph
##### Graph stuff

1. clone
2. cd
3. $ make test

Create a graph:
```
/* graph options are only partially supported */
Graph *g = new_graph(DAG_GRAPH_OPT);
```

Create a vertex:
```
char vertex_value[] = "Hello, World!";
Vert *v = new_vert(vertex_value, strlen(vertex_value) * sizeof(char), 0);
```

Add it to the graph:
```
/* 1 means everything went well, 0 means we failed to allocate memory. */
graph_insert(g, v2);
```

Connect two vertices together:
```
/* returns 0 if an edge between v1 and v2 already exists, or 1 if we inserted a new one. */
unsigned weight = 100;
int success = graph_connect(g, v1, v2, weight);
```

The graph stores the vertices in an array (which grows dynamically).  
The vertex id is the array index at which it is stored.
After inserting a vertex into a graph, check the vertex id:
```
v->id;
```

Connect two vertices using their respective ids:
```
unsigned weight = 200;
graph_connect_ids(g, v1->id, v2->id, weight);
```

Check if a graph contains a vertex:
```
graph_has_vert(g, v);
```

The graph stores edges in a binary search tree sorted by ascending weight.  
Iterate through each edge:
```
/* chose from pre, in, or post order */
graph_iter_edges(g, edge_print, IN_ORDER);
```

Want more in-order flexibility? Set this up:
```
EdgeInorderInfo info;
init_edge_inorder_info(&info, src->edge_nodes);
```

now get the next, in-order edge:
```
Edge *e = next_edge(&info);
```

or keep calling until you're empty:
```
while((e = next_edge(&info)))
    printf("%u\n", e->weight);
```

and clean up:
```
cleanup_edge_inorder_info(info);
```

Free the graph when you're done:
```
/* this frees all graph related memory including any edges and vertices you've added. */
free_graph(g);
```
