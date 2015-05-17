#ifndef _LIST_H__
#define _LIST_H__

typedef struct node_value_s NodeValue;
typedef struct node_s Node;

typedef enum list_direct_e {
    LIST_NEXT,
    LIST_PREV,
    LIST_BOTH
} list_direct;

struct node_value_s {
    void *data;
    size_t size;
    void (*freev)(void *);
};

struct node_s {
    NodeValue *value;
    Node *next, *prev, *last;
};

#define node_releaset(np, type) ((type *) node_released(node_releasev(np)))

#define free_nodes(n) for_each_node(n, free_node, LIST_BOTH)
#define free_node_q(n) node_deq_each(n, 0)

#define new_node(d, s, f) (new_nodev(new_node_value(d, s, f)))

#define node_pushv(sp, v) node_push(sp, new_nodev(v))

#define node_popv(sp) node_releasev(node_pop(sp))
#define node_popd(sp) (node_released(node_popv(sp)))
#define node_popt(sp, type) ((type *) node_popd(sp))

#define node_enqv node_pushv
#define node_enqd node_pushd

#define node_deqv(qp) node_releasev(node_deq(qp))
#define node_deqd(qp) node_released(node_deqv(qp))
#define node_deqt(qp, type) ((type *) (node_deqd(qp)))

#define for_each_next_node(np, it) for_each_node(np, it, LIST_NEXT)
#define for_each_prev_node(np, it) for_each_node(np, it, LIST_PREV)

static inline void *node_released(NodeValue *v)
{
    if(!v || !v->data || !v->freev)
        return 0;

    void *data = v->data;
    free(v);
    return data;
}

static inline NodeValue *node_releasev(Node *n)
{
    if(!n)
        return 0;

    NodeValue *v = n->value;
    free(n);
    return v;
}

static inline void free_nodev(NodeValue *v)
{
    if(!v || !v->data || !v->freev)
        return;

    v->freev(v->data);
    free(v);
}

static inline void free_node(Node *n)
{
    if(!n)
        return;

    free_nodev(n->value);

    free(n);
}

NodeValue *new_node_value(void *data, size_t size, void(*freev)(void *));
Node *new_nodev(NodeValue *v);

void node_push(Node **stack, Node *n);
void node_pushd(Node **stack, void *v);

Node *node_pop(Node **stack);
Node *node_deq(Node **q);

void for_each_node(Node *n, void (*iter)(Node *), list_direct);
void node_deq_each(Node **q, void (*iter)(Node *));

#endif
