/*
Fichier cmd.c : ce fichier contient les fonctions d'exécution des commandes externes, d'initialisation et de parsing des cmd_t
Auteur : SAAD Yassine
Dépendances : cmd.h
*/


#include "cmd.h"
// #define _PROCESSUS_H

/*
 * Fonction exec_cmd : exécute une commande externe.
 * Paramètre p : pointeur vers la structure cmd_t représentant la commande.
 * Retourne 1 si l'exécution réussit, 0 sinon.
 */
int exec_cmd(cmd_t *p) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Code du processus fils
        // Redirections des descripteurs de fichiers
        if (p->stdin != STDIN_FILENO) {
            dup2(p->stdin, STDIN_FILENO);
            close(p->stdin);
        }

        if (p->stdout != STDOUT_FILENO) {
            dup2(p->stdout, STDOUT_FILENO);
            close(p->stdout);
        }

        if (p->stderr != STDERR_FILENO) {
            dup2(p->stderr, STDERR_FILENO);
            close(p->stderr);
        }

        // Exécution de la commande
        execvp(p->path, p->argv);

        // Si execvp retourne, il y a eu une erreur
        perror("minishell");
        exit(EXIT_FAILURE);
    } else {
        // Code du processus parent
        // Fermer les descripteurs de fichiers non utilisés dans le parent
        if (p->stdin != 0) {
            close(p->stdin);
        }

        if (p->stdout != 1) {
            close(p->stdout);
        }

        if (p->stderr != 2) {
            close(p->stderr);
        }

        if (p->wait) {
            // Attendre la fin du processus fils en avant-plan
            int status;
            waitpid(pid, &status, 0);
            p->status = status;
        } else {
            // Ne pas attendre le processus fils en arrière-plan
            p->pid = pid;
        }
    }

     return WIFEXITED(p->status) && (WEXITSTATUS(p->status) == 0);
}



/*
 * Fonction execute_sequence : exécute une séquence de commandes.
 * Paramètre cmd : pointeur vers la première commande de la séquence.
 * Retourne 1 en cas de succès, 0 sinon.
 */

int execute_sequence(cmd_t *cmd) {
    if (cmd == NULL) {
        return 1; // Fin de la séquence
    }

    int result;
    if (is_builtin(cmd->path)) {
        // C'est une commande intégrée, appeler la fonction builtin
        result = builtin(cmd);
    } else {
        // Ce n'est pas une commande intégrée, exécuter la commande normalement
        result = exec_cmd(cmd);
    }

    if (result && cmd->next_success != NULL) {
        return execute_sequence(cmd->next_success); // Exécute la commande suivante en cas de succès
    } else if (!result && cmd->next_failure != NULL) {
        return execute_sequence(cmd->next_failure); // Exécute la commande suivante en cas d'échec
    }

    return result;
}


/*
 * Fonction init_cmd : initialise une structure cmd_t.
 * Paramètre p : pointeur vers la structure cmd_t à initialiser.
 * Retourne 0 en cas de succès, -1 en cas d'erreur.
 */
int init_cmd(cmd_t* p) {
    if (p == NULL) {
        // Vérification de la validité du pointeur
        return -1;
    }

    p->pid = -1;  // Valeur par défaut pour le PID
    p->status = 0;  // Valeur par défaut pour le statut
    p->stdin = STDIN_FILENO;  // Valeur par défaut pour stdin  STDIN_FILENO;
    p->stdout = STDOUT_FILENO;  // Valeur par défaut pour stdout  STDOUT_FILENO;
    p->stderr = STDERR_FILENO;  // Valeur par défaut pour stderr  STDERR_FILENO;
    p->wait = 1;  // Valeur par défaut pour le mode d'attente
    p->path = NULL;  // Pointeur NULL pour le chemin par défaut
    p->next = NULL;  // Pointeur NULL pour la commande suivante
    p->next_success = NULL;  // Pointeur NULL pour la commande suivante en cas de succès
    p->next_failure = NULL;  // Pointeur NULL pour la commande suivante en cas d'échec

    // Initialiser le tableau d'arguments à NULL
    for (int i = 0; i < MAX_CMD_SIZE; i++) {
        p->argv[i] = NULL;
        p->fdclose[i] = -1; // Initialiser le tableau de descripteurs à -1
    }

    return 0;  // Tout s'est bien passé
}

