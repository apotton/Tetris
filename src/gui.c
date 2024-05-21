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
 * @brief An enumeration to represent all symbols.
 *
 * It can be used to ease the readibility of your code.
 *
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
 * @brief The set of `char` values used to represent maps.
 *
 */
static char symbols[] = {'N', 'B', 'V', 'C', 'O', 'P', 'R', '#', 'J'};

/**
 * @brief The locations of the bitmap images to use.
 *
 * The files must be given in the same order as the symbols.
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

void GUI_close()
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

    // exit(EXIT_SUCCESS);
}

void GUI_init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error in SDL_Init: %s\n", SDL_GetError());
        exit(-1);
    }

    window = SDL_CreateWindow("Tetris",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              (LARGEUR + 10) * ICON_SIZE,
                              (HAUTEUR+2) * ICON_SIZE,
                              0);

    if (window == NULL)
    {
        fprintf(stderr, "Error in SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (renderer == NULL)
    {
        fprintf(stderr, "Error in SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
    SDL_EventState(SDL_QUIT, SDL_ENABLE);

    for (int obj = NOIR; obj <= JAUNE; obj++)
    {
        images[obj] = SDL_LoadBMP(image_filename[obj]);

        if (!images[obj])
        {
            fprintf(stderr, "Missing image %s\n", image_filename[obj]);
            exit(1);
        }

        textures[obj] = SDL_CreateTextureFromSurface(renderer, images[obj]);
        SDL_FreeSurface(images[obj]);
    }

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

void GUI_spawn_next(struct piece piece)
{

    for (int x = LARGEUR + 4; x < LARGEUR + 8; x++)
    {
        SDL_Rect area = {
            x * ICON_SIZE,
            1 * ICON_SIZE,
            ICON_SIZE,
            ICON_SIZE};
        SDL_RenderCopy(renderer, lettres[x - LARGEUR - 4], NULL, &area);
    }

    for (int col = LARGEUR + 2; col <= LARGEUR + 9; col += 7)
    {
        for (int y = 14; y < 21; y++)
        {
            SDL_Rect area = {
                col * ICON_SIZE,
                (HAUTEUR + 2 - y) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};
            SDL_RenderCopy(renderer, textures[MUR], NULL, &area);
        }
    }

    for (int row = 14; row <= 20; row += 6)
    {
        for (int x = LARGEUR + 2; x < LARGEUR + 10; x++)
        {
            SDL_Rect area = {
                x * ICON_SIZE,
                (22 - row) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};
            SDL_RenderCopy(renderer, textures[MUR], NULL, &area);
        }
    }

    if (piece.taille == 2)
    {
        for (int row = 16; row < 18; row++)
        {
            for (int col = LARGEUR + 5; col < LARGEUR + 8; col++)
            {
                SDL_Rect area = {
                    col * ICON_SIZE,
                    (22 - row) * ICON_SIZE,
                    ICON_SIZE,
                    ICON_SIZE};

                for (int obj = NOIR; obj <= JAUNE; obj++)
                {
                    if (piece.positions[0][col - LARGEUR - 5][row - 16] == symbols[obj])
                    {
                        SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                    }
                }
            }
        }
    }

    if (piece.taille == 3)
    {
        for (int row = 16; row < 19; row++)
        {
            for (int col = LARGEUR + 5; col < LARGEUR + 8; col++)
            {
                SDL_Rect area = {
                    col * ICON_SIZE,
                    (22 - row) * ICON_SIZE,
                    ICON_SIZE,
                    ICON_SIZE};

                for (int obj = NOIR; obj <= JAUNE; obj++)
                {
                    if ((piece.positions[0][col - LARGEUR - 5][row - 16] == symbols[obj]) && (piece.positions[0][col - LARGEUR - 5][row - 16] != 'N'))
                    {
                        SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                    }
                }
            }
        }
    }

    if (piece.taille == 4)
    {
        for (int row = 16; row < 19; row++)
        {
            for (int col = LARGEUR + 4; col < LARGEUR + 8; col++)
            {
                SDL_Rect area = {
                    col * ICON_SIZE,
                    (23 - row) * ICON_SIZE,
                    ICON_SIZE,
                    ICON_SIZE};

                for (int obj = NOIR; obj <= JAUNE; obj++)
                {
                    if ((piece.positions[1][col - LARGEUR - 4][row - 16] == symbols[obj]) && (piece.positions[1][col - LARGEUR - 4][row - 16] != 'N'))
                    {
                        SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                    }
                }
            }
        }
    }
}

void GUI_spawn_hold(struct piece piece)
{

    for (int x = LARGEUR + 4; x < LARGEUR + 8; x++)
    {
        SDL_Rect area = {
            x * ICON_SIZE,
            12 * ICON_SIZE,
            ICON_SIZE,
            ICON_SIZE};
        SDL_RenderCopy(renderer, lettres[x - LARGEUR], NULL, &area);
    }

    for (int col = LARGEUR + 2; col <= LARGEUR + 9; col += 7)
    {
        for (int y = 3; y < 10; y++)
        {
            SDL_Rect area = {
                col * ICON_SIZE,
                (22 - y) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};
            SDL_RenderCopy(renderer, textures[MUR], NULL, &area);
        }
    }

    for (int row = 3; row <= 9; row += 6)
    {
        for (int x = LARGEUR + 2; x < LARGEUR + 10; x++)
        {
            SDL_Rect area = {
                x * ICON_SIZE,
                (22 - row) * ICON_SIZE,
                ICON_SIZE,
                ICON_SIZE};
            SDL_RenderCopy(renderer, textures[MUR], NULL, &area);
        }
    }

    if (piece.taille == 2)
    {
        for (int row = 5; row < 7; row++)
        {
            for (int col = LARGEUR + 5; col < LARGEUR + 8; col++)
            {
                SDL_Rect area = {
                    col * ICON_SIZE,
                    (22 - row) * ICON_SIZE,
                    ICON_SIZE,
                    ICON_SIZE};

                for (int obj = NOIR; obj <= JAUNE; obj++)
                {
                    if ((piece.positions[0][col - LARGEUR - 5][row - 5] == symbols[obj]) && (piece.positions[0][col - LARGEUR - 5][row - 5] != 'N'))
                    {
                        SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                    }
                }
            }
        }
    }

    if (piece.taille == 3)
    {
        for (int row = 5; row < 8; row++)
        {
            for (int col = LARGEUR + 5; col < LARGEUR + 8; col++)
            {
                SDL_Rect area = {
                    col * ICON_SIZE,
                    (22 - row) * ICON_SIZE,
                    ICON_SIZE,
                    ICON_SIZE};

                for (int obj = NOIR; obj <= JAUNE; obj++)
                {
                    if ((piece.positions[0][col - LARGEUR - 5][row - 5] == symbols[obj]) && (piece.positions[0][col - LARGEUR - 5][row - 5] != 'N'))
                    {
                        SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                    }
                }
            }
        }
    }

    if (piece.taille == 4)
    {
        for (int row = 5; row < 8; row++)
        {
            for (int col = LARGEUR + 4; col < LARGEUR + 8; col++)
            {
                SDL_Rect area = {
                    col * ICON_SIZE,
                    (23 - row) * ICON_SIZE,
                    ICON_SIZE,
                    ICON_SIZE};

                for (int obj = NOIR; obj <= JAUNE; obj++)
                {
                    if ((piece.positions[1][col - LARGEUR - 4][row - 5] == symbols[obj]) && (piece.positions[1][col - LARGEUR - 4][row - 5] != 'N'))
                    {
                        SDL_RenderCopy(renderer, textures[obj], NULL, &area);
                    }
                }
            }
        }
    }
}

void GUI_spawn_ombre(char couleur)
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

void GUI_show()
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

    GUI_spawn_next(next);
    GUI_spawn_hold(hold);
    GUI_spawn_ombre(couleur);

    SDL_RenderPresent(renderer);
}

int GUI_get_key()
{
    SDL_Event event;

    do
    {
        SDL_WaitEvent(&event);

        if (event.type == SDL_QUIT)
        {
            GUI_close();
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
            GUI_close();
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

void GUI_wait_click()
{
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
    SDL_Event event;

    do
    {
        SDL_WaitEvent(&event);
    } while (event.type != SDL_MOUSEBUTTONDOWN);

    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
}

void GUI_pause(int time)
{
    SDL_Delay((Uint32)time);
}
