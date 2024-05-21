/**
 * @file tetric.c
 *
 * @brief Fichier qui définit le fonctionnement d'une partie de Tetris
 *
 * @author Alexandre Potton
 */

#include "tetris.h"

char plateau[LARGEUR + 2][HAUTEUR + 2];
int actif[4][2];
int ombre[4][2];
int pos[2];
int rotation_actuelle;
int indice_piece;
int actuelle_prochaine[2];
int score;

struct piece *pieces[8] = {&o, &i, &l, &s, &z, &j, &t, &rien};
int indice_hold = 7; // Pièce vide pour commencer

void init()
{
    // Initialisation d'une source de nombres aléatoires
    srand((unsigned int)time(NULL));

    // Remplissage du plateau en noir
    for (int x = 1; x <= LARGEUR; x++)
    {
        for (int y = 1; y <= HAUTEUR; y++)
        {
            plateau[x][y] = 'N';
        }
    }

    // Bordures haute et basse
    for (int x = 0; x < LARGEUR + 2; x++)
    {
        plateau[x][HAUTEUR + 1] = '#';
        plateau[x][0] = '#';
    }

    // Bordures latérales
    for (int y = 0; y < HAUTEUR + 2; y++)
    {
        plateau[0][y] = '#';
        plateau[LARGEUR + 1][y] = '#';
    }

    // Indice de la pièce actuelle
    actuelle_prochaine[0] = rand() % 7;
    indice_piece = actuelle_prochaine[0];

    // Indice de la pièce suivante
    actuelle_prochaine[1] = rand() % 7;
}

void print_board()
{
    for (int y = HAUTEUR + 1; y >= 0; y--)
    {
        for (int x = 0; x <= LARGEUR + 1; x++)
        {
            switch (plateau[x][y])
            {
            case 'N':
                printf(BLACK " " DEFAUT);
                break;
            case 'R':
                printf(RED "%c" DEFAUT, plateau[x][y]);
                break;
            case 'C':
                printf(CYAN "%c" DEFAUT, plateau[x][y]);
                break;
            case 'V':
                printf(GREEN "%c" DEFAUT, plateau[x][y]);
                break;
            case 'B':
                printf(BLUE "%c" DEFAUT, plateau[x][y]);
                break;
            case 'J':
                printf(YELLOW "%c" DEFAUT, plateau[x][y]);
                break;
            case 'P':
                printf(PURPLE "%c" DEFAUT, plateau[x][y]);
                break;
            case 'O':
                printf(ORANGE "%c" DEFAUT, plateau[x][y]);
                break;
            default:
                printf(WHITE "%c" DEFAUT, plateau[x][y]);
            }
        }
        printf("\n");
    }
}

bool spawn_possible(struct piece piece)
{
    int hauteur = HAUTEUR - 3 - piece.bas[0];

    for (int x = 0; x < piece.taille; x++)
    {
        for (int y = piece.taille - 1; y >= 0; y--)
        {
            if (piece.positions[0][piece.taille - y - 1][x] != 'N')
            // Si il y a un tetriminos, on regarde ce qu'il y a sur le plateau
            {
                if (plateau[x + piece.offset + 1][y + hauteur] != 'N')
                {
                    return false;
                }
            }
        }
    }

    return true;
}


void spawn_piece(struct piece piece)
{

    // On compte le nombre de blocs qu'on ajoute
    int TetriMino = 0;

    for (int x = 0; x < piece.taille; x++)
    {
        for (int y = piece.taille - 1; y >= 0; y--)
        {
            if (piece.positions[rotation_actuelle][piece.taille - y - 1][x] != 'N')
            {
                plateau[x + pos[0]][y + pos[1]] = piece.positions[rotation_actuelle][piece.taille - y - 1][x];

                ombre[TetriMino][0] = x + pos[0];
                ombre[TetriMino][1] = y + pos[1];

                actif[TetriMino][0] = x + pos[0];
                actif[TetriMino][1] = y + pos[1];

                TetriMino++;
            }
        }
    }
}

