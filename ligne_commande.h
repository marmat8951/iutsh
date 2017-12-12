#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
char *** ligne_commande (int * flag, int *nb);
void affiche (char ***t);
void libere (char ***t);
