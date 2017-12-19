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
  int old = 0;
  old=old;
  int in = dup(0);
  int out= dup(1);
  in=in;
  out=out;
  for(;actualcommand<nb;actualcommand++){
    int pipetable[2];
    if(pipe(pipetable)<0){
      fprintf(stderr,"Erreur lors de la création du pipe");
    }
    int process=fork();
    if(process==0){        // dans le fils
      if(actualcommand==nb-1){ // Si on est sur la dernière commande
	close(pipetable[1]);
	
      }else{                   // Sinon
	close(1);
	dup(pipetable[1]);
      }

      
      if(actualcommand==0){    // Si on est sur la premiere commande
	close(pipetable[0]);
      }else{                   // Sinon
	close(0);
	dup(pipetable[0]);
      }
      
      if(execvp(lc[actualcommand][0],lc[actualcommand])==-1){	// Execute et verifie si la commande a réussi
	perror(lc[actualcommand][0]);
	exit(-1);
      }
      
    }else{         // dans le pere
      close(pipetable[0]);
      close(pipetable[1]);
      if(fl == 0){
	int status=0;
	waitpid(process,&status,0);
      }
      
    }
  
   
  }

  if(lc!=NULL){
  libere(lc);
  }
}
