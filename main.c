#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage : %s <fichier_graphe>\n", argv[0]);
        return EXIT_FAILURE;
    }

    t_adj_list graph = readGraph(argv[1]);

    printf("Liste d'adjacence lue depuis %s :\n", argv[1]);
    print_adj_list(&graph);

    check_markov(&graph);

    const char *mermaid_filename = "graph_mermaid.mmd";
    write_mermaid_file(&graph, mermaid_filename);
    printf("Fichier Mermaid genere : %s\n", mermaid_filename);

    free_adj_list(&graph);

    return EXIT_SUCCESS;
}
