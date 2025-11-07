#include <stdio.h>
#include <stdlib.h>
#include "library.h"


int main() {

    // Declaration of variables
    char *filename = "file.txt";
    int nb_nodes = 0;
    int nb_links = count_links(filename);
    int start = start_node(filename);
    int end = end_node(filename);
    int isolated_count = 0;
    Node **nodes = init_node(filename, &nb_nodes);

    // Print the number of nodes, number of links, the node start and the node end
    printf("nodes: %d\n", nb_nodes);
    printf("links: %d\n", nb_links);
    printf("start: %d\n", start);
    printf("end: %d\n", end);


    // Print the unconnected node
    Node **isolated = get_unconnected_node(nodes, nb_nodes, NULL, &isolated_count);
    printf("unconnected nodes : \n");
    for(int i = 0; i < isolated_count; i++){
        printf("%s ", isolated[i]->name);
    }
    printf("\n");


    // Convert start char into start int
    char start_str[16], end_str[16];
    sprintf(start_str, "%d", start);
    sprintf(end_str, "%d", end);

    //Appel du BFS
    shortest_path(nodes, nb_nodes, start_str, end_str);

    free(isolated);
    free_nodes(nodes, nb_nodes);
    return 0;
}