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


}
