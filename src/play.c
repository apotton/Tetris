#include "gui.h"
#include "tetris.h"

int main(void)
{
    // Initialisations
    init();
    gui_init();
    nouvelle_piece();

    bool termine = false;

    while (!termine)
    {
        actualiser_ombre();

        show();

        int key = get_key();

        switch (key)
        {
        case SDLK_RIGHT:
            deplacer(DROITE);
            break;

        case SDLK_LEFT:

            deplacer(GAUCHE);
            break;

        case SDLK_h:
            while (deplacement_possible(DROITE))
            {
                deplacer(DROITE);
            }
            break;

        case SDLK_s:
            while (deplacement_possible(GAUCHE))
            {
                deplacer(GAUCHE);
            }
            break;

        case SDLK_g:
            tourner(HORAIRE);
            break;

        case SDLK_d:
            tourner(ANTIHORAIRE);
            break;

        case SDLK_q:
            termine = true;
            break;

        case SDLK_r:
            swap_hold();
            break;

        case SDLK_f:
            tomber(actif);
            break;

        default:
            if (descente_possible(actif))
            {
                descendre(actif);
            }
            else
            {
                // Il n'est pas possible de descendre
                eliminer_lignes();
                if (spawn_possible(*pieces[actuelle_prochaine[0]]))
                {
                    nouvelle_piece();
                }
                else
                {
                    // Si on ne peut pas faire apparaître une pièce, on a perdu
                    printf(RED "PERDU HAHAHAAAAAA\n" DEFAUT);
                    termine = true;
                }
            }
        }
    }

    gui_close();

    printf("Score final: %d\n", score);

    return 0;
}