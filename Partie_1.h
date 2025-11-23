#ifndef PARTIE_1_H
#define PARTIE_1_H

#include <stdio.h>
#include <stdlib.h>





// cellule : représente une arête du graphe
typedef struct cellule {
    int sommet_arrivee;
    float proba;
    struct cellule *suiv;
} cellule;


typedef struct {
    cellule *head;
} liste;

// liste_adjacence
typedef struct {
    int taille;
    liste *tab;
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


char *getId(int num);

// export du graphe Mermaid
void generer_fichier_mermaid(liste_adjacence G, const char *filename);
#endif // PARTIE_1_H