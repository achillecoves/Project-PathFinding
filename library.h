#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>

typedef struct n {
    char name[64];   
    struct n **links;     
    int nb_links; 
    int is_start;   
    int is_end;
} Node;

typedef enum{
    FILE_NOT_FOUND = 1,
    NO_START_NODE = 2,
    NO_END_NODE = 3,
    BAD_FILE_FORMAT = 4
}Error;

void trim(char *str);
int find_node_index(Node **nodes, int nb_nodes, const char *name);
int add_node(Node ***nodes, int *nb_nodes, const char *name);
void go_to_section(FILE *file, const char *section);
int count_links(char *path);
void number_nodes(FILE *file, int *nb);
int count_nodes(char *path);
int get_single_value_after(FILE *file);
int start_node(char *path);
int end_node(char *path);
void start_node_init(FILE *file, Node ***nodes, int *nb);
void end_node_init(FILE *file, Node ***nodes, int *nb);
void add_links_from_buffer(char *buffer, Node ***nodes, int *nb);
Node **init_node(char *filename, int *out);
Node **get_unconnected_node(Node **nodes, int size, Node *head, int *out);
void free_nodes(Node **nodes, int size);
void shortest_path(Node **nodes, int size, const char *start, const char *end);
int bfs_while(Node **nodes, int size, Node **queue, int *front, int *rear, int *visited, int *parent, int end_idx);


#endif
