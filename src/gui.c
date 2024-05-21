#include "gui.h"

/** Sokoban icon size. */
#define ICON_SIZE 32

/** @brief The SDL renderer. */
static SDL_Renderer *renderer;

/** @brief The SDL window. */
static SDL_Window *window;

/** @brief An array containing all images */
static SDL_Surface *images[9];

/** @brief An array containing all textures */
static SDL_Texture *textures[9];

static SDL_Surface *images_lettres[8];

static SDL_Texture *lettres[8];

static SDL_Surface *images_ombres[9];

static SDL_Texture *ombres[9];

/**
 * Enumération des couleurs pour simplifier le code
 */
enum object
{
    NOIR,
    BLEU,
    VERT,
    CLAIR,
    ORANJE,
    VIOLET,
    ROUGE,
    MUR,
    JAUNE
};

/**
 * @brief The set of `char` values used to represent colors.
 *
 */
static char symbols[] = {'N', 'B', 'V', 'C', 'O', 'P', 'R', '#', 'J'};

/**
 * Les fichiers bitmaps à utiliser pour les blocs, dans le même ordre que les couleurs
 */
static char *image_filename[] = {
    "bitmap/black.bmp",
    "bitmap/blue.bmp",
    "bitmap/red.bmp",
    "bitmap/light_blue.bmp",
    "bitmap/orange.bmp",
    "bitmap/purple.bmp",
    "bitmap/green.bmp",
    "bitmap/wall.bmp",
    "bitmap/yellow.bmp"};

/**
 * Les fichiers bitmaps correspondant aux couleurs des ombres (plus sombres)
 */
static char *ombre_filename[] = {
    "bitmap/o_black.bmp",
    "bitmap/o_blue.bmp",
    "bitmap/o_red.bmp",
    "bitmap/o_light_blue.bmp",
    "bitmap/o_orange.bmp",
    "bitmap/o_purple.bmp",
    "bitmap/o_green.bmp",
    "bitmap/o_wall.bmp",
    "bitmap/o_yellow.bmp"};

/**
 * Des fichiers correspondant aux caractères (NEXT et HOLD)
 */
static char *char_filename[] = {
    "bitmap/N.bmp",
    "bitmap/E.bmp",
    "bitmap/X.bmp",
    "bitmap/T.bmp",
    "bitmap/H.bmp",
    "bitmap/O.bmp",
    "bitmap/L.bmp",
    "bitmap/D.bmp",
};

