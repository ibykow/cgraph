#ifndef _LIST_H__
#define _LIST_H__

typedef struct node_value_s NodeValue;
typedef struct node_s Node;

struct node_value_s {
    void *data;
    size_t size;
    void (*freev)(void *);
};

struct node_s {
    NodeValue *value;
    Node *next, *prev, *last;
};

#define new_node(d, s, f) (new_nodev(new_node_value(d, s, f)))
#define node_pop(sp) (release_noded(node_popv(sp)))

#define node_enqv node_pushv
#define node_enq node_push

#define node_deq(qp) release_noded(node_deqv(qp))

static inline void *release_noded(NodeValue *v)
{
    if(!v || !v->data || !v->freev)
        return 0;

    void *data = v->data;
    free(v);
    return data;
}

static inline NodeValue *release_nodev(Node *n)
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
void node_pushv(Node **stack, NodeValue *v);
void node_push(Node **stack, void *v);
NodeValue *node_popv(Node **stack);
NodeValue *node_deqv(Node **q);

#endif
