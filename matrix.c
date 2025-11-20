//
// Created by Paul Girault on 13/11/2025.
//

#include "matrix.h"

#include "matrix.h"
#include <math.h> // Pour fabsf()


// IMPLÉMENTATION - ÉTAPE 1 (MATRICIEL)


// Allocation et initialisation à zéro
t_matrix createEmptyMatrix(int n) {
    t_matrix m;
    m.rows = n;

    // Allocation des lignes
    m.data = (t_mat_elem**)malloc(n * sizeof(t_mat_elem*));
    if (m.data == NULL) {
        perror("Erreur d'allocation pour les lignes de la matrice");
        exit(EXIT_FAILURE);
    }

    // Allocation des colonnes et initialisation à 0.0f
    for (int i = 0; i < n; i++) {
        m.data[i] = (t_mat_elem*)calloc(n, sizeof(t_mat_elem)); // calloc met à zéro
        if (m.data[i] == NULL) {
            perror("Erreur d'allocation pour les colonnes de la matrice");
            // Libérer les lignes déjà allouées avant de quitter
            for (int j = 0; j < i; j++) free(m.data[j]);
            free(m.data);
            exit(EXIT_FAILURE);
        }
    }

    return m;
}

// Libération de la mémoire
void freeMatrix(t_matrix *m) {
    if (m != NULL && m->data != NULL) {
        for (int i = 0; i < m->rows; i++) {
            free(m->data[i]);
        }
        free(m->data);
        m->data = NULL;
        m->rows = 0;
    }
}

// Création de la matrice de transition M à partir de la liste d'adjacence G
t_matrix createTransitionMatrix(liste_adjacence G) {
    int n = G.taille;
    t_matrix M = createEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        // i est l'indice 0-indexé (sommet i+1)
        cellule *tmp = G.tab[i].head;
        while (tmp != NULL) {
            // j est l'indice 0-indexé (sommet tmp->sommet_arrivee)
            int j = tmp->sommet_arrivee - 1;
            if (j >= 0 && j < n) {
                M.data[i][j] = tmp->proba;
            }
            tmp = tmp->suiv;
        }
    }
    return M;
}

// Recopie des valeurs d'une matrice
void copyMatrix(t_matrix dest, t_matrix src) {
    if (dest.rows != src.rows) {
        fprintf(stderr, "Erreur: Les matrices n'ont pas la même taille pour la copie.\n");
        return;
    }
    int n = src.rows;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

// Multiplication de deux matrices: C = A * B
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix C) {
    int n = A.rows;
    if (n != B.rows || n != C.rows) {
        fprintf(stderr, "Erreur: Les matrices ne sont pas de taille compatible pour la multiplication.\n");
        return;
    }

    // Utilisation d'une matrice temporaire pour éviter d'écraser A ou B si C=A ou C=B
    t_matrix temp = createEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            t_mat_elem sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += A.data[i][k] * B.data[k][j];
            }
            temp.data[i][j] = sum;
        }
    }

    // Recopie du résultat dans la matrice C
    copyMatrix(C, temp);

    // Libération de la matrice temporaire
    freeMatrix(&temp);
}

// Calcul de la différence |M - N|
t_mat_elem diffMatrix(t_matrix M, t_matrix N) {
    if (M.rows != N.rows) {
        fprintf(stderr, "Erreur: Les matrices n'ont pas la même taille pour le calcul de différence.\n");
        return -1.0f; // Valeur d'erreur
    }

    int n = M.rows;
    t_mat_elem diff_sum = 0.0f;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // fabsf pour la valeur absolue d'un float
            diff_sum += fabsf(M.data[i][j] - N.data[i][j]);
        }
    }
    return diff_sum;
}

// Affichage d'une matrice
void displayMatrix(t_matrix m) {
    printf("Matrice (%d x %d):\n", m.rows, m.rows);
    for (int i = 0; i < m.rows; i++) {
        printf("  |");
        for (int j = 0; j < m.rows; j++) {
            printf(" %.4f", m.data[i][j]);
        }
        printf(" |\n");
    }
}


// IMPLÉMENTATION ÉTAPE 2 (SOUS-MATRICE)

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    if (compo_index < 0 || compo_index >= part.nb_classes) {
        fprintf(stderr, "Erreur: Indice de classe invalide.\n");
        // Retourne une matrice vide (taille 0)
        t_matrix m;
        m.rows = 0;
        m.data = NULL;
        return m;
    }

    t_classe compo = part.classes[compo_index];
    int n_sub = compo.nb_sommets;
    t_matrix sub_m = createEmptyMatrix(n_sub);

    // Mappe les ID de sommets de la classe à leur nouvel indice 0-indexé dans la sous-matrice

    int *id_to_sub_index = (int*)malloc((matrix.rows + 1) * sizeof(int));
    if (!id_to_sub_index) {
        perror("Erreur d'allocation pour la map d'indices");
        freeMatrix(&sub_m);
        exit(EXIT_FAILURE);
    }

    // Initialisation et remplissage du mapping
    for(int i = 0; i < matrix.rows + 1; i++) id_to_sub_index[i] = -1;
    for (int i = 0; i < n_sub; i++) {
        // compo.sommets[i] est l'ID 1-indexé du sommet
        id_to_sub_index[compo.sommets[i]] = i; // ID_sommet -> Indice_sous-matrice
    }

    // Remplissage de la sous-matrice
    for (int i = 0; i < n_sub; i++) {
        // u_id est l'ID 1-indexé du sommet de départ (ligne)
        int u_id = compo.sommets[i];
        // u_idx est l'indice 0-indexé dans la matrice originale
        int u_idx = u_id - 1;

        for (int j = 0; j < n_sub; j++) {
            // v_id est l'ID 1-indexé du sommet d'arrivée (colonne)
            int v_id = compo.sommets[j];
            // v_idx est l'indice 0-indexé dans la matrice originale
            int v_idx = v_id - 1;

            // Le coefficient dans la sous-matrice est le même que dans la matrice originale
            // pour les sommets appartenant à la classe
            sub_m.data[i][j] = matrix.data[u_idx][v_idx];
        }
    }

    free(id_to_sub_index);
    return sub_m;
}