void gui_close()
{
    for (int obj = NOIR; obj <= JAUNE; obj++)
    {
        SDL_DestroyTexture(textures[obj]);
        textures[obj] = NULL;
    }

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void gui_init()
{
    // Initialisation du module SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error in SDL_Init: %s\n", SDL_GetError());
        exit(-1);
    }

    // Création de la fenêtre
    window = SDL_CreateWindow("Tetris",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              (LARGEUR + 10) * ICON_SIZE,
                              (HAUTEUR + 2) * ICON_SIZE,
                              0);

    // Si la fenêtre n'a pas été crée
    if (window == NULL)
    {
        fprintf(stderr, "Error in SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    // Création du contexte de la fenêtre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    // Si la création du contexte n'a pas fonctionné
    if (renderer == NULL)
    {
        fprintf(stderr, "Error in SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    // Création des événements d'appui de touche et pour quitter la fenêtre
    SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
    SDL_EventState(SDL_QUIT, SDL_ENABLE);

    // Chargement des couleurs principales
    for (int obj = NOIR; obj <= JAUNE; obj++)
    {
        // On associe chaque image à une couleur
        images[obj] = SDL_LoadBMP(image_filename[obj]);

        // Si il manque un fichier image
        if (!images[obj])
        {
            fprintf(stderr, "Missing image %s\n", image_filename[obj]);
            exit(1);
        }

        // Création de texture
        textures[obj] = SDL_CreateTextureFromSurface(renderer, images[obj]);
        SDL_FreeSurface(images[obj]);
    }

    // Chargement des lettres
    for (int k = 0; k < 8; k++)
    {
        images_lettres[k] = SDL_LoadBMP(char_filename[k]);

        if (!images_lettres[k])
        {
            fprintf(stderr, "Missing image %s\n", char_filename[k]);
            exit(1);
        }

        lettres[k] = SDL_CreateTextureFromSurface(renderer, images_lettres[k]);
        SDL_FreeSurface(images_lettres[k]);
    }

    // Chargement des ombres
    for (int k = 0; k < 9; k++)
    {
        images_ombres[k] = SDL_LoadBMP(ombre_filename[k]);

        if (!images_ombres[k])
        {
            fprintf(stderr, "Missing image %s\n", ombre_filename[k]);
            exit(1);
        }

        ombres[k] = SDL_CreateTextureFromSurface(renderer, images_ombres[k]);
        SDL_FreeSurface(images_ombres[k]);
    }
}

void afficher_pieces_cote(struct piece piece, int laquelle)
{
    // Affichage des murs atour des pièces
    for (int x = LARGEUR + 4; x < LARGEUR + 8; x++)
    {
        SDL_Rect area = {
            x * ICON_SIZE,
            (1 + laquelle * 11) * ICON_SIZE,
            ICON_SIZE,
            ICON_SIZE};
        SDL_RenderCopy(renderer, lettres[x - LARGEUR - 4 * (1 - laquelle)], NULL, &area);
    }

    // Si on affiche la pièce suivante, il faut décaler l'affichage de 11 lignes
    int ajout_lignes = 11 * (1 - laquelle);

    // Ajout des murs verticaux
    for (int x = LARGEUR + 2; x <= LARGEUR + 9; x += 7)
    {
        for (int y = 3 + ajout_lignes; y <= 9 + ajout_lignes; y++)
        {
            SDL_Rect area = {
                x * ICON_SIZE,
                (22 - y) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};
            SDL_RenderCopy(renderer, textures[MUR], NULL, &area);
        }
    }

    // Ajout des murs horizontaux
    for (int y = 3 + ajout_lignes; y <= 9 + ajout_lignes; y += 6)
    {
        for (int x = LARGEUR + 2; x < LARGEUR + 10; x++)
        {
            SDL_Rect area = {
                x * ICON_SIZE,
                (22 - y) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};
            SDL_RenderCopy(renderer, textures[MUR], NULL, &area);
        }
    }

    // Détermination de la couleur de la surface
    int couleur = 0;

    for (int coul = NOIR; coul <= JAUNE; coul++)
    {
        if (piece.couleur == symbols[coul])
        {
            couleur = coul;
            break;
        }
    }

    // Affichage de la pièce
    for (int row = 0; row < piece.taille; row++)
    {
        for (int col = 0; col < piece.taille; col++)
        {
            // On positionne l'aire à droite du plateau
            SDL_Rect area = {
                (col + 5 + LARGEUR - (piece.taille == 4)) * ICON_SIZE,
                (17 + (piece.taille == 4) - row - ajout_lignes) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};

            if (piece.positions[(piece.taille == 4)][col][row] == piece.couleur)
            {
                SDL_RenderCopy(renderer, textures[couleur], NULL, &area);
            }
        }
    }
}

void afficher_ombre(char couleur)
{
    int objet;

    for (int obj = NOIR; obj <= JAUNE; obj++)
    {
        if (couleur == symbols[obj])
        {
            objet = obj;
        }
    }

    for (int k = 0; k < 4; k++)
    {
        SDL_Rect area = {
            ombre[k][0] * ICON_SIZE,
            (HAUTEUR + 1 - ombre[k][1]) * ICON_SIZE,
            ICON_SIZE,
            ICON_SIZE};

        SDL_RenderCopy(renderer, ombres[objet], NULL, &area);
    }
}

void show()
{
    char couleur = pieces[indice_piece]->couleur;
    struct piece hold = *pieces[indice_hold];
    struct piece next = *pieces[actuelle_prochaine[0]];

    SDL_RenderClear(renderer);

    for (int row = 0; row < HAUTEUR + 2; row++)
    {
        for (int col = 0; col < LARGEUR + 2; col++)
        {
            SDL_Rect area = {
                col * ICON_SIZE,
                row * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};

            for (int obj = NOIR; obj <= JAUNE; obj++)
            {
                if (plateau[col][HAUTEUR + 1 - row] == symbols[obj])
                {
                    SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                }
            }
        }
    }

    afficher_pieces_cote(next, NEXT);
    afficher_pieces_cote(hold, HOLD);
    afficher_ombre(couleur);

    SDL_RenderPresent(renderer);
}

int get_key()
{
    SDL_Event event;

    do
    {
        SDL_WaitEvent(&event);

        if (event.type == SDL_QUIT)
        {
            gui_close();
            return -1;
        }

    } while (event.type != SDL_KEYDOWN);

    return event.key.keysym.sym;
}

int *get_evenement()
{
    SDL_Event event;

    do
    {
        SDL_WaitEvent(&event);

        if (event.type == SDL_QUIT)
        {
            gui_close();
            int *array = malloc(sizeof(int));
            array[0] = -1;
            return array;
        }

    } while (event.type != SDL_KEYDOWN && event.type != SDL_MOUSEBUTTONDOWN);

    if (event.type == SDL_KEYDOWN)
    {
        int *array = malloc(2 * sizeof(int));
        array[0] = 0;
        array[1] = event.key.keysym.sym;
        return array;
    }
    else
    {
        int *array = malloc(2 * sizeof(int));
        array[0] = 1;
        array[1] = event.button.x;
        array[2] = event.button.y;
        return array;
    }
}

void wait_click()
{
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
    SDL_Event event;

    do
    {
        SDL_WaitEvent(&event);
    } while (event.type != SDL_MOUSEBUTTONDOWN);

    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
}

void pause(int time)
{
    SDL_Delay((Uint32)time);
}
