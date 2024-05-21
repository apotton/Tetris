# Flags de compilation
OPTI = -O3 -flto
CC = gcc $(OPTI)
CFLAGS = -g -std=c99 -Wall -I./include
LDFLAGS = -lSDL2
SANITIZE = -fsanitize=undefined,address,bounds-strict,signed-integer-overflow,float-divide-by-zero,leak,integer-divide-by-zero,null,bool,enum -Wconversion -Wall -pedantic


# Supprime les fichiers objet et les exécutables
clean:
	- rm -f *.o
	- find . -maxdepth 1 -executable -type f \( ! -name "Makefile" \) -delete


# Règles de compilation

%.o: ./src/%.c
	$(CC) $(CFLAGS) -o $@ -c $^


# Différents programmes

play: play.o tetris.o pieces.o gui.o
	$(CC) $(CFLAGS) -o play play.o tetris.o pieces.o gui.o $(LDFLAGS)


ia: ia.o tetris.o pieces.o gui.o
	$(CC) $(CFLAGS) -o ia ia.o tetris.o pieces.o gui.o $(LDFLAGS)