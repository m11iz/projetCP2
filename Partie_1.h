//
// Created by Paul Girault on 20/10/2025.
//

#ifndef PARTIE_1_H
#define PARTIE_1_H

#include <stdio.h>
#include <stdlib.h>


// STRUCTURES


// cellule : représente une arête du graphe
typedef struct cellule {
    int sommet_arrivee;        // numéro du sommet d’arrivée
    float proba;               // probabilité de passage
    struct cellule *suiv;      // pointeur vers la cellule suivante
} cellule;

// liste : ensemble des arêtes sortantes d’un sommet
typedef struct {
    cellule *head;             // tête de liste
} liste;

// liste_adjacence
typedef struct {
    int taille;                // nombre de sommets
    liste *tab;                // tableau de listes
} liste_adjacence;


//FONCTIONS


// question 2
cellule *creer_cellule(int arrivee, float proba);
liste creer_liste_vide(void);
void ajouter_cellule(liste *l, int arrivee, float proba);
void afficher_liste(liste l);

// question 3
liste_adjacence readGraph(const char *filename);

// question 4
void verifier_markov(liste_adjacence G);
// question 4
void verifier_markov(liste_adjacence G);

// question 5 - export Mermaid

// génération d'un identifiant de sommet de type A, B, ..., Z, AA, AB, ...
char *getId(int num);

// export du graphe au format Mermaid (.mmd)
void generer_fichier_mermaid(liste_adjacence G, const char *filename);
#endif // PARTIE_1_H