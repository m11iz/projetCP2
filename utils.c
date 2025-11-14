#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static char *getID(int i)
{
    static char buffer[10];
    char temp[10];
    int index = 0;

    // i commence à 1 , on passe en base 26 "A-Z"
    i--;
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

t_cell *create_cell(int dest, float proba)
{
    t_cell *new_cell = (t_cell *)malloc(sizeof(t_cell));
    if (new_cell == NULL)
    {
        perror("Erreur malloc create_cell");
        exit(EXIT_FAILURE);
    }

    new_cell->dest = dest;
    new_cell->proba = proba;
    new_cell->next = NULL;

    return new_cell;
}

t_list create_empty_list(void)
{
    t_list l;
    l.head = NULL;
    return l;
}

void add_cell_to_list(t_list *p_list, int dest, float proba)
{
    t_cell *new_cell = create_cell(dest, proba);
    new_cell->next = p_list->head;
    p_list->head = new_cell;
}

void print_list(const t_list *p_list)
{
    const t_cell *cur = p_list->head;

    printf("[head]");
    while (cur != NULL)
    {
        printf(" -> (%d, %.2f)", cur->dest, cur->proba);
        cur = cur->next;
    }
    printf("\n");
}

t_adj_list create_empty_adj_list(int nb_vertices)
{
    t_adj_list adj;
    adj.nb_vertices = nb_vertices;

    adj.array = (t_list *)malloc(nb_vertices * sizeof(t_list));
    if (adj.array == NULL)
    {
        perror("Erreur malloc create_empty_adj_list");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nb_vertices; i++)
    {
        adj.array[i] = create_empty_list();
    }

    return adj;
}

t_adj_list readGraph(const char *filename)
{
    FILE *file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;

    if (file == NULL)
    {
        perror("Impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Lecture du nombre de sommets impossible");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    t_adj_list adj = create_empty_adj_list(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
    {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert)
        {
            fprintf(stderr,
                    "Ligne invalide dans le fichier : %d %d %f (ignoree)\n",
                    depart, arrivee, proba);
            continue;
        }

        // j'ajoute l'arête dans la liste du sommet depart
        add_cell_to_list(&adj.array[depart - 1], arrivee, proba);
    }

    fclose(file);
    return adj;
}

void print_adj_list(const t_adj_list *p_adj)
{
    for (int i = 0; i < p_adj->nb_vertices; i++)
    {
        printf("Sommet %d : ", i + 1);
        print_list(&p_adj->array[i]);
    }
}


int check_markov(const t_adj_list *p_adj)
{
    int ok = 1;                 // on suppose vrai au début
    const float EPS = 0.01f;

    for (int i = 0; i < p_adj->nb_vertices; i++)
    {
        float sum = 0.0f;
        t_cell *cur = p_adj->array[i].head;

        while (cur != NULL)
        {
            sum += cur->proba;
            cur = cur->next;
        }

        if (sum < 1.0f - EPS || sum > 1.0f + EPS)
        {
            printf("Sommet %d : somme des probabilites = %.3f -> PAS OK\n",
                   i + 1, sum);
            ok = 0;
        }
    }

    if (ok)
    {
        printf("Le graphe est un graphe de Markov.\n");
    }
    else
    {
        printf("Le graphe n'est PAS un graphe de Markov.\n");
    }

    return ok;
}

void write_mermaid_file(const t_adj_list *p_adj, const char *filename)
{
    FILE *f = fopen(filename, "wt");
    if (f == NULL)
    {
        perror("Impossible de creer le fichier Mermaid");
        return;
    }

    // config par défaut
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "  layout: elk\n");
    fprintf(f, "  theme: neo\n");
    fprintf(f, "  look: neo\n");
    fprintf(f, "---\n\n");

    fprintf(f, "flowchart LR\n");

    // on déclare des sommets
    for (int i = 0; i < p_adj->nb_vertices; i++)
    {
        char *id = getID(i + 1);
        fprintf(f, "%s((%d))\n", id, i + 1);
    }
    fprintf(f, "\n");

    // on déclare des arêtes
    for (int i = 0; i < p_adj->nb_vertices; i++)
    {
        const char *from_id = getID(i + 1);
        t_cell *cur = p_adj->array[i].head;

        while (cur != NULL)
        {
            const char *to_id = getID(cur->dest);
            fprintf(f, "%s -->|%.2f|%s\n", from_id, cur->proba, to_id);
            cur = cur->next;
        }
    }

    fclose(f);
}


void free_adj_list(t_adj_list *p_adj)
{
    if (p_adj == NULL || p_adj->array == NULL)
        return;

    for (int i = 0; i < p_adj->nb_vertices; i++)
    {
        t_cell *cur = p_adj->array[i].head;
        while (cur != NULL)
        {
            t_cell *next = cur->next;
            free(cur);
            cur = next;
        }
        p_adj->array[i].head = NULL;
    }

    free(p_adj->array);
    p_adj->array = NULL;
    p_adj->nb_vertices = 0;
}