void nouvelle_piece()
{   
    struct piece piece = *pieces[actuelle_prochaine[0]];

    // On fixe la rotation à son état de base
    rotation_actuelle = 0;

    // On détermine la position de départ
    pos[0] = piece.offset + 1;
    pos[1] = HAUTEUR - 3 - piece.bas[0];

    // On fait apparaître la pièce
    spawn_piece(piece);

    // Actualisations
    indice_piece = actuelle_prochaine[0];
    actuelle_prochaine[0] = actuelle_prochaine[1];
    actuelle_prochaine[1] = rand() % 7;

    // On réduit les chances de tomber deux fois de suite sur la même pièce
    if (actuelle_prochaine[1] == actuelle_prochaine[0])
    {
        actuelle_prochaine[1] = rand() % 7;
    }
}

bool pos_dans_piece(int positions[4][2], int x, int y)
{
    for (int k = 0; k < 4; k++)
    {
        // Si la position est dans les positions actives
        if ((positions[k][0] == x) && (positions[k][1] == y))
        {
            return true;
        }
    }
    return false;
}

bool descente_possible(int positions[4][2])
{
    for (int k = 0; k < 4; k++)
    {
        // Si la position suivante est occupée par ce que l'on veut faire descendre
        if (pos_dans_piece(positions, positions[k][0], positions[k][1] - 1))
        {
            continue;
        }

        // Si on dépasse par le bas
        if (positions[k][1] - 1 <= 0)
        {
            return false;
        }

        // Si il y a quelque chose dans la case du dessous
        if (plateau[positions[k][0]][positions[k][1] - 1] != 'N')
        {
            return false;
        }
    }

    // Si tous les tests sont passés
    return true;
}

void descendre(int positions[4][2])
{
    // On descend position par position
    for (int k = 0; k < 4; k++)
    {
        positions[k][1]--;
    }

    if (positions == actif)
    {
        // Si la pièce descend, il faut repeindre sur ses positions
        char coul = pieces[indice_piece]->couleur;
        pos[1]--;

        for (int k = 0; k < 4; k++)
        {
            plateau[actif[k][0]][actif[k][1] + 1] = 'N';
        }

        for (int k = 0; k < 4; k++)
            plateau[actif[k][0]][actif[k][1]] = coul;
        {
        }
    }
}

void tomber(int positions[4][2])
{
    while (descente_possible(positions))
    {
        descendre(positions);
    }
}

void actualiser_ombre()
{
    // On remet l'ombre à actif et on la fait tomber
    for (int k = 0; k < 4; k++)
    {
        ombre[k][0] = actif[k][0];
        ombre[k][1] = actif[k][1];
    }
    tomber(ombre);
}

void swap_hold()
{
    for (int k = 0; k < 4; k++)
    {
        plateau[actif[k][0]][actif[k][1]] = 'N';
    }

    if (indice_hold != 7)
    // Si il y a une pièce dans hold
    {
        struct piece temp = *pieces[indice_hold];
        int temp_indice = indice_hold;
        indice_hold = indice_piece;
        indice_piece = temp_indice;
        spawn_piece(temp);
    }
    else
    // Si il n'y a rien dans hold
    {
        indice_hold = indice_piece;
        actuelle_prochaine[1] = rand() % 7;
        if (spawn_possible(*pieces[actuelle_prochaine[0]]))
        {
            nouvelle_piece();
        }
    }
}

bool deplacement_possible(int direction)
{
    for (int k = 0; k < 4; k++)
    {
        if (pos_dans_piece(actif, actif[k][0] + direction, actif[k][1]))
        // Si la position évaluée est déjà dans la pièce
        {
            continue;
        }

        // Dépassements latéraux
        if ((actif[k][0] + GAUCHE <= 0) && (direction == GAUCHE))
        {
            return false;
        }

        if ((actif[k][0] + DROITE > LARGEUR) && (direction == DROITE))
        // Si la position dépasse du plateau
        {
            return false;
        }

        if (plateau[actif[k][0] + direction][actif[k][1]] != 'N')
        // Si il y a quelque chose dans la nouvelle position
        {
            return false;
        }
    }

    return true;
}

