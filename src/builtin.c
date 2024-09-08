
/*
Fichier builtin.c : ce fichier contient les fonctions de vérification, de traitement et d'exécution de commandes intégrées de ce minishell
Auteur : SAAD Yassine
Dépendances : cmd.h builtin.h
*/


#include "cmd.h"
#include "builtin.h"

#define _BUILTIN_H


#define MAX_BUILTIN_COMMANDS 4

/*
 * Tableau des commandes built-in.
 */
char* builtin_commands[MAX_BUILTIN_COMMANDS] = {"cd", "export", "unset","exit"};



/*
 * Fonction is_builtin : vérifie si une commande est un built-in.
 * Paramètre cmd : pointeur vers la commande à vérifier.
 * Retourne 1 si la commande est un built-in, 0 sinon.
 */
int is_builtin(char* cmd) {
    for (int i = 0; i < MAX_BUILTIN_COMMANDS; i++) {
        if (strcmp(cmd, builtin_commands[i]) == 0) {
            return 1;  // La commande est un built-in
        }
    }
    return 0;  // La commande n'est pas un built-in
}



/*
 * Fonction builtin : exécute une commande built-in.
 * Paramètre cmd : pointeur vers la structure cmd_t représentant la commande.
 * Retourne 0 en cas de succès.
 */
int builtin(cmd_t* cmd) {
    if (cmd == NULL || cmd->path == NULL) {
        // Vérifier si la structure cmd_t est valide et si la commande est définie
        return -1;
    }

    if (strcmp(cmd->path, "cd") == 0) {
        return cd(cmd->argv[1], STDERR_FILENO);
    } else if (strcmp(cmd->path, "export") == 0) {
        if (cmd->argv[1] != NULL) {
            return export(cmd->argv[1], STDERR_FILENO);
        } else {
            fprintf(stderr, "export: Utilisation incorrecte. Syntaxe : export VAR=\"value\"\n");
            return 1;
        }
    } else if (strcmp(cmd->path, "exit") == 0) {
        if (cmd->argv[1] != NULL) {
            int ret = atoi(cmd->argv[1]);
            exit_shell(ret, STDERR_FILENO);
        } else {
            exit_shell(0, STDERR_FILENO);
        }
    } else if (strcmp(cmd->path, "unset") == 0) {
        if (cmd->argv[1] != NULL) {
           return unset_variable(cmd->argv[1]);
        } else {
           return 1;
        }
    }

    return 0; 
}



/*
 * Fonction cd : change le répertoire courant.
 * Paramètre path : pointeur vers le chemin du répertoire à atteindre.
 * Paramètre fderr : descripteur de fichier pour les messages d'erreur.
 * Retourne 0 en cas de succès.
 */
int cd(const char* path, int fderr) {
    // Si le chemin commence par ~
    if (path[0] == '~') {
        // Obtenir le chemin du répertoire personnel
        const char* home = getenv("HOME");
        if (home == NULL) {
            dprintf(fderr, "cd: Impossible d'obtenir le répertoire personnel\n");
            return 1;
        }

        // Concaténer le répertoire personnel avec le reste du chemin
        char expanded_path[4096]; 
        strcpy(expanded_path, home);
        strcat(expanded_path, path + 1);

  
        if (chdir(expanded_path) != 0) {
            dprintf(fderr, "cd: %s: Aucun fichier ou dossier de ce type\n", expanded_path);
            return 1;
        }
    } else {
        if (chdir(path) != 0) {
            dprintf(fderr, "cd: %s: Aucun fichier ou dossier de ce type\n", path);
            return 1;
        }
    }

    return 0;
}


/*
 * Fonction export : ajoute ou modifie une variable d'environnement.
 * Paramètre arg : pointeur vers l'argument de la commande export (VAR="value").
 * Paramètre fderr : descripteur de fichier pour les messages d'erreur.
 * Retourne 0 en cas de succès.
 */
int export(char* arg, int fderr) {
    // Check if the argument is in the form VAR="value"
    const char* equal_sign = strchr(arg, '=');
    if (equal_sign == NULL) {
        dprintf(fderr, "export: Utilisation incorrecte. Syntaxe : export VAR=\"value\"\n");
        return 1;
    }

    // Extract the variable name and value
    char var[equal_sign - arg + 1];
    char value[strlen(equal_sign)];
    sscanf(arg, "%[^=]=%[^\n]", var, value);

    // Set the environment variable
    if (setenv(var, value, 1) != 0) {
        // En cas d'échec de la commande export
        dprintf(fderr, "export: Échec de l'exportation de la variable %s\n", var);
        return 1;  // Retourne une valeur non nulle pour indiquer une erreur
    }
    return 0;
}



/*
 * Fonction unset_variable : supprime une variable d'environnement.
 * Paramètre variable : pointeur vers le nom de la variable à supprimer.
 * Retourne 0 en cas de succès.
 */
int unset_variable(char *variable) {
    char *env_variable = getenv(variable);

    // Si la variable d'environnement n'existe pas
    if (env_variable == NULL) {
        printf("La variable d'environnement %s n'existe pas.\n", variable);
        return 0;
    }

    // Créer une chaîne de la forme "variable="
    char *unset_string = malloc(strlen(variable) + 2);
    sprintf(unset_string, "%s=", variable);

    // Réaffecter la variable d'environnement sans valeur
    if (putenv(unset_string) == 0) {
        printf("La variable d'environnement %s a été unset.\n", variable);
    } else {
        perror("Erreur lors du unset de la variable d'environnement");
        free(unset_string);
        return 1;
    }

    free(unset_string);
    return 0;
}



/*
 * Fonction exit_shell : termine le shell avec le code de retour spécifié.
 * Paramètre ret : code de retour.
 * Paramètre fderr : descripteur de fichier pour les messages d'erreur.
 */
void exit_shell(int ret, int fderr) {
    // La fonction exit_shell ne retourne pas de valeur, elle termine directement le programme
    dprintf(fderr, "Sortie du shell avec le code de retour %d\n", ret);
    exit(ret);
}