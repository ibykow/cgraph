#ifndef _GRAPH_H_
#define _GRAPH_H_

#define id_in_graph(g, id) (vert_at(g, (id)) != 0)
#define vert_in_graph(g, v) (v ? vert_at(g, (v)->id) == v : 0)

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
    Vert **verts, *next;
    unsigned len;
} Graph;

static inline Vert *vert_at(Graph *g, unsigned id)
{
    return g && (id < g->len) ? g->verts[id] : 0;
}

static inline int full_graph(Graph *g)
{
    return g ? ((g->next == (*g->verts + g->len)) ? 1 : 0) : -1;
}

unsigned insert_vert(Graph *g, Vert *v);
Edge *find_edge(Vert *src, Vert *target);
Edge *connect_verts(Vert *src, Vert *dest, int weight);
Vert *new_vert(void *val, size_t vsize, void (*freev)(void *));
void free_graph(Graph *g);
Graph *new_graph(Vert *v);

void iter_edges(Vert *v, void (*iter)(Edge *e));

#endif
