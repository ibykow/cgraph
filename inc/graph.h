#ifndef _GRAPH_H_
#define _GRAPH_H_

#define UNDIRECTED_GRAPH_OPT (0)
#define DIRECTED_GRAPH_OPT (1)
#define ACYCLIC_GRAPH_OPT (2)
#define DAG_GRAPH_OPT (DIRECTED_GRAPH_OPT | ACYCLIC_GRAPH_OPT)

#define undirected_graph(g) (g && ((g)->opts == UNDIRECTED_GRAPH_OPT))
#define directed_graph(g) (g && ((g)->opts & DIRECTED_GRAPH_OPT))
#define acyclic_graph(g) (g && ((g)->opts & ACYCLIC_GRAPH_OPT))
#define dag_graph(g) (g && ((g)->opts & DAG_GRAPH_OPT))

#define id_in_graph(g, id) (vert_at(g, (id)) != 0)
#define vert_in_graph(g, v) (v ? vert_at((g), ((v)->id)) == (v) : 0)
#define connect_ids(g, srcid, destid, w) \
    (graph_connect(g, vert_at((g), (srcid)), vert_at((g), (destid)), w))

#define for_each_edge(v, it, o) for_each_edge_f(v, it, o, 1)
#define for_each_edge_node(v, it, o) for_each_edge_f(v, it, o, 0)

#define cleanup_edge_inorder_info(info) while(pop_edge_node(&info.stack))

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

static inline Vert *vert_at(Graph *g, unsigned id)
{
    return g && (id < g->len) ? g->verts[id] : 0;
}

static inline int full_graph(Graph *g)
{
    // printf("%s %u %u\n", __func__, g->len, g->max);
    return g ? (g->len == g->max ? 1 : 0) : -1;
}

EdgeNode *find_edge_node(Vert *src, Vert *target);
unsigned insert_vert(Graph *g, Vert *v);
Vert *new_vert(void *val, size_t vsize, void (*freev)(void *));
void free_graph(Graph *g);
Graph *new_graph(unsigned opts);
int graph_connect(Graph *g, Vert *a, Vert *b, unsigned w);
void init_edge_inorder_info(EdgeInorderInfo *info, EdgeNode *n);
Edge *next_edge(EdgeInorderInfo *info);
void for_each_edge_f(EdgeNode *n, void (*iter)(void *), t_order o, unsigned edge_only);
void print_edge(void *v);

#endif
