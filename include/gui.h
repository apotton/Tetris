/**
 * @file gui.h
 * 
 * @brief Fonctions d'affichage
 * 
 * @author c.garion
*/
#include "SDL2/SDL.h"
#include "tetris.h"

#ifndef GUI_H
#define GUI_H


/**
 * @brief Close the GUI window.
 *
 */
void GUI_close();



/**
 * @brief Initialize the GUI windows.
 *
 * The width and height of the window are specified in number of cells.
 * `ICON_SIZE` is a constant determining the size in pixel of an icon.
 *
 * @param title  a string representing the title of the window
 *
 * @param width  the width of the window (number of cells)
 *
 * @param height the height of the window (number of cells)
 */
void GUI_init();

/**
 * @brief Display a Tetris level.
 */
void GUI_show() ;

/**
 * @brief Get the key pressed by the user
 *
 * @return a char representing the key pressed by the user
 */
int GUI_get_key();

/**
 * @brief Wait for a mouse click on the window
 *
 */
void GUI_wait_click();

/**
 * @brief Fonction qui attend un évènement (un clic ou une touche appuyée)
 * 
 * @return Un array contenant la position du clic ou la touche appuyée
*/
int * get_evenement() ;

/**
 * @brief Wait for time ms.
 *
 * @param time the time to wait (in ms)
 *
 */
void GUI_pause(int time);


#endif
