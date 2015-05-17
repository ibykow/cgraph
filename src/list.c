#include "common.h"

// static void _circle_back(Node **np)
// {
//     (*np)->next = *np;
//     (*np)->prev = *np;
//     (*np)->last = *np;
// }

NodeValue *new_node_value(void *data, size_t size, void(*freev)(void *))
{
    NodeValue *v = (NodeValue *) malloc(sizeof(NodeValue));
    if(!v)
        return 0;

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
    return n;
}

void node_push(Node **stack, Node *n)
{
    if(!stack || !n)
        return;

    if(!*stack) {
        *stack = n;
    } else {
        n->next = *stack;
        n->last = (*stack)->last ? (*stack)->last : *stack;

        (*stack)->last = 0;

        if((*stack)->prev) {
            n->prev = (*stack)->prev;
            (*stack)->prev->next = n;
        }

        (*stack)->prev = n;
        *stack = n;
    }
}

void node_pushd(Node **stack, void *d)
{
    if(!stack || !*stack || !(*stack)->value)
        return;

    node_pushv(stack, new_node_value(d,
        (*stack)->value->size, (*stack)->value->freev));
}

Node *node_pop(Node **stack)
{
    if(!stack || !*stack)
        return 0;

    Node *ret = *stack;
    *stack = (*stack)->next;

    return ret;
}

Node *node_deq(Node **q)
{
    if(!q || !*q)
        return 0;

    Node *last = (*q)->last;

    if((*q)->last && (*q)->last->prev && ((*q)->last != *q)) {
        (*q)->last = (*q)->last->prev;
        (*q)->last->next = last->next;
        if(last->next)
            last->next->prev = last->prev;
    } else {
        *q = 0;
    }

    return last;
}

void for_each_node(Node *n, void (*iter)(Node *), list_direct dir)
{
    if(!n)
        return;

    Node *jump = n->next;
    list_direct pdir = dir;

    if (dir == LIST_BOTH)
        dir = LIST_PREV;

    while(n) {
        Node *next = (dir == LIST_NEXT) ? n->next : n->prev;
        iter(n);
        n = next;
    }

    if(pdir == LIST_BOTH)
        for_each_node(jump, iter, LIST_NEXT);
}

void node_deq_each(Node **q, void (*iter)(Node *))
{
    if(!q)
        return;

    if(!iter)
        iter = free_node;

    Node *n;
    while((n = node_deq(q)))
        iter(n);
}
