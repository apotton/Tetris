#include "tetris.h"
#include "gui.h"

// Nombre très grand
#define INF 10000000

/**
 * Plateau avant la pose de la première pièce
 */
char plateau_stocke1[LARGEUR + 2][HAUTEUR + 2];

/**
 * Plateau avant la pose de la deuxième pièce
 */
char plateau_stocke2[LARGEUR + 2][HAUTEUR + 2];

/**
 * Structure définissant les données pour effectuer un coup
 *
 * @param offset La position horizontale de la pièce
 * @param rotation Le niveau de rotation de la pièce
 * @param eval L'évaluation du coup
 */
struct best_move
{
    int offset;
    int rotation;
    int eval;
    int offset2;
};

/**
 * Le meilleur coup trouvé
 */
struct best_move meilleur = {
    .eval = -INF,
    .rotation = 0,
    .offset = 0};

/**
 * Fait apparaître une pièce dans le plateau de jeu
 *
 * @param indice_piece L'indice de la pièce à ajouter
 * @param rotation Son niveau de rotation
 * @param offset Sa position horizontale
 */
void spawn_piece_ia(int indice_piece, int rotation, int offset)
{
    // On récupère la pièce à ajouter
    struct piece piece = *pieces[indice_piece];
    int bloc = 0;

    // On lit les positions de la pièce de sa première position jusqu'à la dernière
    for (int x = piece.premiere_pos[rotation]; x < piece.premiere_pos[rotation] + piece.largeur[rotation]; x++)
    {
        for (int y = 0; y < piece.taille; y++)
        {
            if (piece.positions[rotation][y][x] != 'N')
            {
                // Si on a trouvé un bloc de la pièce, on l'ajoute au plateau et à actif
                plateau[x - piece.premiere_pos[rotation] + offset + 1][y + HAUTEUR + 1 - piece.taille] = piece.positions[rotation][y][x];

                actif[bloc][0] = x - piece.premiere_pos[rotation] + offset + 1;
                actif[bloc][1] = y + HAUTEUR + 1 - piece.taille;

                bloc++;
            }
        }
    }
}

/**
 * Evalue la hauteur du plateau
 *
 * @return La hauteur du plateau
 */
int hauteur()
{
    int y;

    for (y = HAUTEUR; y > 0; y--)
    {
        for (int x = 1; x <= LARGEUR; x++)
        {
            if (plateau[x][y] != 'N')
            {
                // Dès que l'on trouve une cases non vide, on la renvoie
                return y;
            }
        }
    }

    return 0;
}

/**
 * Evalue à quel point un plateau est plat
 *
 * @return La différence entre la colonne la plus haute et la moins haute
 */
int platitude()
{
    int h = hauteur();

    int max = 0;
    int min = HAUTEUR;

    // On parcourt les colonnes
    for (int x = 1; x <= LARGEUR; x++)
    {
        // On regarde du haut vers le bas
        for (int y = h; y > 0; y--)
        {
            if (plateau[x][y] != 'N')
            {
                // Si on trouve un bloc non vide, on vérifie
                if (y > max)
                {
                    max = y;
                }
                if (y < min)
                {
                    min = y;
                }

                // Pas la peine de vérifier en dessous
                break;
            }

            // Si on est pas sorti de la boucle for avec le break, c'est qu'on est arrivé en bas
            min = 0;
        }
    }

    return max - min;
}

/**
 * Evalue le nombre de trous dans un plateau
 *
 * @return Le nombre de trous du plateau
 */
int nombre_trous()
{
    int trous = 0;
    int hauteur_tableau = hauteur();

    for (int x = 0; x <= LARGEUR; x++)
    {
        for (int y = hauteur_tableau; y > 0; y--)
        {
            // Si il y a une case vide avec une case pleine au dessus
            if ((plateau[x][y] == 'N') && (plateau[x][y + 1] != 'N'))
            {
                trous++;

                // On vérifie si il y a des cases vides en dessous
                int h_sub = y - 1;
                while ((plateau[x][h_sub] == 'N'))
                {
                    trous++;
                    h_sub--;
                }

                // On a vérifié jusqu'à cette ligne
                y = h_sub;
            }
        }
    }

    return trous;
}

