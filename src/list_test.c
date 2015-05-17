#include "common.h"
#include "test.h"

#define TEST_LIST_LEN 10

static void test_node_iter(Node *n)
{
    fail(!n || !n->value || !n->value->data, "Node iteration failed.");
}

// static void print_node_value(Node *n)
// {
//     if(n && n->value && n->value->data)
//         printf("%s ", n->value->data);
// }

// static void test_empty_list(Node *n)
// {
//     fail(1, "The list is not empty");
// }

void list_test()
{
    unsigned i;
    char *s1 = new_str("Hello", 5), *s2 = new_str("World", 5);
    NodeValue *v = new_node_value(s1, 5, free);
    Node *n1 = new_nodev(v),
        *n2 = new_node(s2, 5, free);

    fail(!s1, "Couldn't create string.");
    fail(!v, "Couldn't create list value.");
    fail(!n1, "Couldn't create list node with value.");
    fail(!n2, "Couldn't create list node.");
    fail(!(v = node_releasev(n1)), "Couldn't release node and value.");
    fail(!(s2 = node_releaset(n2, char)), "Couldn't release node and value.");

    free(s2);

    n1 = n2 = 0;
    node_pushv(&n1, v);
    node_enqv(&n2, new_node_value(new_str("World", 5), 5, free));

    fail(!n1, "Couldn't create stack.");
    for(i = 0; i < TEST_LIST_LEN; i++) {
        s1 = (char *) malloc(sizeof(char) * 4);
        if(!s1)
            continue;

        s2 = (char *) malloc(sizeof(char) * 4);
        if(!s2) {
            free(s1);
            continue;
        }

        snprintf(s1, 4, "%c%c%c", 'a' + (i % 26),
            'a' + ((i + 2) % 26), 'a' + ((i * 2) % 26));

        snprintf(s2, 4, "%s", s1);

        node_pushd(&n1, s1);
        node_enqd(&n2, s2);
    }

    for_each_next_node(n1, test_node_iter);
    for_each_node(n1, test_node_iter, LIST_NEXT);
    free_node_q(&n2);
    free_nodes(n1);
}
