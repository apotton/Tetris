#include "stdbool.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"

#ifndef TETRIS_H
#define TETRIS_H

// Couleurs
#define BLACK "\033[1;30m"
#define RED "\033[1;31m"
#define ORANGE "\033[1;38:5:208m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"
#define DEFAUT "\x1B[0m"

// Directions et sens
#define GAUCHE -1
#define DROITE 1
#define HORAIRE 1
#define ANTIHORAIRE -1

// Dimensions (plateau 10x20 normalement)
#define LARGEUR 10
#define HAUTEUR 20

/**
 * Tableau de caractères représentant le plateau
 */
extern char plateau[LARGEUR + 2][HAUTEUR + 2];

/**
 * Liste des positions contenant le Tetrimino actuel:
 * il y a quatre carrées (premier indice), et une
 * abscisse une ordonnée pour chacun (deuxième indice)
 */
extern int actif[4][2];

/**
 * Même principe que pour actif, mais pour "l'ombre"
 * de la pièce: la position qu'elle prendrait si elle
 * venaît à tomber totalememnt
 */
extern int ombre[4][2];

/**
 * Position du centre de la pièce actuelle (format [x,y])
 */
extern int pos[2];

/**
 * Niveau de rotation de la pièce considérée
 */
extern int rotation_actuelle;

/**
 * Infice de la pièce dans le tableau pieces
 */
extern int indice_piece;

/**
 * Indices de la pièce actuelle et de la suivante
 */
extern int actuelle_prochaine[2];

/**
 * Score de la partie en cours
 */
extern int score;

/**
 * Tetriminos (pièces) individuels
 */
struct piece
{
    /**
     * Taille maximale de la pièce (largeur ou longueur)
     */
    int taille;

    /**
     * Couleur de la pièce
     */
    char couleur;

    /**
     * Le nombre de rotations différentes de la pièce, sert au programme ia
     */
    int nb_rotations;

    /**
     * Position du bas selon la rotation
     */
    int bas[4];

    /**
     * Largeur de la pièce selon la rotation
     */
    int largeur[4];

    /**
     * Index de la première position selon la rotation
    */
    int premiere_pos[4];

    /**
     * Décalage en x à partir de la position pour tracer la pièce
     */
    int offset;

    /**
     * Tableau de caractères pour chaque position (max 4x4)
     */
    char positions[4][4][4];
};

extern struct piece o, i, l, s, z, j, t, rien;

/**
 * Indice de la pièce dans le hold
 */
extern int indice_hold;

/**
 * Tableau des pièces
 */
extern struct piece *pieces[8];

/**
 * Initialisation du plateau
 */
void init();

/**
 * Imprime le plan dans la console
 */
void print_board();

/**
 * Détermine si il est possible qu'une pièce apparaisse
 *
 * @param piece La pièce à faire apparaître
 * @return True si c'est possible, False sinon
 */
bool spawn_possible(struct piece piece);

/**
 * Fait apparaître une pièce, sans vérifier que ce soit possible
 *
 * @param piece La pièce à faire apparaître
 */
void spawn_piece(struct piece piece);

/**
 * Met à jour les listes de pièces et en choisit une nouvelle au hasard
 */
void nouvelle_piece();


/**
 * Détermine si une position est dans la pièce active
 *
 * @param positions Là où chercher la pièce
 * @param x Abscisse
 * @param y Ordonnée
 *
 * @return True si oui, False sinon
 */
bool pos_dans_piece(int positions[4][2], int x, int y);

/**
 * Détermine si des positions peuvent aller plus bas
 *
 * @param positions Les positions
 *
 * @return True si oui, False sinon
 */
bool descente_possible(int positions[4][2]);

/**
 * Fait descendre le tableau d'un cran
 *
 * @param positions Les positions
 */
void descendre(int positions[4][2]);

/**
 * Fait descendre l'ombre d'un coup
 *
 * @param positions Les positions
 */
void tomber(int positions[4][2]);

/**
 * Copie le contenu de actif dans ombre (à faire lors d'un changement de pièce)
 */
void actualiser_ombre();

/**
 * Echange la pièce dans hold et la pièce active
 */
void swap_hold();

/**
 * Détermine si un déplacement est possible
 *
 * @param direction La direction (-1 = gauche, 1 = droite)
 *
 * @return True si oui, False sinon
 */
bool deplacement_possible(int direction);

/**
 * Déplace une pièce
 *
 * @param direction La direction du déplacement
 */
void deplacer(int direction);

/**
 * Efface le positions actives du tableau
*/
void effacer_positions_actif() ;

/**
 * Peind les positions actives
*/
void peindre_actif();


/**
 * Détermine si une rotation est possible
 *
 * @param sens Le sens de la rotation (1 = horaire, -1 = antihoraire)
 *
 * @return True si oui, False sinon
 */
bool rotation_possible(int sens);

/**
 * Effectue une rotation de la pièce actuelle
 *
 * @param sens Le sens de rotation (1 pour horaire, -1 pour antihoraire)
 */
void tourner(int sens);

/**
 * Fait descendre toutes les lignes au dessus d'une certaine
 *
 * @param ligne La ligne de départ
 */
void descendre_lignes(int ligne);

/**
 * Vérifie les lignes pleines et les fait sauter
 */
void eliminer_lignes();

#endif