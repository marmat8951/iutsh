#include "functions.h"
#include <stdlib.h>
#include <string.h>

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
  int pipetable[nb][2];
  
  if(lc != NULL && strcmp(lc[actualcommand][0],"exit")==0){         // On sort si Exit
    libere(lc);
    exit(0);
  }
  
  for(;actualcommand<nb;actualcommand++){
    int process=fork();

    if(process==0){    //Fils
      

     
      if(nb==1){                      //1 seule commande
	close(pipetable[0][0]);
	close(pipetable[0][1]);
      }
      else if(actualcommand==0){      // Première commande
	 if(pipe(pipetable[actualcommand])<0){
            fprintf(stderr,"Erreur lors de la création du pipe");
	    exit(1);
         }
	 close(pipetable[0][0]);
	 close(1);
	 dup(pipetable[0][1]);
	 

      }else if(actualcommand==nb-1){  // Dernière commande
	int n=0;
	for(;n<nb;n++){
	  close(pipetable[n][1]);
	}
	//close(pipetable[actualcommand-1][1]);
	close(0);
	dup(pipetable[actualcommand-1][0]);
	close(pipetable[actualcommand-1][0]);
	close(1);
	dup(out);
	
      }else{                          // Commande au centre
	 if(pipe(pipetable[actualcommand])<0){
           fprintf(stderr,"Erreur lors de la création du pipe");
	   exit(1);
         }
	 close(pipetable[actualcommand-1][1]);
	 close(0);
	 dup(pipetable[actualcommand-1][0]);
	 close(1);
	 dup(pipetable[actualcommand][1]);
	 

      }
      
      if(execvp(lc[actualcommand][0],lc[actualcommand])==-1){	// Execute et verifie si la commande a réussi
	perror(lc[actualcommand][0]);
	exit(-1);
      }
      if(actualcommand>0){
	close(pipetable[actualcommand-1][0]);
      }
      close(pipetable[actualcommand][1]);
	   

    }                 // Pere
    else{
      int i;
      for(i=0;i<nb;i++){
      close(pipetable[i][0]);
      close(pipetable[i][1]);
      }
      if(fl == 0){
	int status=0;
	waitpid(process,&status,0);
      }
      
    }
    /*
    if(process==0){        // dans le fils
      if(actualcommand==nb-1){ // Si on est sur la dernière commande
	close(pipetable[actualcommand][1]); // ON ferme l'entrée du Pipe Liant la précédente à celle çi
	close(1);                          // On enleve la sortie actuelle
	dup(out);                          // On rétablit en sortie stdout
	
      }else{                   // Si on n'est pas dans la derniere commande
	close(1);              // On ferme stdin
	dup(pipetable[actualcommand-1][0]); // On copie en entrée standard la sortie du pipe 
      }

      
      if(actualcommand==0){    // Si on est sur la premiere commande
	close(pipetable[actualcommand][0]);    //On ferme la lecture du pipe
	close(1);                              //On ferme la sortie standard
	dup(pipetable[0][1]);                  //On utilise l'ecriture sur le pipe comme sortie   
	
      }else{                   // Sinon
	close(pipetable[actualcommand-1][1]);
	close(0);
	dup(pipetable[actualcommand-1][0]);
      }
      
      if(execvp(lc[actualcommand][0],lc[actualcommand])==-1){	// Execute et verifie si la commande a réussi
	perror(lc[actualcommand][0]);
	exit(-1);
      }
      
    }else{         // dans le pere
      close(pipetable[0][0]);
      close(pipetable[0][1]);
      if(fl == 0){
	int status=0;
	waitpid(process,&status,0);
      }
      
    }
    */
   
  }

  if(lc!=NULL){
  libere(lc);
  }
}
