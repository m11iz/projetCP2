//
// Created by Paul Girault on 13/11/2025.
//

#ifndef LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H
#define LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include "Partie_1.h" // Pour liste_adjacence
#include "Partie_2.h" // Pour t_partition

// Définition d'un type pour les éléments de la matrice
typedef float t_mat_elem;


// STRUCTURES MATRICES


/**
 * @brief Structure pour une matrice carrée.
 */
typedef struct {
    int rows;           // Nombre de lignes (égal au nombre de colonnes)
    t_mat_elem **data;  // Tableau 2D de données (matrice)
} t_matrix;



// PROTOTYPES FONCTIONS  ÉTAPE 1


/**
 * @brief Crée une matrice n x n remplie avec la valeur 0.
 * @param n Taille de la matrice (n x n).
 * @return La matrice allouée et initialisée à zéro.
 */
t_matrix createEmptyMatrix(int n);

/**
 * @brief Libère la mémoire allouée pour la matrice.
 * @param m Pointeur vers la matrice à libérer.
 */
void freeMatrix(t_matrix *m);

/**
 * @brief Crée une matrice n x n à partir de la liste d'adjacence du graphe.
 * Les coefficients sont les probabilités de passage.
 * @param G La liste d'adjacence du graphe de Markov.
 * @return La matrice d'adjacence M.
 */
t_matrix createTransitionMatrix(liste_adjacence G);

/**
 * @brief Recopie les valeurs d'une matrice source dans une matrice destination
 * (doivent être de même taille).
 * @param dest Matrice destination.
 * @param src Matrice source.
 */
void copyMatrix(t_matrix dest, t_matrix src);

/**
 * @brief Multiplie deux matrices: C = A * B.
 * Les matrices A, B, et C doivent être de même taille n x n.
 * @param A Matrice de gauche.
 * @param B Matrice de droite.
 * @param C Matrice résultat (doit être allouée avant l'appel).
 */
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix C);

/**
 * @brief Calcule 'la différence' entre deux matrices M et N:
 * diff(M,N) = Somme_i Somme_j |m_ij - n_ij|
 * @param M Première matrice.
 * @param N Deuxième matrice.
 * @return La somme des valeurs absolues des différences.
 */
t_mat_elem diffMatrix(t_matrix M, t_matrix N);

/**
 * @brief Affiche le contenu de la matrice sur la console.
 * @param m La matrice à afficher.
 */
void displayMatrix(t_matrix m);



// PROTOTYPES FONCTIONS ÉTAPE 2


/**
 * @brief Extrait la sous-matrice correspondant à une composante du graphe.
 * On ne garde que les lignes et colonnes des sommets de la composante.
 * @param matrix La matrice d'adjacence originale du graphe.
 * @param part La partition du graphe en classes (CFC).
 * @param compo_index L'indice de la classe à extraire (0-indexé).
 * @return La sous-matrice correspondante à la classe.
 */
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);


#endif // MATRIX_H


#endif //LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H