void deplacer(int direction)
{
    if (deplacement_possible(direction))
    {
        // On récupère la couleur depuis le plateau
        char couleur = plateau[actif[0][0]][actif[0][1]];

        // On efface la pièce et on la déplace
        for (int k = 0; k < 4; k++)
        {
            plateau[actif[k][0]][actif[k][1]] = 'N';
            actif[k][0] += direction;
        }

        // On retrace la pièce
        for (int k = 0; k < 4; k++)
        {
            plateau[actif[k][0]][actif[k][1]] = couleur;
        }

        pos[0] += direction;
    }
}

void effacer_positions_actif()
{
    for (int k = 0; k < 4; k++)
    {
        plateau[actif[k][0]][actif[k][1]] = 'N';
    }
}

void peindre_actif()
{
    for (int k = 0; k < 4; k++)
    {
        plateau[actif[k][0]][actif[k][1]] = pieces[indice_piece]->couleur;
    }
}

bool rotation_possible(int sens)
{
    bool possible = true;

    // On efface
    effacer_positions_actif();

    // Rotation souhaitée
    int rotation = (rotation_actuelle + 3 * (sens == ANTIHORAIRE) + 1 * (sens == HORAIRE)) % 4;
    struct piece piece = *pieces[indice_piece];

    // On parcourt les positions de la pièce une fois tournée
    for (int x = 0; x < piece.taille; x++)
    {
        for (int y = piece.taille - 1; y >= 0; y--)
        {
            // Si il y a un bloc à cette position
            if (piece.positions[rotation][piece.taille - y - 1][x] != 'N')
            {
                // Si le plateau n'est pas vide à cet endroit
                if (plateau[x + pos[0]][y + pos[1]] != 'N')
                {
                    possible = false;
                }
            }
        }
    }

    // On repeint
    peindre_actif();

    return possible;
}

void tourner(int sens)
{
    // On vérifie que la rotation est possible
    if (rotation_possible(sens))
    {
        // On calcule le nouvel index de rotation
        rotation_actuelle = (rotation_actuelle + 3 * (sens == ANTIHORAIRE) + 1 * (sens == HORAIRE)) % 4;

        // On efface la pièce
        effacer_positions_actif();
        
        // On la replace à l'endroit où elle était
        spawn_piece(*pieces[indice_piece]);
    }
}

void descendre_lignes(int ligne)
{
    for (int nouvelles_lignes = ligne; nouvelles_lignes < HAUTEUR; nouvelles_lignes++)
    {
        for (int colonne = 1; colonne <= LARGEUR; colonne++)
        {
            // Chaque ligne devient celle d'au dessus
            plateau[colonne][nouvelles_lignes] = plateau[colonne][nouvelles_lignes + 1];
        }
    }

    // On remplit la ligne du haut
    for (int colonne = 1; colonne <= LARGEUR; colonne++)
    {
        plateau[colonne][HAUTEUR] = 'N';
    }
}

void eliminer_lignes()
{
    int nombre_lignes = 0;

    for (int ligne = 1; ligne <= HAUTEUR; ligne++)
    {
        bool ligne_complete = true;

        for (int colonne = 1; colonne <= LARGEUR; colonne++)
        {
            // On vérifie si chaque caractère est occupé
            if (plateau[colonne][ligne] == 'N')
            {
                ligne_complete = false;
            }
        }

        if (ligne_complete)
        {
            nombre_lignes++;
            
            // On supprime la ligne
            descendre_lignes(ligne);

            // On reste sur la ligne précédente
            ligne--;
        }
    }

    // On actualise le score
    if (nombre_lignes == 1)
    {
        score += 100;
    }
    if (nombre_lignes == 2)
    {
        score += 400;
    }
    if (nombre_lignes == 3)
    {
        score += 900;
    }
    if (nombre_lignes == 4)
    {
        score += 2000;
    }
}
