CFLAGS = -g -W -Wall -Werror
CC = gcc

iutsh: main.o functions.o ligne_commande_bouleau.o
	$(CC) $(CFLAGS) -o iutsh $^
