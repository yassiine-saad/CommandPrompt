/*
Fichier main.c : c'est le cœur principal du minishell. Il contient les appels des fonctions qui sont déclarées dans les autres fichiers.
Auteur : SAAD Yassine
Dépendances : parser.h cmd.h builin.h autre.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "parser.h"
#include "cmd.h"
#include "builtin.h"
#include "autre.c"


// #define _BUILTIN_H



int main(int argc, char* argv[]) {

    // Sauvegarde des descripteurs de fichier actuels
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);

  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t* current;


  while (1) {
    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);


    printPrompt();
    
    // Lire une ligne dans cmdline
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) break;
    cmdline[strcspn(cmdline, "\n")] = '\0';
    // cmdline[strlen(cmdline)-1]='\0';
    
     if (strlen(cmdline) == 0) {
        continue;
     }


    // Traiter la ligne de commande

    if(trim(cmdline) !=0){ //   - supprimer les espaces en début et en fin de ligne
        continue;
    }

    if(ajouter_Espaces_Operateurs(cmdline,MAX_LINE_SIZE) !=0){ //   - ajouter d'éventuels espaces autour de ; ! || && & ...
        continue;
    }

    if (clean(cmdline) != 0){//   - supprimer les doublons d'espaces
        continue;
    }


    if(substenv(cmdline,MAX_LINE_SIZE) !=0){ //   - traiter les variables d'environnement
        continue;
    }

    if(strcut(cmdline,' ',cmdtoks,MAX_CMD_SIZE) !=0){// Découper la ligne dans cmdtoks
        continue;
    }

    if (parse_cmd(cmdtoks,cmds,MAX_CMD_SIZE) !=0){ // Traduire la ligne en structures cmd_t dans cmds
        continue;
    }

   


    // printf("Commande après traitement : %s \n",cmdline);
    // printf("Commande après traitement :\n");
    //   for (int i = 0; cmdtoks[i] != NULL; i++) {
    //     printf("[%d] \"%s\"\n", i, cmdtoks[i]);
    //   }
    // print_cmds(cmds);

  
    for (current=cmds; current!=NULL; ) {
        execute_sequence(current);
        current=current->next;
    }

    free_memory(cmdline,cmdtoks,cmds);
  }
  
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}













