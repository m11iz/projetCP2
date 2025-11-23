#include "Partie_2.h"
#include <stdlib.h>
#include <string.h>


#define MIN(a, b) (((a) < (b)) ? (a) : (b))



t_pile* creer_pile(int capacite_initiale) {
    t_pile *p = malloc(sizeof(t_pile));
    if (!p) {
        perror("Erreur allocation pile");
        exit(EXIT_FAILURE);
    }
    p->valeurs = malloc(capacite_initiale * sizeof(int));
    if (!p->valeurs) {
        perror("Erreur allocation tableau pile");
        free(p);
        exit(EXIT_FAILURE);
    }
    p->sommet_pile = 0;
    p->capacite = capacite_initiale;
    return p;
}

void empiler(t_pile *p, int id_sommet) {
    if (p->sommet_pile == p->capacite) {
        p->capacite *= 2;
        p->valeurs = realloc(p->valeurs, p->capacite * sizeof(int));
        if (!p->valeurs) {
            perror("Erreur re-allocation pile");
            exit(EXIT_FAILURE);
        }
    }
    p->valeurs[p->sommet_pile] = id_sommet;
    p->sommet_pile++;
}

int depiler(t_pile *p) {
    if (est_vide(p)) {
        fprintf(stderr, "Erreur : la pile est vide.\n");
        return -1;
    }
    p->sommet_pile--;
    return p->valeurs[p->sommet_pile];
}

int est_vide(t_pile *p) {
    return p->sommet_pile == 0;
}

void liberer_pile(t_pile *p) {
    if (p) {
        free(p->valeurs);
        free(p);
    }
}



// Initialise une classe
t_classe creer_classe(char* nom) {
    t_classe c;
    strncpy(c.nom, nom, 10);
    c.nb_sommets = 0;
    c.cap_sommets = 10;
    c.sommets = malloc(c.cap_sommets * sizeof(int));
    if (!c.sommets) {
        perror("Erreur allocation sommets classe");
        exit(EXIT_FAILURE);
    }
    return c;
}

void ajouter_sommet_classe(t_classe *c, int id_sommet) {
    if (c->nb_sommets == c->cap_sommets) {
        c->cap_sommets *= 2;
        c->sommets = realloc(c->sommets, c->cap_sommets * sizeof(int));
        if (!c->sommets) {
            perror("Erreur re-allocation sommets classe");
            exit(EXIT_FAILURE);
        }
    }
    c->sommets[c->nb_sommets] = id_sommet;
    c->nb_sommets++;
}


t_partition creer_partition() {
    t_partition p;
    p.nb_classes = 0;
    p.cap_classes = 10;
    p.classes = malloc(p.cap_classes * sizeof(t_classe));
    if (!p.classes) {
        perror("Erreur allocation partition");
        exit(EXIT_FAILURE);
    }
    return p;
}


void ajouter_classe_partition(t_partition *p, t_classe nouvelle_classe) {
    if (p->nb_classes == p->cap_classes) {
        p->cap_classes *= 2;
        p->classes = realloc(p->classes, p->cap_classes * sizeof(t_classe));
        if (!p->classes) {
            perror("Erreur re-allocation partition");
            exit(EXIT_FAILURE);
        }
    }
    p->classes[p->nb_classes] = nouvelle_classe;
    p->nb_classes++;
}


void afficher_partition(t_partition p) {
    printf("\n=== Partition du graphe (Algorithme de Tarjan) ===\n");
    for (int i = 0; i < p.nb_classes; i++) {
        printf("%s: {", p.classes[i].nom);
        for (int j = 0; j < p.classes[i].nb_sommets; j++) {
            printf("%d", p.classes[i].sommets[j]);
            if (j < p.classes[i].nb_sommets - 1) {
                printf(",");
            }
        }
        printf("}\n");
    }
}


void liberer_partition(t_partition *p) {
    if (p) {
        for (int i = 0; i < p->nb_classes; i++) {
            free(p->classes[i].sommets); // Libère le tableau de sommets de chaque classe
        }
        free(p->classes);
    }
}


// ÉTAPE 1 (TARJAN)



