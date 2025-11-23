

#ifndef TI301_PJT
#define TI301_PJT

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include "Partie_1.h" // Pour liste_adjacence
#include "Partie_2.h" // Pour t_partition

typedef float t_mat_elem;
typedef struct {
    int rows;           // Nombre de lignes
    t_mat_elem **data;  // Tableau 2D de données
} t_matrix;



// FONCTIONS  ÉTAPE 1


t_matrix createEmptyMatrix(int n);
void freeMatrix(t_matrix *m);
t_matrix createTransitionMatrix(liste_adjacence G);
void copyMatrix(t_matrix dest, t_matrix src);
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix C);
t_mat_elem diffMatrix(t_matrix M, t_matrix N);
void displayMatrix(t_matrix m);



// FONCTIONS ÉTAPE 2


t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);


#endif // MATRIX_H
#endif