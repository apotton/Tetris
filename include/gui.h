#include "SDL2/SDL.h"
#include "tetris.h"

#ifndef H
#define H

#define HOLD 1
#define NEXT 0

/**
 * Ferme une fenêtre
 *
 */
void gui_close();

/**
 * Initialise une fenêtre
 */
void gui_init();

/**
 * Affiche les pièces sur le côté du plateau
 *
 * @param piece La pièce à afficher
 *
 * @param laquelle HOLD ou NEXT
 */
void afficher_pieces_cote(struct piece piece, int laquelle);

/**
 * Affiche l'ombre de la pièce actuelle
 *
 * @param couleur La couleur de la pièce
 */
void afficher_ombre(char couleur);

/**
 * Affiche un plateau Tetris
 */
void show();

/**
 * Get the key pressed by the user
 *
 * @return a char representing the key pressed by the user
 */
int get_key();

/**
 * Wait for a mouse click on the window
 *
 */
void wait_click();

/**
 * Fonction qui attend un évènement (un clic ou une touche appuyée)
 *
 * @return Un array contenant la position du clic ou la touche appuyée
 */
int *get_evenement();

/**
 * Wait for time ms.
 *
 * @param time the time to wait (in ms)
 *
 */
void pause(int time);

#endif
