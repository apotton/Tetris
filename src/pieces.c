#include "tetris.h"

/**
 * Pièce carrée
 */
struct piece o = {
    .taille = 2,
    .couleur = 'J',
    .bas = {0},
    .offset = LARGEUR / 2 - 1,
    .premiere_pos = {0, 0, 0, 0},
    .nb_rotations = 1,
    .largeur = {2, 2, 2, 2},
    .positions = {{{'J', 'J'},
                   {'J', 'J'}},

                  {{'J', 'J'},
                   {'J', 'J'}},

                  {{'J', 'J'},
                   {'J', 'J'}},

                  {{'J', 'J'},
                   {'J', 'J'}}}};

/**
 * Barre
 */
struct piece i = {
    .taille = 4,
    .bas = {2, 0, 1, 0},
    .offset = LARGEUR / 2 - 2,
    .premiere_pos = {0, 2, 0, 1},
    .couleur = 'C',
    .largeur = {4, 1, 4, 1},
    .nb_rotations = 2,
    .positions = {{{'N', 'N', 'N', 'N'},
                   {'N', 'N', 'N', 'N'},
                   {'C', 'C', 'C', 'C'},
                   {'N', 'N', 'N', 'N'}},

                  {{'N', 'N', 'C', 'N'},
                   {'N', 'N', 'C', 'N'},
                   {'N', 'N', 'C', 'N'},
                   {'N', 'N', 'C', 'N'}},

                  {{'N', 'N', 'N', 'N'},
                   {'C', 'C', 'C', 'C'},
                   {'N', 'N', 'N', 'N'},
                   {'N', 'N', 'N', 'N'}},

                  {{'N', 'C', 'N', 'N'},
                   {'N', 'C', 'N', 'N'},
                   {'N', 'C', 'N', 'N'},
                   {'N', 'C', 'N', 'N'}}}};

/**
 * Pièce L
 */
struct piece l = {
    .taille = 3,
    .bas = {1, 0, 0, 0},
    .couleur = 'O',
    .premiere_pos = {0, 1, 0, 0},
    .offset = LARGEUR / 2 - 2,
    .largeur = {3, 2, 3, 2},
    .nb_rotations = 4,
    .positions = {{{'N', 'N', 'O'},
                   {'O', 'O', 'O'},
                   {'N', 'N', 'N'}},

                  {{'N', 'O', 'N'},
                   {'N', 'O', 'N'},
                   {'N', 'O', 'O'}},

                  {{'N', 'N', 'N'},
                   {'O', 'O', 'O'},
                   {'O', 'N', 'N'}},

                  {{'O', 'O', 'N'},
                   {'N', 'O', 'N'},
                   {'N', 'O', 'N'}}}};

/**
 * Pièce S
 */
struct piece s = {
    .taille = 3,
    .bas = {1, 0, 0, 0},
    .nb_rotations = 2,
    .premiere_pos = {0, 1, 0, 0},
    .largeur = {3, 2, 3, 2},
    .offset = LARGEUR / 2 - 2,
    .couleur = 'V',
    .positions = {{{'N', 'V', 'V'},
                   {'V', 'V', 'N'},
                   {'N', 'N', 'N'}},

                  {{'N', 'V', 'N'},
                   {'N', 'V', 'V'},
                   {'N', 'N', 'V'}},

                  {{'N', 'N', 'N'},
                   {'N', 'V', 'V'},
                   {'V', 'V', 'N'}},

                  {{'V', 'N', 'N'},
                   {'V', 'V', 'N'},
                   {'N', 'V', 'N'}}}};

/**
 * Pièce Z
 */
struct piece z = {
    .taille = 3,
    .bas = {1, 0, 0, 0},
    .offset = LARGEUR / 2 - 2,
    .largeur = {3, 2, 3, 2},
    .premiere_pos = {0, 1, 0, 0},
    .couleur = 'R',
    .nb_rotations = 2,
    .positions = {{{'R', 'R', 'N'},
                   {'N', 'R', 'R'},
                   {'N', 'N', 'N'}},

                  {{'N', 'N', 'R'},
                   {'N', 'R', 'R'},
                   {'N', 'R', 'N'}},

                  {{'N', 'N', 'N'},
                   {'R', 'R', 'N'},
                   {'N', 'R', 'R'}},

                  {{'N', 'R', 'N'},
                   {'R', 'R', 'N'},
                   {'R', 'N', 'N'}}}};

/**
 * Pièce J
 */
struct piece j = {
    .taille = 3,
    .bas = {1, 0, 0, 0},
    .offset = LARGEUR / 2 - 2,
    .largeur = {3, 2, 3, 2},
    .premiere_pos = {0, 1, 0, 0},
    .nb_rotations = 4,
    .couleur = 'B',
    .positions = {{{'B', 'N', 'N'},
                   {'B', 'B', 'B'},
                   {'N', 'N', 'N'}},

                  {{'N', 'B', 'B'},
                   {'N', 'B', 'N'},
                   {'N', 'B', 'N'}},

                  {{'N', 'N', 'N'},
                   {'B', 'B', 'B'},
                   {'N', 'N', 'B'}},

                  {{'N', 'B', 'N'},
                   {'N', 'B', 'N'},
                   {'B', 'B', 'N'}}}};

/**
 * Pièce T
 */
struct piece t = {
    .taille = 3,
    .bas = {1, 0, 0, 0},
    .offset = LARGEUR / 2 - 2,
    .premiere_pos = {0, 1, 0, 0},
    .largeur = {3, 2, 3, 2},
    .couleur = 'P',
    .nb_rotations = 4,
    .positions = {{{'N', 'P', 'N'},
                   {'P', 'P', 'P'},
                   {'N', 'N', 'N'}},

                  {{'N', 'P', 'N'},
                   {'N', 'P', 'P'},
                   {'N', 'P', 'N'}},

                  {{'N', 'N', 'N'},
                   {'P', 'P', 'P'},
                   {'N', 'P', 'N'}},

                  {{'N', 'P', 'N'},
                   {'P', 'P', 'N'},
                   {'N', 'P', 'N'}}}};

/**
 * Pièce vide
 */
struct piece rien = {
    .taille = 2,
    .bas = {0},
    .largeur = {2, 2, 2, 2},
    .premiere_pos = {0, 0, 0, 0},
    .offset = LARGEUR / 2 - 1,
    .couleur = 'N',
    .nb_rotations = 1,
    .positions = {{{'N', 'N'},
                   {'N', 'N'}},

                  {{'N', 'N'},
                   {'N', 'N'}},

                  {{'N', 'N'},
                   {'N', 'N'}},

                  {{'N', 'N'},
                   {'N', 'N'}}}};