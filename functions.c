#include "functions.h"
#include <stdlib.h>
void affiche_prompt(){
  struct utsname name;
  int i = uname(&name);
  if(i<0){
    printf(" EFAULT buf is not valid.");
    exit(i);
  }
  printf("%s :",name.sysname);
  
  
}

void execute_ligne_commande(){
  int fl = 0;
  int nb = 0;
  char *** lc = ligne_commande(&fl,&nb);
  int actualcommand = 0;
  lc=lc;
  for(;actualcommand<nb;actualcommand++){
    if(fork()==0){
      execvp(lc[actualcommand][0],lc[actualcommand]);
    }
  }
}
