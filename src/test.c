#include "common.h"

#define TEST_ROUNDS 100
#define NUM_VERTS 10
void fail(unsigned condition, const char reason[])
{
    if(!condition)
        return;

    printf("Fail! %s\n", reason);
    exit(0);
}

void print_vert(Vert *v, const char prefix[])
{
    printf("%s%s (%u)\n", prefix, (char *) v->value, v->id);
}

char *new_str(const char str[], unsigned len)
{
    if (!str || !len)
        return 0;

    char *s = (char *) malloc(sizeof(char) * len + 1);
    if(!s)
        return 0;

    strncpy(s, str, len);
    char *endp = s + len;
    *endp = '\0';

    return s;
}

static unsigned populate_graph(Graph *g, unsigned n)
{
    unsigned i, lost = 0;
    Vert *v;
    for(i = 1; i < n; i++) {
        char *s = new_str("aaa", 3);
        if (!s) {
            lost++;
            continue;
        }

        v = new_vert(s, sizeof(char *), free);

        if(!v) {
            lost++;
            continue;
        }
        s[0] += (char) ur(26);
        s[1] += (char) ur(26);
        s[2] += (char) ur(26);

        insert_vert(g, v);

        connect_ids(g, ur(i), ur(i), ur(i*20));
        connect_ids(g, ur(i), ur(i), ur(i*20));
        connect_ids(g, ur(i), ur(i), ur(i*20));
    }

    return n - lost || 1;
}

int main(int argc, char const *argv[])
{
    unsigned i;
    init_random();

    for(i = 1; i <= TEST_ROUNDS; i++) {

        char s1[] = "hello", s2[] = "world";

        Vert *v1 = new_vert(s1, strlen(s1), 0),
            *v2 = new_vert(s2, strlen(s2), 0);

        fail(!v1 || !v2, "Couldn't create verticies.");

        Graph *g = new_graph(DAG_GRAPH_OPT);

        printf("[round %u ", i);
        fail(!g, "Couldn't create graph.");
        fail(!insert_vert(g, v1), "Couldn't insert v1 into graph.");
        fail(!insert_vert(g, v2), "Couldn't insert v2 into graph.");
        fail(!graph_connect(g, v1, v2, 100), "Couldn't connect verticies.");
        fail(connect_ids(g, 1, 2, 200), "Created a repeat edge.");
        fail(!vert_in_graph(g, v1), "Couldn't find v1.");
        fail(!vert_in_graph(g, v2), "Couldn't find v2.");
        fail(!find_edge_node(v1, v2), "Couldn't find edge between v1 and v2.");
        fail(!vert_at(g, 1), "Couldn't find v1 by id.");
        fail(!vert_at(g, 2), "Couldn't find v2 by id.");
        fail(!populate_graph(g, NUM_VERTS), "Couldn't populate graph.");
        printf("passed]\n");

        for_each_edge(g->edge_nodes, print_edge, IN_ORDER);
        free_graph(g);

    }
    printf("All test rounds passed!\n");
    return 0;
}
