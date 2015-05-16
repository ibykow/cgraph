#include "common.h"

// #define copy_edge(e) (new_edge((e)->src, (e)->dest, (e)->weight))

static EdgeNode *new_edge_node(BTEdgeNode *n)
{
    if(!n)
        return 0;

    EdgeNode *en = (EdgeNode *) malloc(sizeof(EdgeNode));
    if(!en)
        return 0;

    en->n = n;
    en->next = 0;
    return en;
}

void push_edge_node(EdgeNode **n, BTEdgeNode *v)
{
    if(!n || !*n || !v)
        return;

    EdgeNode *next = new_edge_node(v);
    if(!next)
        return;

    next->next = *n;
    *n = next;
}

static BTEdgeNode *pop_edge_node(EdgeNode **n)
{
    if(!n || !*n)
        return 0;

    BTEdgeNode *pop = (*n)->n;
    EdgeNode *next = (*n)->next;
    free(*n);
    *n = next;
    return pop;
}

static Edge *_new_edge(Vert *src, Vert *dest, int weight)
{
    Edge *e = (Edge *) malloc(sizeof(Edge));
    if(!e)
        return 0;

    e->src = src;
    e->dest = dest;
    e->weight = weight;

    return e;
}

BTEdgeNode *new_edge(Vert *src, Vert *dest, int weight)
{
    if(!src || !dest)
        return 0;

    Edge *e = _new_edge(src, dest, weight);
    if(!e)
        return 0;

    BTEdgeNode *n = (BTEdgeNode *) malloc(sizeof(BTEdgeNode));
    if(!n)
        return 0;

    n->e = e;
    n->left = 0;
    n->right = 0;

    return n;
}

static void free_bt_edge_node(BTEdgeNode *n)
{
    if(!n)
        return;

    if(n->e)
        free(n->e);

    free(n);
}

static void free_bt_edge_nodes(BTEdgeNode *n) {
    if(!n)
        return;

    if(n->left)
        free_bt_edge_nodes(n->left);
    if(n->right)
        free_bt_edge_nodes(n->right);

    free_bt_edge_node(n);
}

static void free_vert(Vert *v)
{
    if(!v)
        return;

    free_bt_edge_nodes(v->edges);

    if(v->freev)
        v->freev(v->value);

    free(v);
}

static unsigned grow_graph(Graph *g, unsigned amount)
{
    if (!g || !g->verts)
        return 0;

    g->verts = (Vert **) realloc(g->verts, (sizeof(Vert *) * (g->max + amount)));

    if(!g->verts)
        return 0;

    g->max += amount;

    return g->len;
}

static unsigned resize_graph(Graph *g)
{
    if(!g)
        return 0;

    return full_graph(g) ? grow_graph(g, g->len * 2) : g->len;
}

static void insert_edge(Vert *v, BTEdgeNode *dest)
{
    BTEdgeNode *n = v->edges, *prev;
    while(n) {
        prev = n;
        if(dest->e->weight > n->e->weight)
            n = n->right;
        else
            n = n->left;
    }

    if(dest->e->weight > prev->e->weight)
        prev->right = dest;
    else
        prev->left = dest;
}

static Edge *connect_verts(Vert *src, Vert *dest, int weight)
{
    if(!src || !dest)
        return 0;

    Edge *prev = find_edge(src, dest);
    if(prev)
        return prev;

    BTEdgeNode *n = new_edge(src, dest, weight);
    if(!n)
        return 0;

    if(src->edges)
        insert_edge(src, n);
    else
        src->edges = n;

    return n->e;
}

unsigned insert_vert(Graph *g, Vert *v)
{
    if(!g || !v || id_in_graph(g, v->id) || !resize_graph(g))
        return 0;

    g->verts[g->len] = v;
    g->verts[g->len]->id = g->len;
    g->len++;

    return g->len;
}

Edge *find_edge(Vert *src, Vert *target)
{
    if(!src || !target)
        return 0;

    EdgeInorderInfo *info = new_edge_inorder_info(src->edges);
    if(!info)
        return 0;

    Edge *next;
    while((next = next_edge(info)) && (next->dest != target))
        ;

    free_edge_inorder_info(info);

    return next || 0;
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

Graph *new_graph(unsigned opts)
{
    Graph *g = (Graph *) malloc(sizeof(Graph));
    if(!g)
        return 0;

    g->max = 4;

    if(!(g->verts = (Vert **) malloc(sizeof(Vert *) * g->max))) {
        free(g);
        return 0;
    }

    *g->verts = 0;
    g->len = 1;
    g->opts = opts;

    return g;
}

int graph_connect(Graph *g, Vert *a, Vert *b, unsigned w)
{
    if (!g || !a || !b)
        return 0;

    if(!connect_verts(a, b, w))
        return 0;

    return undirected_graph(g)
        ? !!connect_verts(b, a, w)
        : 1;
}

void free_edge_inorder_info(EdgeInorderInfo *info)
{
    if(!info)
        return;

    while(pop_edge_node(&info->stack))
        ;

    free(info);
}

EdgeInorderInfo *new_edge_inorder_info(BTEdgeNode *n)
{
    if(!n)
        return 0;

    EdgeInorderInfo *info = (EdgeInorderInfo *)
        malloc(sizeof(EdgeInorderInfo));

    if(!info)
        return 0;

    info->current = n;
    info->stack = new_edge_node(n);
    info->goLeft = 1;

    return info;
}

Edge *next_edge(EdgeInorderInfo *info)
{
    if (!info)
        return 0;

    for(;;) {
        if(info->goLeft && info->current->left) {
            info->current = info->current->left;
            push_edge_node(&info->stack, info->current);
            continue;
        }

        if(info->goLeft) {
            info->goLeft = 0;
            break;
        }

        if(info->current->right) {
            info->goLeft = 1;
            info->current = info->current->right;
            push_edge_node(&info->stack, info->current);
            continue;
        }

        break;
    }
    info->current = pop_edge_node(&info->stack);
    return info->current->e;
}

void for_each_edge(Vert *v, void (*iter)(Edge *n))
{
    if (!v || !iter)
        return;

    EdgeInorderInfo *info = new_edge_inorder_info(v->edges);
    if(!info)
        return;

    BTEdgeNode *next;

    while((next = next_edge(info)))
        iter(next->e);

    free_edge_inorder_info(info);
}
