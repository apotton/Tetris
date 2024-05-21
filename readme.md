# Tetris 🟦⬜🟥🟨🟧🟪🟩

## Règle de compilation

- Pour jouer à Tetris, taper `make play` puis lancer `./play`
- Pour voir l'ordinateur jouer tout seul, taper `make ia` puis lancer `./ia`
- Pour supprimer tous les exécutables et les fichiers .o, taper `make clean`

## Jeu de Tetris

### Comment jouer

Les pièces ne tombent pas par elles-même, mais appuyer sur n'importe quelle touche les fera descendre d'une ligne. Pour les déplacer latéralement, il faut appuyer sur les flèches du clavier.

Pour les autres controles, il faut placer son majeur sur la touche F (donc l'index sur G et l'annulaire sur D). G fera une rotation horaire, D antihoraire, tandis que F fera tomber la pièce le plus bas possible (sur sa colonne). Une fois en bas, elle pourra toujours être déplacée latéralement si elle n'entre pas en contact avec des blocs. Un appui sur la flèche du bas la fixera et fera apparaître une nouvelle pièce.

Les touches S et H plaquent la pièce respectivement le plus à gauche et à droite possible. Enfin, la touche R (au dessus de F) échange la pièce actuelle avec la pièce en hold, et la touche Q quitte la partie.

### Fonctionnement du code

Les grandeurs utiles sont toutes déclarées dans le header `tetris.h`. On y trouve notamment définies en préprocesseur les grandeurs `LARGEUR` et `HAUTEUR`, dont on comprend facilement l'utilité. Pour que les modifications soient prises en compte, il faut tout recompiler après les avoir changées.

Le fonctionnement du jeu est défini dans le fichier `tetris.c`. Celui-ci contient toutes les fonctions et est utilisé par le jeu comme par l'ia. Il utilise des grandeurs définies avec le mot clef `extern` dans le header. Cela signifie que ces variables sont partagées en mémoire.

Les coordonnées de la pièce actuelle (celle qui peut être contrôlée) sont stockées dans le tableau `actif`, qui contient 4 sets de coordonnées (x,y). De la même manière, le tableau `ombre` contient des coordonnées de l'"ombre" de la pièce, aka sa position si elle venait à tomber. Le tout s'affiche sur un `plateau`, qui est une table de caractères contenant les couleurs et les positions des blocs.

## IA

### Comment s'en servir

Une fois compilée, l'IA a accès à l'état du plateau, la pièce actuelle ainsi que la pièce suivante. Elle génère ensuite toutes les positions possibles pour la première pièce, puis la seconde. Elle choisit enfin la meilleure des combinaisons possibles et joue le premier coup. Cela lui permet de prévoir du mieux qu'elle peut avec un coup d'avance, tout en ayant la capacité de s'adapter avec la pièce qu'elle ne connaît pas encore.

Pour la voir coup par coup, il suffit de décommenter autour de la ligne 306 du programme `ia.c`. Ainsi, l'ia ne jouera aucun coup avant d'avoir appuyé sur une touche. Comme pour le jeu, appuyer sur q quitte la partie.

### Fonctionnement du code

Avant d'effectuer une action, le programme stocke le plateau actuel dans un autre tableau temporaire, appelé `plateau_stocke` (pour la première et la deuxième pièce). Pour trouver toutes les positions possibles, il essaye chaque rotation et chaque déplacement horizontal et fait tomber la pièce.

Fondamentalement, le niveau de ce programme dépend de la fonction d'évaluation, `eval`. Dans mon implémentation, celle-ci attribue un score à une situation en fonction de trois paramètres: la hauteur, le nombre de trous et la "platitude" (différence entre la case la plus haute et la plus basse):

- **Hauteur**: une partie de Tetris se termine quand il y a des blocs trop élevés, qui empêchent l'apparition de nouvelles pièces. Il apparaît ainsi naturel de prendre en compte ce paramètre essentiel.
- **Nombre de trous**: un trou est un bloc vide qui contient au dessus de lui un bloc non vide (directement ou pas). Avec l'implémentation de l'IA, un trou ne pourra pas être bouché, même si il est accessible latéralement. La présence de trous empêchant la complétion de lignes, il s'agit également d'un paramètre à prendre en compte.
- **Platitude**: dans leur volonter de ne pas faire de trous, certains joueurs de Tetris (et l'ordinateur) se retrouvent avec des "puis": des colonnes entières vides mais entourées par des blocs. Ce genre de formation ne peut se régler qu'en présence d'une barre, mais celles-ci sont relativement rares et peuvent ne pas suffire. Il convient donc d'avoir un plateau le plus plat possible.

Pour le calcul de l'évaluation, j'élève chacun de ces paramètres au carré, afin qu'ils prennent plus d'importance si ils deviennent trop grands. Comme on souhaite pénaliser des valeurs importantes, il y a un signe - devant chacune d'entre elles. Enfin, il y a également des coefficients qui peuvent être ajustés afin de produire de meilleurs résultats.

## Conclusion

Il s'agit ici d'une IA extrêmement rudimentaire, avec une simple fonction d'évaluation heuristique. Elle aide néanmoins, par itérations successives, à comprendre les enjeux d'une partie Tetris. Contrairement à ce que l'on pourrait penser, le fait de construire sans trous ne mène pas nécessairement à une longue partie, la hauteur ayant une bien meilleure influence. Toute la difficulté du jeu vient de l'impossibilité de prévoir ses mouvements à plus de deux coups, donc d'établir des grandes stratégies comme pour les échecs. Cela a néanmoins le mérite de limiter le nombre de différentes positions à examiner.

Pour aller plus loin, il pourrait être possible de coder un programme qui évalue la performance d'une fonction d'évaluation (en faisant des moyennes pour s'affranchir des nombres aléatoires), en fonction des paramètres qui lui sont donnés. Par descente de gradient, on pourrait s'approcher d'une fonction optimale.