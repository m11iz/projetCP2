#include "Partie_1.h"
#include "Partie_2.h"
#include "matrix.h"
#include <string.h>
#include <stdio.h>

void exportDistributionRow(int n, t_matrix M_power_n, int start_state_index, const char *filepath, int append_mode) {
    FILE *file = fopen(filepath, append_mode ? "a" : "w");
    if (file == NULL) {
        perror("Erreur: impossible d'ouvrir le fichier d'export CSV");
        return;
    }

    if (start_state_index < 0 || start_state_index >= M_power_n.rows) {
        fprintf(stderr, "Erreur: Indice de ligne invalide pour l'export.\n");
        fclose(file);
        return;
    }

    // Écrire l'en-tête CSV (uniquement au premier appel)
    if (!append_mode) {
        fprintf(file, "Etape");
        for (int j = 0; j < M_power_n.rows; j++) {
            fprintf(file, ",P(Xn=%d)", j + 1);
        }
        fprintf(file, "\n");
    }

    // Écrire le numéro de pas
    fprintf(file, "%d", n);

    // Écrire les probabilités pour la ligne correspondant à l'état de départ
    for (int j = 0; j < M_power_n.rows; j++) {
        fprintf(file, ",%.10f", M_power_n.data[start_state_index][j]);
    }
    fprintf(file, "\n");

    fclose(file);
}

