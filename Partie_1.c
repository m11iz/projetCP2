//
// Created by Paul Girault on 20/10/2025.
//

#include "Partie_1.h"
#include <string.h>

// Création d'une cellule
cellule *creer_cellule(int arrivee, float proba) {
    cellule *c = malloc(sizeof(cellule));
    if (c == NULL) {
        perror("Erreur d'allocation mémoire pour une cellule");
        exit(EXIT_FAILURE);
    }
    c->sommet_arrivee = arrivee;
    c->proba = proba;
    c->suiv = NULL;
    return c;
}

// Création d'une liste vide
liste creer_liste_vide(void) {
    liste l;
    l.head = NULL;
    return l;
}

// Ajout d'une cellule au début de la liste
void ajouter_cellule(liste *l, int arrivee, float proba) {
    cellule *c = creer_cellule(arrivee, proba);
    c->suiv = l->head;
    l->head = c;
}

// Affichage d'une liste
void afficher_liste(liste l) {
    cellule *tmp = l.head;
    printf("[head]");
    while (tmp != NULL) {
        printf(" -> (%d, %.2f)", tmp->sommet_arrivee, tmp->proba);
        tmp = tmp->suiv;
    }
    printf(" -> NULL\n");
}

//QUESTION 3


// Lecture fichier texte et création de liste d'adjacence
liste_adjacence readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int nbvert, depart, arrivee;
    float proba;

    // Lecture nombre sommets
    if (fscanf(file, "%d", &nbvert) != 1) {
        fprintf(stderr, "Erreur : impossible de lire le nombre de sommets.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialisation du graphe
    liste_adjacence G;
    G.taille = nbvert;
    G.tab = malloc(nbvert * sizeof(liste));
    if (G.tab == NULL) {
        perror("Erreur d'allocation mémoire pour les listes");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialiser toutes les listes vides
    for (int i = 0; i < nbvert; i++) {
        G.tab[i] = creer_liste_vide();
    }

    // Lecture des arêtes
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            fprintf(stderr, "Erreur : sommet hors bornes (%d -> %d)\n", depart, arrivee);
            continue;
        }
        ajouter_cellule(&G.tab[depart - 1], arrivee, proba);
    }

    fclose(file);
    return G;
}

// QUESTION 4


// Vérification de la propriété de Markov
void verifier_markov(liste_adjacence G) {
    printf("\n=== Vérification du graphe de Markov ===\n");
    for (int i = 0; i < G.taille; i++) {
        cellule *tmp = G.tab[i].head;
        float somme = 0.0f;

        // Calcul de la somme des probabilités sortantes du sommet i+1
        while (tmp != NULL) {
            somme += tmp->proba;
            tmp = tmp->suiv;
        }

        // Vérification
        if (somme < 0.99f || somme > 1.01f)
            printf("Sommet %d : somme = %.2f ❌\n", i + 1, somme);
        else
            printf("Sommet %d : somme = %.2f ✅\n", i + 1, somme);
    }
}

/**
 * @brief Génère un identifiant de sommet pour Mermaid à partir d'un numéro.
 *
 * 1  -> "A"
 * 2  -> "B"
 * ...
 * 26 -> "Z"
 * 27 -> "AA"
 * etc.
 *
 * La chaîne retournée est ALLOUÉE dynamiquement : il faut la free()
 * après utilisation.
 */
char *getId(int num) {
    if (num <= 0) {
        return NULL;
    }

    // Calcul de la longueur nécessaire
    int n = num;
    int len = 0;
    while (n > 0) {
        len++;
        n = (n - 1) / 26;
    }

    char *id = malloc((len + 1) * sizeof(char));
    if (!id) {
        perror("Erreur d'allocation mémoire dans getId");
        exit(EXIT_FAILURE);
    }

    id[len] = '\0';
    n = num;
    for (int i = len - 1; i >= 0; --i) {
        int reste = (n - 1) % 26;
        id[i] = (char)('A' + reste);
        n = (n - 1) / 26;
    }

    return id;
}
/**
 * @brief Génère un fichier texte au format Mermaid (.mmd) représentant le graphe.
 *
 * Format compatible avec mermaidchart.com :
 *
 * ---
 * config:
 *   layout: elk
 *   theme: neo
 *   look: neo
 * ---
 * flowchart LR
 * A((1))
 * B((2))
 * A -->|0.50|B
 * ...
 *
 * @param G        Graphe sous forme de liste d'adjacence.
 * @param filename Nom du fichier de sortie (.mmd).
 */
void generer_fichier_mermaid(liste_adjacence G, const char *filename) {
    if (!filename) {
        fprintf(stderr, "Nom de fichier Mermaid invalide.\n");
        return;
    }

    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Erreur lors de l'ouverture du fichier Mermaid en écriture");
        return;
    }

    // En-tête de configuration Mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "  layout: elk\n");
    fprintf(f, "  theme: neo\n");
    fprintf(f, "  look: neo\n");
    fprintf(f, "---\n");
    fprintf(f, "flowchart LR\n");

    // Déclaration des sommets
    for (int i = 0; i < G.taille; ++i) {
        char *id = getId(i + 1); // Sommets numérotés à partir de 1
        if (id) {
            fprintf(f, "%s((%d))\n", id, i + 1);
            free(id);
        }
    }

    // Déclaration des arêtes avec probabilités
    for (int i = 0; i < G.taille; ++i) {
        cellule *tmp = G.tab[i].head;
        while (tmp != NULL) {
            char *from_id = getId(i + 1);
            char *to_id   = getId(tmp->sommet_arrivee);
            if (from_id && to_id) {
                fprintf(f, "%s -->|%.2f| %s\n", from_id, tmp->proba, to_id);
            }
            free(from_id);
            free(to_id);
            tmp = tmp->suiv;
        }
    }

    fclose(f);
    printf("\n✅ Fichier Mermaid généré dans '%s'.\n", filename);
}