#include "functions.h"
#include <stdlib.h>
void affiche_prompt(){
  struct utsname name;
  int i = uname(&name);
  if(i<0){
    printf(" EFAULT buf is not valid.");
    exit(i);
  }
  int len=40;
  int path_len=1000;
  char *cpuname=(char *)malloc(sizeof(char)*len);
  char *username = getenv("USER");
  if(username == NULL){
    fprintf(stderr,"Erreur: Impossible de récuperer l'utilisateur");
    exit(-1);
  }
  char *path=(char *)malloc(sizeof(char)*path_len);
  if(getcwd(path,path_len)==NULL){
    perror("path");
    exit(1);
  } 
  if(gethostname(cpuname, len)<0){
    perror("hostname");
    exit(1);
  }
  printf("%s@%s:%s$ ",username,cpuname,path);
  //printf("iutsh$");
  fflush(stdout);
  execute_ligne_commande();
  free(cpuname);
  free(path);
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
    }else{
      if(fl == 0){
	wait(NULL);
      }
    }
  }
}
