#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

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

// Removes leading and trailing whitespace from a string
void trim(char *str) {
    char *start = str;
    while(*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n')
        start++;
    if (start != str)
        memmove(str, start, strlen(start) + 1);

    char *end = str + strlen(str) - 1;
    while(end >= str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        *end-- = '\0';
}

// Finds the index of a node by its name in the array
int find_node_index(Node **nodes, int nb_nodes, const char *name) {
    for (int i = 0; i < nb_nodes; i++)
        if (strcmp(nodes[i]->name, name) == 0)
            return i;
    return -1;
}

// Adds a node if it does not already exist and returns its index
int add_node(Node ***nodes, int *nb_nodes, const char *name) {
    int idx = find_node_index(*nodes, *nb_nodes, name);
    if (idx != -1) return idx;

    *nodes = realloc(*nodes, (*nb_nodes + 1) * sizeof(Node*));
    (*nodes)[*nb_nodes] = calloc(1, sizeof(Node));
    strcpy((*nodes)[*nb_nodes]->name, name);
    return (*nb_nodes)++;
}

// Moves the file cursor to a given section name
void go_to_section(FILE *file, const char *section) {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file))
        if (!strncmp(buffer, section, strlen(section)))
            return;
}

// Counts how many links exist in the #links section of the file
int count_links(char *path) {
    FILE *f = fopen(path, "r");
    if (!f) exit(BAD_FILE_FORMAT);
    go_to_section(f, "#links");
    char buffer[256];
    int nb = 0;

    while (fgets(buffer, sizeof(buffer), f))
        if (strchr(buffer, '-'))
            nb++;

    fclose(f);
    return nb;
}

// Counts how many node lines appear before the #start section
void number_nodes(FILE *file, int *nb) {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        trim(buffer);
        if (!strcmp(buffer,"#start")) break;
        if (buffer[0]>='0' && buffer[0]<='9')
            (*nb)++;
    }
}

// Returns total number of nodes found in the file
int count_nodes(char *path){
    int nb = 0;
    FILE *f = fopen(path,"r");
    if (!f) exit(BAD_FILE_FORMAT);
    number_nodes(f,&nb);
    fclose(f);
    return nb;
}

// Reads the next line and returns it as an integer
int get_single_value_after(FILE *file) {
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    trim(buffer);
    return atoi(buffer);
}

// Returns the start node value after #start
int start_node(char *path){
    FILE *f = fopen(path,"r");
    if (!f) exit(NO_START_NODE);
    go_to_section(f,"#start");
    int v = get_single_value_after(f);
    fclose(f);
    return v;
}

// Returns the end node value after #end
int end_node(char *path){
    FILE *f = fopen(path,"r");
    if (!f) exit(NO_END_NODE);
    go_to_section(f,"#end");
    int v = get_single_value_after(f);
    fclose(f);
    return v;
}

// Reads nodes after #start and marks the first one as start
void start_node_init(FILE *file, Node ***nodes, int *nb) {
    char buffer[256];
    int start_set = 0;
    long pos = ftell(file);

    while (fgets(buffer, sizeof(buffer), file)) {
        trim(buffer);
        if (buffer[0] == '#') break;

        int idx = add_node(nodes, nb, buffer);

        if (!start_set) {
            (*nodes)[idx]->is_start = 1;
            start_set = 1;
        }
    }

    fseek(file, pos, SEEK_SET);
}

// Reads nodes after #end and marks the first one as end
void end_node_init(FILE *file, Node ***nodes, int *nb) {
    char buffer[256];
    int end_set = 0;
    long pos = ftell(file);

    while (fgets(buffer,sizeof(buffer),file)) {
        trim(buffer);
        if (buffer[0]=='#') break;

        int idx = add_node(nodes,nb,buffer);

        if (!end_set) {
            (*nodes)[idx]->is_end = 1;
            end_set = 1;
        }
    }
    fseek(file,pos,SEEK_SET);
}

