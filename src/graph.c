#include "common.h"

static void _for_each_edge_f(EdgeNode *n, void(*iter)(void *), t_order o, unsigned edge_only)
{
    if(!n)
        return;

    if(o != PRE_ORDER) {
        _for_each_edge_f(n->left, iter, o, edge_only);

        if(o == POST_ORDER)
            _for_each_edge_f(n->right, iter, o, edge_only);
    }

    if(edge_only)
        iter(n->e);
    else
        iter(n);

    switch(o) {
        case PRE_ORDER:
            _for_each_edge_f(n->left, iter, o, edge_only);
        case IN_ORDER:
            _for_each_edge_f(n->right, iter, o, edge_only);
        case POST_ORDER:
        default:
            ;
    }
}

static Edge *new_edge(Vert *src, Vert *dest, int weight)
{
    Edge *e = (Edge *) malloc(sizeof(Edge));
    if(!e)
        return 0;

    e->src = src;
    e->dest = dest;
    e->weight = weight;

    return e;
}

static EdgeNode *new_edge_node(Edge *e)
{
    EdgeNode *n = (EdgeNode *) malloc(sizeof(EdgeNode));
    if(!n)
        return 0;

    n->e = e;
    n->left = 0;
    n->right = 0;

    return n;
}

static EdgeNode *create_edge_node(Vert *src, Vert *dest, int weight)
{
    if(!src || !dest)
        return 0;

    Edge *e = new_edge(src, dest, weight);
    if(!e)
        return 0;

    EdgeNode *n = new_edge_node(e);
    if(!n) {
        free(e);
        return 0;
    }

    return n;
}

static void push_edge_node(EdgeNode **stack, EdgeNode *n)
{
    if(!stack || !n)
        return;

    EdgeNode *next = new_edge_node(0);
    if(!next)
        return;

    next->left = n;
    next->right = *stack;
    *stack = next;
}

static EdgeNode *pop_edge_node(EdgeNode **stack)
{
    if(!stack || !*stack)
        return 0;

    EdgeNode *pop = (*stack)->left,
            *next = (*stack)->right;

    free(*stack);
    *stack = next;

    return pop;
}

static void free_edge_node(void *n)
{
    if(!n)
        return;

    EdgeNode *node = (EdgeNode *) n;

    if(node->e)
        free(node->e);

    free(node);
}

static void free_vert(Vert *v)
{
    if(!v)
        return;

    for_each_edge_node(v, free_edge_node, POST_ORDER);

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

static void insert_edge_node(Vert *v, EdgeNode *next)
{
    EdgeNode *n = v->edge_nodes, *prev;

    while(n) {
        prev = n;
        if(next->e->weight > n->e->weight)
            n = n->right;
        else
            n = n->left;
    }

    if(next->e->weight > prev->e->weight)
        prev->right = next;
    else
        prev->left = next;
}

static Edge *connect_verts(Vert *src, Vert *dest, int weight)
{
    if(!src || !dest)
        return 0;

    Edge *prev = find_edge(src, dest);
    if(prev)
        return prev;

    EdgeNode *n = create_edge_node(src, dest, weight);
    if(!n)
        return 0;

    if(src->edge_nodes)
        insert_edge_node(src, n);
    else
        src->edge_nodes = n;

    return n->e;
}

Edge *find_edge(Vert *src, Vert *target)
{
    if(!src || !src->edge_nodes || !target)
        return 0;

    EdgeInorderInfo info;
    init_edge_inorder_info(&info, src->edge_nodes);
    Edge *next;
    while((next = next_edge(&info)) && (next->dest != target))
        ;

    cleanup_edge_inorder_info(info);

    return next;
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

Vert *new_vert(void *val, size_t vsize, void (*freev)(void *))
{
    Vert *v = (Vert *) malloc(sizeof(Vert));

    if(!v)
        return 0;

    v->value = val;
    v->vsize = vsize;
    v->freev = freev;
    v->edge_nodes = 0;
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
    if (!g || !a || !b || !connect_verts(a, b, w))
        return 0;

    return undirected_graph(g)
        ? !!connect_verts(b, a, w)
        : 1;
}

void init_edge_inorder_info(EdgeInorderInfo *info, EdgeNode *n)
{
    if(!info || !n)
        return;

    info->goLeft = 1;
    info->current = n;
    info->stack = 0;
    push_edge_node(&info->stack, n);
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
    return info->current ? info->current->e : 0;
}

void for_each_edge_f(Vert *v, void (*iter)(void *), t_order o, unsigned edge_only)
{
    if (!v || !iter)
        return;

    _for_each_edge_f(v->edge_nodes, iter, o, edge_only);
}
