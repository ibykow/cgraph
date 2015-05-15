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

typedef struct gedge_s Edge;

typedef struct gvert_s {
    void *value, (*freev)(void *);
    size_t vsize;
    Edge *edges;
    unsigned id;
} Vert;

struct gedge_s {
    Vert *src, *dest;
    Edge *next;
    int weight;
};

typedef struct graph_s {
    Vert **verts;
    unsigned len, max, opts;
} Graph;

static inline Vert *vert_at(Graph *g, unsigned id)
{
    return g && (id < g->len) ? g->verts[id] : 0;
}

static inline int full_graph(Graph *g)
{
    // printf("%s %u %u\n", __func__, g->len, g->max);
    return g ? (g->len == g->max ? 1 : 0) : -1;
}

unsigned insert_vert(Graph *g, Vert *v);
Edge *find_edge(Vert *src, Vert *target);
Vert *new_vert(void *val, size_t vsize, void (*freev)(void *));
void free_graph(Graph *g);
Graph *new_graph(unsigned opts);
int graph_connect(Graph *g, Vert *a, Vert *b, unsigned w);
void for_each_edge(Vert *v, void (*iter)(Edge *e));

#endif
