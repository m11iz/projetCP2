#ifndef __HASSE_H__
#define __HASSE_H__


/**
 * @brief Structure pour un lien (compatible avec hasse.c).
 */
typedef struct {
    int from; // Classe de départ
    int to;   // Classe d'arrivée
} t_link;

/**
 * @brief Structure pour un tableau de liens (compatible avec hasse.c).
 */
typedef struct {
    t_link *links;
    int log_size; // Taille logique (utilisée par hasse.c)
    int phy_size; // Taille physique (pour nos realloc)
} t_link_array;


void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif