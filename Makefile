CFLAGS = -g -W -Wall -Werror
CC = gcc

iutsh: main.o functions.o ligne_commande.o
	$(CC) $(CFLAGS) -o iutsh $^
