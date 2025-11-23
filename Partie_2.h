#ifndef PARTIE_2_H
#define PARTIE_2_H

#include "Partie_1.h"
#include "hasse.h"
#include <stdio.h>
#include <stdlib.h>


// ÉTAPE 1 STRUCTURES ALGORITHME DE TARJAN


typedef struct {
    int identifiant;        // Numéro du sommet dans le graphe
    int numero;             // Numéro temporaire (ordre de visite)
    int numero_accessible;  // Plus petit num accessible
    int dans_pile;          // 0 si non, 1 si oui
} t_tarjan_vertex;

typedef struct {
    int *valeurs;
    int sommet_pile;
    int capacite;
} t_pile;

typedef struct {
    char nom[10];           // Nom de la classe
    int *sommets;           // Tableau des identifiants des sommets
    int nb_sommets;
    int cap_sommets;
} t_classe;


typedef struct {
    t_classe *classes;
    int nb_classes;
    int cap_classes;
} t_partition;


t_pile* creer_pile(int capacite_initiale);
void empiler(t_pile *p, int id_sommet);
int depiler(t_pile *p);
int est_vide(t_pile *p);
void liberer_pile(t_pile *p);


void ajouter_sommet_classe(t_classe *c, int id_sommet);
void ajouter_classe_partition(t_partition *p, t_classe nouvelle_classe);
void afficher_partition(t_partition p);
void liberer_partition(t_partition *p);

// Fonctions principales (Tarjan)

t_tarjan_vertex* initialiser_tarjan_data(liste_adjacence G);

void tarjan_parcours(int u_id, liste_adjacence G, t_tarjan_vertex *data,
                     t_pile *pile, int *p_index, t_partition *partition);


t_partition algorithme_tarjan(liste_adjacence G);



// FONCTIONS ÉTAPE 2 ET 3

int* creer_tableau_appartenance(t_partition partition, int nb_sommets_graphe);
t_link_array* creer_diagramme_hasse(liste_adjacence G, t_partition partition, int* tab_appartenance);
void afficher_diagramme_mermaid(t_partition partition, t_link_array *liens);
void analyser_graphe(t_partition partition, t_link_array *liens);


#endif //PARTIE_2_H