/**
 * Fonction d'évaluation d'un plateau. Plus la valeur est basse,
 * moins le plateau est intéressant
 *
 * @return L'évalutation du plateau
 */
int eval()
{
    int h = hauteur();
    int t = nombre_trous();
    int p = platitude();

    return -400 * h * h - 30 * t - 40 * p * p;
}

/**
 * Copie un tableau vers un autre
 *
 * @param tableau_a_copier Le tableau à copier
 * @param tableau_destination Le tableau vers lequel copier
 */
void tableau_vers_tableau(char tableau_a_copier[LARGEUR+2][HAUTEUR+2], char tableau_destination[LARGEUR+2][HAUTEUR+2])
{
    for (int x = 1; x <= LARGEUR; x++)
    {
        for (int y = 1; y <= HAUTEUR; y++)
        {
            tableau_destination[x][y] = tableau_a_copier[x][y];
        }
    }
}

/**
 * Remet le meilleur coup trouvé à zéro
 */
void reset_meilleur()
{
    meilleur.eval = -INF;
}

int main(void)
{
    // Initialisations
    init();
    GUI_init();

    // Détermination des premières pièces
    indice_piece = rand() % 7;
    int indice_suivante = rand() % 7;
    struct piece piece_actuelle;
    struct piece piece_suivante;

    int key = 'f';
    int nombre_coups = 0;

    while (key != 'q' && hauteur() < HAUTEUR - 1)
    {
        // On remet le best_move à zéro
        reset_meilleur();

        // On charge la pièce suivante
        indice_piece = indice_suivante;
        indice_suivante = rand() % 7;

        // Pour minimiser la probabilité d'avoir deux pièces identiques à la suite
        if (indice_suivante == indice_piece)
        {
            indice_suivante = rand() % 7;
        }

        piece_actuelle = *pieces[indice_piece];
        piece_suivante = *pieces[indice_suivante];

        // On stocke le plateau actuel
        tableau_vers_tableau(plateau, plateau_stocke1);

        // On teste toutes les combinaisons possibles pour la première pièce
        for (int rotation1 = 0; rotation1 < piece_actuelle.nb_rotations; rotation1++)
        {
            for (int offset1 = 0; offset1 < LARGEUR + 1 - piece_actuelle.largeur[rotation1]; offset1++)
            {

                tableau_vers_tableau(plateau_stocke1, plateau);

                // Spawn et mouvement
                spawn_piece_ia(indice_piece, rotation1, offset1);
                tomber(actif);
                eliminer_lignes();

                // Stockage
                tableau_vers_tableau(plateau, plateau_stocke2);

                // On teste toutes les combinaisons possibles pour la deuxième pièce
                for (int rotation2 = 0; rotation2 < piece_suivante.nb_rotations; rotation2++)
                {
                    for (int offset2 = 0; offset2 < LARGEUR + 1 - piece_suivante.largeur[rotation2]; offset2++)
                    {
                        tableau_vers_tableau(plateau_stocke2, plateau);

                        // Spawn et mouvement
                        spawn_piece_ia(indice_suivante, rotation2, offset2);
                        tomber(actif);
                        eliminer_lignes();

                        // Evaluation et stockage
                        int evaluation = eval();

                        // Si on a trouvé un meilleur coup, on le stocke
                        // On stocke bien sûr la position de la première pièce
                        if (evaluation > meilleur.eval)
                        {
                            meilleur.rotation = rotation1;
                            meilleur.offset = offset1;
                            meilleur.eval = evaluation;
                        }
                    }
                }
            }
        }

        tableau_vers_tableau(plateau_stocke1, plateau);

        // On effectue le best move trouvé
        spawn_piece_ia(indice_piece, meilleur.rotation, meilleur.offset);
        tomber(actif);
        eliminer_lignes();

        nombre_coups++;

        GUI_show();

        // Commenter ou décommenter cette ligne pour voir coup par coup
        // key = GUI_get_key();
    }

    GUI_close();

    printf("Nombre de coups joués: %d\n", nombre_coups);

    return 0;
}
