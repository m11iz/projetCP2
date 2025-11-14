#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct s_cell
{
    int dest;
    float proba;
    struct s_cell *next;
} t_cell;

typedef struct s_list
{
    t_cell *head;
} t_list;


typedef struct s_adj_list
{
    int nb_vertices;
    t_list *array;
} t_adj_list;


t_cell *create_cell(int dest, float proba);
t_list create_empty_list(void);
void add_cell_to_list(t_list *p_list, int dest, float proba);
void print_list(const t_list *p_list);
t_adj_list create_empty_adj_list(int nb_vertices);
t_adj_list readGraph(const char *filename);
void print_adj_list(const t_adj_list *p_adj);
int check_markov(const t_adj_list *p_adj);
void write_mermaid_file(const t_adj_list *p_adj, const char *filename);
void free_adj_list(t_adj_list *p_adj);

#endif