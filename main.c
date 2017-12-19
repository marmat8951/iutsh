#include "main.h"
#include "functions.h"

int main(int argc, char **argv){
  signal(SIGCHLD, SIG_IGN);
  argc = argc;
  argv = argv;
  do{
    affiche_prompt();
  }while(1);
  return 0;
}