t_tarjan_vertex* initialiser_tarjan_data(liste_adjacence G) {
    t_tarjan_vertex* data = malloc(G.taille * sizeof(t_tarjan_vertex));
    if (!data) {
        perror("Erreur allocation tarjan_data");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < G.taille; i++) {
        data[i].identifiant = i + 1;
        data[i].numero = -1;
        data[i].numero_accessible = -1;
        data[i].dans_pile = 0;
    }
    return data;
}


void tarjan_parcours(int u_id, liste_adjacence G, t_tarjan_vertex *data,
                     t_pile *pile, int *p_index, t_partition *partition)
{
    // u_idx est l'indice 0-indexé du sommet
    int u_idx = u_id - 1;

    // 1. Marquer le sommet 'u' comme visité
    data[u_idx].numero = *p_index;
    data[u_idx].numero_accessible = *p_index;
    (*p_index)++;
    empiler(pile, u_id);
    data[u_idx].dans_pile = 1;

    // 2. Parcourir les voisins v et u
    cellule *voisin = G.tab[u_idx].head;
    while (voisin != NULL) {
        int v_id = voisin->sommet_arrivee;
        int v_idx = v_id - 1;

        if (data[v_idx].numero == -1) {
            // Voisin v non visité
            tarjan_parcours(v_id, G, data, pile, p_index, partition);
            data[u_idx].numero_accessible = MIN(data[u_idx].numero_accessible, data[v_idx].numero_accessible);
        }
        else if (data[v_idx].dans_pile) {
            // Voisin v déjà visité ET dans la pile
            data[u_idx].numero_accessible = MIN(data[u_idx].numero_accessible, data[v_idx].numero);
        }
        voisin = voisin->suiv;
    }

    // 3. Vérifier si u est la racine d'une cfc
    if (data[u_idx].numero_accessible == data[u_idx].numero) {
        // u est une racine
        char nom_classe[10];
        sprintf(nom_classe, "C%d", partition->nb_classes + 1);
        t_classe cfc = creer_classe(nom_classe);

        int w_id;
        do {
            w_id = depiler(pile);
            int w_idx = w_id - 1;
            data[w_idx].dans_pile = 0;
            ajouter_sommet_classe(&cfc, w_id);
        } while (w_id != u_id);

        ajouter_classe_partition(partition, cfc);
    }
}



t_partition algorithme_tarjan(liste_adjacence G) {
    t_partition partition = creer_partition();
    t_tarjan_vertex *data = initialiser_tarjan_data(G);
    t_pile *pile = creer_pile(G.taille);
    int index = 0;
    for (int i = 0; i < G.taille; i++) {
        if (data[i].numero == -1) {
            tarjan_parcours(i + 1, G, data, pile, &index, &partition);
        }
    }

    free(data);
    liberer_pile(pile);

    return partition;
}


//ÉTAPE 2 et 3


int* creer_tableau_appartenance(t_partition partition, int nb_sommets_graphe) {
    int *tab = malloc((nb_sommets_graphe + 1) * sizeof(int));
    if (!tab) {
        perror("Erreur allocation tableau appartenance");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= nb_sommets_graphe; i++) {
        tab[i] = -1;
    }

    for (int c = 0; c < partition.nb_classes; c++) {
        t_classe *classe = &partition.classes[c];
        for (int k = 0; k < classe->nb_sommets; k++) {
            int s = classe->sommets[k];
            if (s >= 1 && s <= nb_sommets_graphe) {
                tab[s] = c;
            }
        }
    }

    return tab;
}

static int lien_existe(t_link_array *arr, int from, int to) {
    for (int i = 0; i < arr->log_size; i++) {
        if (arr->links[i].from == from && arr->links[i].to == to) {
            return 1;
        }
    }
    return 0;
}

