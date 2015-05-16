#ifndef _GRAPH_H_
#define _GRAPH_H_

/* masks */

#define UNDIRECTED_GRAPH_OPT (0)
#define DIRECTED_GRAPH_OPT (1)
#define ACYCLIC_GRAPH_OPT (2)
#define DAG_GRAPH_OPT (DIRECTED_GRAPH_OPT | ACYCLIC_GRAPH_OPT)

/* macros */

#define undirected_graph(g) (g && ((g)->opts == UNDIRECTED_GRAPH_OPT))
#define directed_graph(g) (g && ((g)->opts & DIRECTED_GRAPH_OPT))
#define acyclic_graph(g) (g && ((g)->opts & ACYCLIC_GRAPH_OPT))
#define dag_graph(g) (g && ((g)->opts & DAG_GRAPH_OPT))

#define graph_id_occupied(g, id) \
    ((id < (g)->len) && (graph_vert_at(g, (id)) != 0))

#define graph_has_vert(g, v) \
    (v ? graph_vert_at((g), ((v)->id)) == (v) : 0)

#define graph_connect_ids(g, srcid, destid, w) \
    (graph_connect(g, graph_vert_at((g), (srcid)), graph_vert_at((g), (destid)), w))

#define graph_iter_edges(g, it, o) \
    for_each_edge_node((g)->edge_nodes, it, o, 1)

#define graph_iter_edge_nodes(g, it, o) \
    for_each_edge_node((g)->edge_nodes, it, o, 0)

#define cleanup_edge_inorder_info(info) while(pop_edge_node(&info.stack))

/* types */

typedef struct gedge_s Edge;
typedef struct gedge_node_s EdgeNode;
typedef struct edge_inorder_info_s EdgeInorderInfo;
typedef struct gvert_s Vert;
typedef struct graph_s Graph;

typedef enum tree_order_e {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER
} t_order;

/* data structures */

struct gvert_s {
    void *value, (*freev)(void *);
    size_t vsize;
    EdgeNode *edge_nodes;
    unsigned id;
};

struct gedge_s {
    Vert *src, *dest;
    int weight;
};

struct gedge_node_s {
    Edge *e;
    EdgeNode *left, *right, *next;
};

struct graph_s {
    Vert **verts;
    EdgeNode *edge_nodes;
    unsigned len, max, opts;
};

struct edge_inorder_info_s {
    EdgeNode *current, *stack;
    unsigned goLeft;
};

/* graph functions */

void free_graph(Graph *g);
Graph *new_graph(unsigned opts);

static inline int graph_full(Graph *g)
{
    return g ? (g->len == g->max ? 1 : 0) : -1;
}

/* vertex functions */

Vert *new_vert(void *val, size_t vsize, void (*freev)(void *));
unsigned graph_insert(Graph *g, Vert *v);

static inline Vert *graph_vert_at(Graph *g, unsigned id)
{
    return g && (id < g->len) ? g->verts[id] : 0;
}

/* edge functions */

int graph_connect(Graph *g, Vert *a, Vert *b, unsigned w);
void edge_print(void *v);

/* stack based iteration functions */

void init_edge_inorder_info(EdgeInorderInfo *info, EdgeNode *n);
Edge *next_edge(EdgeInorderInfo *info);

/* other graph search functions. try the macros instead (eg. graph_iter_edges) */

void for_each_edge_node(EdgeNode *n, void (*iter)(void *),
    t_order o, unsigned edge_only);

/* edge lookup by src/dest */

EdgeNode *graph_find_edge_node(Vert *src, Vert *target);

static inline Edge *graph_find_edge(Vert *src, Vert *target)
{
    EdgeNode *n = graph_find_edge_node(src, target);
    return n ? n->e : 0;
}

#endif