// Adds a two-way link between two nodes
void add_links_from_buffer(char *buffer, Node ***nodes, int *nb) {
    char a[64], b[64];
    if (sscanf(buffer, "%63[^-]-%63s", a, b) != 2)
        return;

    trim(a); trim(b);

    int ia = add_node(nodes, nb, a);
    int ib = add_node(nodes, nb, b);

    (*nodes)[ia]->links = realloc((*nodes)[ia]->links,
        ((*nodes)[ia]->nb_links + 1) * sizeof(Node*));
    (*nodes)[ia]->links[(*nodes)[ia]->nb_links++] = (*nodes)[ib];

    (*nodes)[ib]->links = realloc((*nodes)[ib]->links,
        ((*nodes)[ib]->nb_links + 1) * sizeof(Node*));
    (*nodes)[ib]->links[(*nodes)[ib]->nb_links++] = (*nodes)[ia];
}

// Reads full file and builds the node graph
Node **init_node(char *filename, int *out) {
    FILE *file = fopen(filename, "r");
    if (!file) exit(FILE_NOT_FOUND);

    Node **nodes = NULL;
    int nb = 0;
    char buffer[256];
    int in_nodes = 0, in_links = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        trim(buffer);
        if (!strlen(buffer)) continue;
        if (!strncmp(buffer, "#nodes", 6)) {in_nodes=1; in_links=0; continue;}
        if (!strncmp(buffer, "#start", 6)) {in_nodes=0; in_links=0; start_node_init(file,&nodes,&nb); continue;}
        if (!strncmp(buffer, "#end",   4)) {in_nodes=0; in_links=0; end_node_init(file,&nodes,&nb); continue;}
        if (!strncmp(buffer, "#links", 6)) {in_links=1; in_nodes=0; continue;}

        if (in_nodes)
            add_node(&nodes, &nb, buffer);
        else if (in_links)
            add_links_from_buffer(buffer, &nodes, &nb);
    }
    fclose(file);
    *out = nb;
    return nodes;
}

// Returns an array of nodes with no links
Node **get_unconnected_node(Node **nodes, int size, Node *head, int *out) {
    Node **u=NULL;
    int c=0;
    for(int i=0;i<size;i++) {
        if(head && nodes[i]==head) continue;
        if(nodes[i]->nb_links==0) {
            u=realloc(u,(c+1)*sizeof(Node*));
            u[c++]=nodes[i];
        }
    }
    *out=c;
    return u;
}

// Frees memory for all nodes and their link arrays
void free_nodes(Node **nodes, int size) {
    for(int i=0;i<size;i++){
        free(nodes[i]->links);
        free(nodes[i]);
    }
    free(nodes);
}

// Find the way into the BFS
int bfs_while(Node **nodes, int size, Node **queue, int *front, int *rear, int *visited, int *parent, int end_idx) {
    Node *cur = queue[(*front)++];
    int cidx = find_node_index(nodes, size, cur->name);

    if (cidx == end_idx)
        return 1; // Found

    for (int i = 0; i < cur->nb_links; i++) {
        int nidx = find_node_index(nodes, size, cur->links[i]->name);
        if (!visited[nidx]) {
            visited[nidx] = 1;
            parent[nidx] = cidx;
            queue[(*rear)++] = cur->links[i];
        }
    }
    return 0; // Not found
}

// Uses BFS to find and print the shortest path
void shortest_path(Node **nodes, int size, const char *start, const char *end){
    int visited[size], parent[size];
    memset(visited,0,sizeof(visited));
    memset(parent,-1,sizeof(parent));

    int start_idx = find_node_index(nodes,size,start);
    int end_idx   = find_node_index(nodes,size,end);

    if(start_idx==-1 || end_idx==-1) {
        printf("No path\n");
        return;
    }

    Node **queue = malloc(size*sizeof(Node*));
    int front=0,rear=0;
    queue[rear++] = nodes[start_idx];
    visited[start_idx]=1;

    int found=0;

    while (front < rear) {
    if (bfs_while(nodes, size, queue, &front, &rear, visited, parent, end_idx)) {
        found = 1;
        break;
    }
    }   

    if(!found){
        printf("No path\n");
        free(queue);
        return;
    }

    int path[size], len=0;
    for(int i=end_idx;i!=-1;i=parent[i])
        path[len++]=i;

    printf("pathfinding:\n");
    for(int i=len-1;i>=0;i--){
        printf("%s",nodes[path[i]]->name);
        if(i) printf(" ");
    }
    printf("\n");
    free(queue);
}
