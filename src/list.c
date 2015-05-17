#include "common.h"

static void _circle_back(Node **np)
{
    (*np)->next = *np;
    (*np)->prev = *np;
    (*np)->last = *np;
}

NodeValue *new_node_value(void *data, size_t size, void(*freev)(void *))
{
    NodeValue *v = (NodeValue *) malloc(sizeof(NodeValue));
    v->data = data, v->size = size, v->freev = freev;
    return v;
}

Node *new_nodev(NodeValue *v)
{
    if(!v)
        return 0;

    Node *n = (Node *) malloc(sizeof(Node));
    if(!n)
        return 0;

    n->value = v;
    n->next = 0, n->prev = 0, n->last = 0;
    return 0;
}

void node_pushv(Node **stack, NodeValue *v)
{
    if(!stack || !v)
        return;

    Node *n = new_nodev(v);
    if(!n)
        return;

    if(!*stack) {
        *stack = n;
        _circle_back(stack);
    } else {
        n->next = *stack;
        n->prev = (*stack)->prev;
        n->last = (*stack)->last;

        (*stack)->last = 0;
        (*stack)->prev->next = n;
        (*stack)->next->prev = n;
        *stack = n;
    }
}

void node_push(Node **stack, void *d)
{
    if(!stack || !*stack || !(*stack)->value || !d)
        return;

    node_pushv(stack, new_node_value(d,
        (*stack)->value->size, (*stack)->value->freev));
}

NodeValue *node_popv(Node **stack)
{
    if(!stack || !*stack)
        return 0;

    Node *next = (*stack)->next;
    NodeValue *v = release_nodev(*stack);
    *stack = next;

    return v;
}

NodeValue *node_deqv(Node **q)
{
    if(!q || !*q || !(*q)->last)
        return 0;

    Node *last = (*q)->last->prev;
    last->next = (*q)->last->next;

    if((*q)->last->next)
        (*q)->last->next->prev = last;

    NodeValue *v = release_nodev((*q)->last);
    (*q)->last = last;
    return v;
}
