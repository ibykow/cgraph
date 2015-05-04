#include "common.h"

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

void print_edge(Edge *e)
{
    print_vert(e->src, "from:\t");
    print_vert(e->dest, "to:\t");
}

int main(int argc, char const *argv[])
{
    char s1[] = "Hello", s2[] = "World";

    Vert    *v1 = new_vert(s1, sizeof(s1), 0),
            *v2 = new_vert(s2, sizeof(s2), 0);


    fail(!v1 || !v2, "Couldn't create verticies.");

    Graph *g = new_graph(v1);

    fail(!g, "Couldn't create graph.");
    fail(!insert_vert(g, v2), "Couldn't insert v2 into graph.");
    fail(!connect_verts(v1, v2, 1), "Couldn't connect verticies.");
    fail(!vert_in_graph(g, v1), "Couldn't find v1.");
    fail(!find_edge(v1, v2), "Couldn't find edge between v1 and v2.");
    iter_edges(v1, print_edge);

    return 0;
}
