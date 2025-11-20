#ifndef PARTIE_2_H
#define PARTIE_2_H

#include "Partie_1.h" // Pour la structure liste_adjacence
#include "hasse.h"      // <-- AJOUTÉ
#include <stdio.h>
#include <stdlib.h>


// ÉTAPE 1 STRUCTURES ALGORITHME DE TARJAN


/**
 * @brief Structure pour stocker les informations d'un sommet
 * durant l'algorithme de Tarjan.
 */
typedef struct {
    int identifiant;        // Numéro du sommet dans le graphe (ex: 1, 2, 3...)
    int numero;             // Numéro temporaire (ordre de visite). -1 si non visité.
    int numero_accessible;  // Plus petit 'numero' accessible.
    int dans_pile;          // 0 si non, 1 si oui.
} t_tarjan_vertex;

/**
 * @brief Structure de pile (Stack) pour l'algorithme.
 * Implémentée comme un tableau dynamique.
 */
typedef struct {
    int *valeurs;     // Tableau des identifiants (ID) des sommets
    int sommet_pile;  // Indice du sommet de la pile (logical size)
    int capacite;     // Taille physique du tableau (physical size)
} t_pile;

/**
 * @brief Structure pour une classe (Composante Fortement Connexe).
 * Implémentée comme un tableau dynamique de sommets.
 */
typedef struct {
    char nom[10];           // Nom de la classe (ex: "C1")
    int *sommets;           // Tableau des identifiants des sommets (ex: {1, 5, 7})
    int nb_sommets;         // Nombre de sommets dans la classe (logical size)
    int cap_sommets;        // Capacité du tableau (physical size)
} t_classe;

/**
 * @brief Structure pour la partition (ensemble de toutes les classes).
 * Implémentée comme un tableau dynamique de classes.
 */
typedef struct {
    t_classe *classes;      // Tableau de t_classe
    int nb_classes;         // Nombre de classes (logical size)
    int cap_classes;        // Capacité du tableau (physical size)
} t_partition;





// Fonctions utilitaires (Pile)
t_pile* creer_pile(int capacite_initiale);
void empiler(t_pile *p, int id_sommet);
int depiler(t_pile *p);
int est_vide(t_pile *p);
void liberer_pile(t_pile *p);

// Fonctions utilitaires (Classe/Partition)
void ajouter_sommet_classe(t_classe *c, int id_sommet);
void ajouter_classe_partition(t_partition *p, t_classe nouvelle_classe);
void afficher_partition(t_partition p);
void liberer_partition(t_partition *p);

// Fonctions principales (Tarjan)

/**
 * @brief Initialise le tableau des données Tarjan pour chaque sommet.
 */
t_tarjan_vertex* initialiser_tarjan_data(liste_adjacence G);

/**
 * @brief Fonction récursive 'parcours' de l'algorithme de Tarjan.
 */
void tarjan_parcours(int u_id, liste_adjacence G, t_tarjan_vertex *data,
                     t_pile *pile, int *p_index, t_partition *partition);

/**
 * @brief Fonction principale qui exécute l'algorithme de Tarjan.
 * @return La partition du graphe en classes.
 */
t_partition algorithme_tarjan(liste_adjacence G);



// PROTOTYPES FONCTIONS ÉTAPE 2 ET 3


/**
 * @brief Crée un tableau pour savoir à quelle classe appartient chaque sommet.
 * Ex: tab[5] = 1 (le sommet 5 appartient à la classe C1)
 */
int* creer_tableau_appartenance(t_partition partition, int nb_sommets_graphe);

/**
 * @brief Crée le diagramme de Hasse (tableau de liens).
 */
t_link_array* creer_diagramme_hasse(liste_adjacence G, t_partition partition, int* tab_appartenance);

/**
 * @brief Affiche le diagramme au format Mermaid.
 */
void afficher_diagramme_mermaid(t_partition partition, t_link_array *liens);

/**
 * @brief Analyse les classes et affiche les caractéristiques du graphe.
 */
void analyser_graphe(t_partition partition, t_link_array *liens);


#endif //PARTIE_2_H