/*
 * Fonction parse_cmd : analyse une séquence de tokens pour créer des structures cmd_t.
 * Paramètre tokens : tableau de chaînes représentant les tokens.
 * Paramètre cmds : tableau de structures cmd_t pour stocker les commandes.
 * Paramètre max : taille maximale du tableau cmds.
 * Retourne 0 en cas de succès.
 */
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
    int i = 0;
    int cmdIndex = 0;
    int arg_index = 0;
    cmd_t *current_cmd = &cmds[cmdIndex];
    init_cmd(current_cmd);
    int pipe_fd[2];  // Descripteurs de fichier pour le pipe

    // Parcourir les "mots" (tokens) pour traduire en structures cmd_t
    while (tokens[i] != NULL) {
        if (strcmp(tokens[i], "|" ) == 0) {
            // Nouvelle commande
            current_cmd->argv[arg_index] = NULL;

            if(tokens[i+1] != NULL){

                // Créer le pipe
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                current_cmd->stdout = pipe_fd[1];

                cmdIndex++;
                if(cmdIndex < max){
                    current_cmd->next = &cmds[cmdIndex];
                    current_cmd = &cmds[cmdIndex];
                    init_cmd(current_cmd);
                    arg_index = 0;
                    current_cmd->stdin = pipe_fd[0];
                }else{
                    printf("espace insufusant");
                    exit_shell(1,STDERR_FILENO);
                }

            }

        } else if (strcmp(tokens[i], "<") == 0) {
            // Redirection de stdin
            if(tokens[i+1] != NULL){
                i++;
                current_cmd->stdin = open(tokens[i], O_RDONLY);
            }else{
                printf("minishell : absence de token apres %s",tokens[i]);
                exit_shell(1,STDERR_FILENO);
            }

        } else if (strcmp(tokens[i], ">") == 0) {
            // Redirection de stdout
            if(tokens[i+1] != NULL){
                    i++;
                    current_cmd->stdout = open(tokens[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);   
            }else{
                printf("minishell : absence de token apres %s",tokens[i]);
                exit_shell(1,STDERR_FILENO);
            }
            
        } else if (strcmp(tokens[i], "2GT") == 0) { //2GT => >>
            // Redirection de stdout en mode append
            if(tokens[i+1] != NULL){
                i++;
                current_cmd->stdout = open(tokens[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
            }else{
                printf("minishell : absence de token apres %s",tokens[i]);
                exit_shell(1,STDERR_FILENO);
            }

        } else if (strcmp(tokens[i], "EGT") == 0) { //EGT => 2> 
            // Redirection de stderr
            if(tokens[i+1] != NULL){
                    i++;
                    current_cmd->stderr = open(tokens[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            } else{
                printf("minishell : absence de token apres %s",tokens[i]);
                exit_shell(1,STDERR_FILENO);
            }

        } else if (strcmp(tokens[i], "TGT") == 0) {  // TGT => 2>>
            // Redirection de stderr en mode append
            if (tokens[i + 1] != NULL) {
                i++;
                current_cmd->stderr = open(tokens[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
            } else {
                printf("minishell : absence de token après %s", tokens[i]);
                exit_shell(1, STDERR_FILENO);
            }
        } else if (strcmp(tokens[i], "EO") == 0) {
            // Redirection de stderr vers stdout
            // close(current_cmd->stderr);  // Ferme le descripteur de fichier stderr existant
            dup2(current_cmd->stdout, STDERR_FILENO);  // Redirige stderr vers stdout
        } else if (strcmp(tokens[i], "OE") == 0) {
            // Redirection de stdout vers stderr
            // close(current_cmd->stdout);  // Ferme le descripteur de fichier stdout existant
            dup2(current_cmd->stderr, STDOUT_FILENO);  // Redirige stdout vers stderr
        } else if (strcmp(tokens[i], ";") == 0) {
            // Fin de la commande actuelle, passer à la suivante
            current_cmd->argv[arg_index] = NULL;
            if(tokens[i+1] != NULL){
                cmdIndex++;
                current_cmd->next = &cmds[cmdIndex];
                current_cmd = current_cmd->next;
                init_cmd(current_cmd);
                arg_index = 0;
            }
        } else if (strcmp(tokens[i], "AND") == 0) { //AND => &&
            // Exécution conditionnelle (AND)
            current_cmd->argv[arg_index] = NULL;
            if(tokens[i+1] != NULL){
                cmdIndex++;
                current_cmd->next_success = &cmds[cmdIndex];
                current_cmd = current_cmd->next_success;
                init_cmd(current_cmd);
                arg_index = 0;
            }
        } else if (strcmp(tokens[i], "OR") == 0) { // OR => ||
            // Exécution conditionnelle (OR)
            current_cmd->argv[arg_index] = NULL;
            if(tokens[i+1] != NULL){
                cmdIndex++;
                current_cmd->next_failure = &cmds[cmdIndex];
                current_cmd = current_cmd->next_failure;
                init_cmd(current_cmd);
                arg_index = 0;
            }
        } else if (strcmp(tokens[i], "&") == 0) {
            current_cmd->wait = 0; // La commande doit être exécutée en arrière-plan
        } else {
            if (arg_index == 0) {
                // init_cmd_t(current_cmd);
                // JE DOIS SUPPRIMER LES " "  => grep "^d"
                current_cmd->path = strdup(tokens[i]);
            }
            current_cmd->argv[arg_index] = strdup(tokens[i]);
            arg_index++;
        }

        i++;
    }
    current_cmd->argv[arg_index] = NULL;
    current_cmd->next = NULL;
    current_cmd->next_success = NULL;
    current_cmd->next_failure = NULL;
    return 0;
}