int main() {
    int choix;
    liste_adjacence G;       // structure du graphe
    int graphe_charge = 0;
    char chemin[256];

    // Pour stocker les résultats des Parties 2 et 3
    t_partition partition;
    int partition_calculee = 0;
    int *tab_appartenance = NULL;
    t_link_array *liens = NULL;

    t_matrix M;              // Matrice de transition du graphe
    int matrice_chargee = 0;

    do {
        printf("     PROJET GRAPHES DE MARKOV\n");
        printf("--- Partie 1 ---\n");
        printf("1. Charger un graphe depuis un fichier\n");
        printf("2. Afficher la liste d'adjacence\n");
        printf("3. Vérifier si le graphe est de Markov\n");
        printf("6. Exporter le graphe au format Mermaid (.mmd)\n");
        printf("\n--- Partie 2 ---\n");
        printf("4. Calculer les classes (Tarjan)\n");
        printf("7. Diagramme de Hasse + analyse des classes\n");
        printf("\n--- Partie 3 ---\n");
        printf("5. Calculs de distributions (Matriciel/Export CSV)\n");
        printf("-----------------------------\n");
        printf("0. Quitter\n");
        printf("-----------------------------\n");
        printf("Votre choix : ");

        // --- GESTION DE LA SAISIE CORRIGÉE (alex)
        if (scanf("%d", &choix) != 1) {

            printf("\n❌ Entrée invalide. Veuillez entrer un numéro.\n");
            choix = -1;


            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            getchar();
        }


        switch (choix) {
            case 1: {
                printf("\nEntrez le chemin du fichier à charger (ex: ../data/test.txt) : ");
                fgets(chemin, sizeof(chemin), stdin);
                chemin[strcspn(chemin, "\n")] = '\0';

                // Libération des anciennes structures
                if (partition_calculee) {
                    liberer_partition(&partition);
                    partition_calculee = 0;
                }
                if (matrice_chargee) {
                    freeMatrix(&M);
                    matrice_chargee = 0;
                }

                G = readGraph(chemin);
                graphe_charge = 1;
                printf("\n✅ Graphe chargé avec succès depuis '%s'.\n", chemin);
                break;
            }

            case 2:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    printf("\n=== Liste d'adjacence du graphe ===\n");
                    for (int i = 0; i < G.taille; i++) {
                        printf("Sommet %d : ", i + 1);
                        afficher_liste(G.tab[i]);
                    }
                }
                break;

            case 3:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    verifier_markov(G);
                }
                break;

            case 6: {
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                    break;
                }

                char fichier_mmd[256];
                printf("\nEntrez le nom du fichier Mermaid à générer (ex: graphe.mmd) : ");
                fgets(fichier_mmd, sizeof(fichier_mmd), stdin);
                fichier_mmd[strcspn(fichier_mmd, "\n")] = '\0';

                if (strlen(fichier_mmd) == 0) {
                    printf("\n⚠️  Nom de fichier vide.\n");
                    break;
                }

                generer_fichier_mermaid(G, fichier_mmd);
                break;
            }

            case 7: {
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                    break;
                }
                if (!partition_calculee) {
                    printf("\n⚠️  Aucune partition calculée. Utilisez d'abord l'option 4 (Tarjan).\n");
                    break;
                }

                // Libérer d’anciens résultats si on relance
                if (tab_appartenance != NULL) {
                    free(tab_appartenance);
                    tab_appartenance = NULL;
                }
                if (liens != NULL) {
                    free(liens->links);
                    free(liens);
                    liens = NULL;
                }

                printf("\n⚙️  Création du tableau d'appartenance sommet → classe...\n");
                tab_appartenance = creer_tableau_appartenance(partition, G.taille);

                printf("⚙️  Création du graphe des classes (diagramme de Hasse brut)...\n");
                liens = creer_diagramme_hasse(G, partition, tab_appartenance);

                printf("⚙️  Suppression des liens transitifs (Hasse réduit)...\n");
                removeTransitiveLinks(liens);

                printf("⚙️  Génération du fichier Mermaid 'hasse.mmd'...\n");
                afficher_diagramme_mermaid(partition, liens);

                printf("⚙️  Analyse des classes (transitoires, persistantes, absorbantes)...\n");
                analyser_graphe(partition, liens);

                break;
            }

            case 4:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    if (partition_calculee) {
                        liberer_partition(&partition);
                    }
                    printf("\n⚙️  Calcul des composantes fortement connexes (Tarjan)...\n");
                    partition = algorithme_tarjan(G);
                    partition_calculee = 1;
                    afficher_partition(partition);
                }
                break;

            case 5: {
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                    break;
                }

                if (!matrice_chargee) {
                    M = createTransitionMatrix(G);
                    matrice_chargee = 1;
                }

                const int START_STATE = 2;
                const int START_ROW_INDEX = START_STATE - 1;
                const char *EXPORT_FILENAME = "Pi_A_n_data.csv";
                const int MAX_PLOT_STEPS = 50;



                printf("\n=== Calcul de la convergence et Exportation ===\n");
                printf("Matrice P (%d x %d):\n", M.rows, M.rows);
                displayMatrix(M);
                printf("\nDépart: État %d. Exportation de Pi(n) vers '%s' pour n=1 à %d...\n", START_STATE, EXPORT_FILENAME, MAX_PLOT_STEPS);

                // M_curr stocke P^n, M_prev stocker P^(n-1)
                t_matrix M_curr = createTransitionMatrix(G); // Commence à P^1
                t_matrix M_prev = createEmptyMatrix(G.taille);
                t_matrix M_next = createEmptyMatrix(G.taille); // Pour stocker P^(n+1)

                float epsilon = 0.01f;
                float diff = 100.0f;
                int n = 1;

                exportDistributionRow(n, M_curr, START_ROW_INDEX, EXPORT_FILENAME, 0);

                for (n = 2; n <= MAX_PLOT_STEPS; n++) {

                    copyMatrix(M_prev, M_curr);
                    multiplyMatrices(M_curr, M, M_next);
                    copyMatrix(M_curr, M_next);
                    exportDistributionRow(n, M_curr, START_ROW_INDEX, EXPORT_FILENAME, 1); // Export (append_mode=1)
                    diff = diffMatrix(M_curr, M_prev);

                    if (diff < epsilon) {
                        printf("\n✅ Convergence atteinte (|M^n - M^(n-1)| < %.2f) à n = %d.\n", epsilon, n);
                        printf("Différence |M^n - M^(n-1)| = %.6f\n", diff);
                        printf("\n=== Distribution limite approchée (n=%d) ===\n", n);
                        printf("Pi(%d) [Départ État %d]: |", n, START_STATE);
                        for(int j=0; j < G.taille; j++) {
                            printf(" %.4f", M_curr.data[START_ROW_INDEX][j]);
                        }
                        printf(" |\n");
                        break;
                    }
                }

                if (n > MAX_PLOT_STEPS) {
                    printf("\n⚠️  La convergence n'a pas été atteinte après %d itérations (|M^n - M^(n-1)| >= %.2f). Dernière différence: %.6f\n", MAX_PLOT_STEPS, epsilon, diff);
                    printf("La dernière distribution Pi(n) exportée est pour n=%d.\n", MAX_PLOT_STEPS);
                }


                // Libération des matrices
                freeMatrix(&M_curr);
                freeMatrix(&M_prev);
                freeMatrix(&M_next);

                break;
            }

            case 0:
                if (partition_calculee) {
                    liberer_partition(&partition);
                }
                if (matrice_chargee) {
                    freeMatrix(&M);
                }
                if (tab_appartenance != NULL) {
                    free(tab_appartenance);
                }
                if (liens != NULL) {
                    free(liens->links);
                    free(liens);
                }
                printf("\n👋 Fin du programme. Au revoir !\n");
                break;

            default:
                if (choix != -1) {
                    printf("\n❌ Choix invalide. Réessayez.\n");
                }
        }
    } while (choix != 0);

    return 0;
}
