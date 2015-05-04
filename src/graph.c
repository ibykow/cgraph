#include "common.h"

static Edge *new_edge(Vert *src, Vert *dest, int weight)
{
    if(!src || !dest)
        return 0;

    Edge *e = (Edge *) malloc(sizeof(Edge));
    if(!e)
        return 0;

    e->src = src;
    e->dest = dest;
    e->weight = weight;
    e->next = 0;
    e->next = e->src->edges;
    e->src->edges = e;

    return e;
}

static void free_vert(Vert *v)
{
    if(!v)
        return;

    Edge *ne;
    for(; v->edges; v->edges = ne) {
        ne = v->edges->next;
        free(v->edges);
    }

    if(v->freev)
        v->freev(v->value);

    free(v);
}

static unsigned grow_graph(Graph *g, unsigned amount)
{
    if (!g || !g->verts)
        return 0;

    Vert **verts = (Vert **) malloc(sizeof(Vert *) * g->len + amount);
    if(!verts)
        return 0;

    unsigned i;
    for(i = 0; i < g->len; i++)
        verts[i] = g->verts[i];

    for(g->len += amount; i < g->len; i++)
        *verts++ = 0;

    free(g->verts);
    g->verts = verts;

    return g->len;
}

static unsigned resize_graph(Graph *g)
{
    if(!g)
        return 0;

    return full_graph(g) ? grow_graph(g, g->len * 2) : g->len;
}

unsigned insert_vert(Graph *g, Vert *v)
{
    if(!g || !v || !id_in_graph(g, v->id) || !resize_graph(g))
        return 0;

    g->next = v;
    v->id = g->next - *g->verts;
    g->next++;

    return g->next - *g->verts;
}

Edge *find_edge(Vert *src, Vert *target)
{
    if(!src || !target)
        return 0;

    Edge *next = src->edges;
    for(next = src->edges;
        next && (next->dest != target);
        next = next->next)
        ;

    return next;
}

Edge *connect_verts(Vert *src, Vert *dest, int weight)
{
    if(!src || !dest)
        return 0;

    Edge *next = find_edge(src, dest);
    if(next)
        return next;

    return new_edge(src, dest, weight);
}

Vert *new_vert(void *val, size_t vsize, void (*freev)(void *))
{
    Vert *v = (Vert *) malloc(sizeof(Vert));
    if(!v)
        return 0;
    v->value = val;
    v->vsize = vsize;
    v->freev = freev;
    v->edges = 0;
    v->id = 0;

    return v;
}

void free_graph(Graph *g)
{
    if(!g)
        return;

    while(g->len--)
        free_vert(g->verts[g->len]);

    if(g->verts)
        free(g->verts);

    free(g);
}

Graph *new_graph(Vert *v)
{
    if(!v)
        return 0;

    Graph *g = (Graph *) malloc(sizeof(Graph));
    if(!g)
        return 0;

    if(!(g->verts = (Vert **) malloc(sizeof(Vert *) * 2))) {
        free(g);
        return 0;
    }

    v->id = 0;
    g->verts[0] = v;
    g->len = 1;
    g->next = g->verts[1];
    g->next = 0;

    return g;
}

void iter_edges(Vert *v, void (*iter)(Edge *e))
{
    Edge *e = v->edges;

    while(e) {
        iter(e);
        e = e->next;
    }
}