t_link_array* creer_diagramme_hasse(liste_adjacence G, t_partition partition, int* tab_appartenance) {
    t_link_array *res = malloc(sizeof(t_link_array));
    if (!res) {
        perror("Erreur allocation t_link_array");
        exit(EXIT_FAILURE);
    }

    res->log_size = 0;
    res->phy_size = 8;
    res->links = malloc(res->phy_size * sizeof(t_link));
    if (!res->links) {
        perror("Erreur allocation links");
        free(res);
        exit(EXIT_FAILURE);
    }

    // Parcourt tous les sommets du graphe
    for (int s = 1; s <= G.taille; s++) {
        int Cs = tab_appartenance[s];
        if (Cs < 0) continue;

        cellule *voisin = G.tab[s - 1].head;
        while (voisin != NULL) {
            int t  = voisin->sommet_arrivee;
            int Ct = tab_appartenance[t];

            if (Ct >= 0 && Cs != Ct) {
                if (!lien_existe(res, Cs, Ct)) {

                    if (res->log_size == res->phy_size) {
                        res->phy_size *= 2;
                        res->links = realloc(res->links, res->phy_size * sizeof(t_link));
                        if (!res->links) {
                            perror("Erreur realloc links");
                            exit(EXIT_FAILURE);
                        }
                    }
                    res->links[res->log_size].from = Cs;
                    res->links[res->log_size].to   = Ct;
                    res->log_size++;
                }
            }

            voisin = voisin->suiv;
        }
    }

    return res;
}
void afficher_diagramme_mermaid(t_partition partition, t_link_array *liens) {
    const char *filename = "hasse.mmd";
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Erreur ouverture fichier Mermaid");
        return;
    }

    // En-tête pour Mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "  layout: elk\n");
    fprintf(f, "  theme: neo\n");
    fprintf(f, "---\n");
    fprintf(f, "flowchart TD\n");


    for (int c = 0; c < partition.nb_classes; c++) {
        t_classe *classe = &partition.classes[c];

        fprintf(f, "%s[\"%s : {", classe->nom, classe->nom);
        for (int k = 0; k < classe->nb_sommets; k++) {
            fprintf(f, "%d", classe->sommets[k]);
            if (k < classe->nb_sommets - 1) {
                fprintf(f, ",");
            }
        }
        fprintf(f, "}\"]\n");
    }

    for (int i = 0; i < liens->log_size; i++) {
        int from = liens->links[i].from;
        int to   = liens->links[i].to;

        if (from >= 0 && from < partition.nb_classes &&
            to   >= 0 && to   < partition.nb_classes) {
            fprintf(f, "%s --> %s\n",
                    partition.classes[from].nom,
                    partition.classes[to].nom);
        }
    }

    fclose(f);
    printf("\n✅ Fichier Mermaid du diagramme de Hasse généré dans '%s'.\n", filename);
}

void analyser_graphe(t_partition partition, t_link_array *liens) {
    int nb_classes = partition.nb_classes;
    if (nb_classes == 0) {
        printf("\nAucune classe dans la partition.\n");
        return;
    }

    int *est_transitoire = calloc(nb_classes, sizeof(int));
    if (!est_transitoire) {
        perror("Erreur allocation est_transitoire");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < liens->log_size; i++) {
        int from = liens->links[i].from;
        if (from >= 0 && from < nb_classes) {
            est_transitoire[from] = 1;
        }
    }

    printf("\n=== Analyse du graphe de Markov ===\n");
    int nb_persistantes = 0;
    int nb_absorbants   = 0;

    for (int c = 0; c < nb_classes; c++) {
        t_classe *classe = &partition.classes[c];

        printf("Classe %s : {", classe->nom);
        for (int k = 0; k < classe->nb_sommets; k++) {
            printf("%d", classe->sommets[k]);
            if (k < classe->nb_sommets - 1) printf(",");
        }
        printf("} -> ");

        if (est_transitoire[c]) {
            printf("transitoire\n");
        } else {
            printf("persistante\n");
            nb_persistantes++;
            if (classe->nb_sommets == 1) {
                printf("   -> L'état %d est absorbant.\n", classe->sommets[0]);
                nb_absorbants++;
            }
        }
    }

    if (nb_classes == 1) {
        printf("\nLe graphe est irréductible (une seule classe).\n");
    } else {
        printf("\nLe graphe n'est pas irréductible (il y a %d classes).\n", nb_classes);
    }

    if (nb_absorbants == 0) {
        printf("Aucun état absorbant.\n");
    } else {
        printf("%d état(s) absorbant(s) au total.\n", nb_absorbants);
    }

    free(est_transitoire);
}