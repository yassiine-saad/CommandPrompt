/*
Fichier autre.c : contient quelques fonctions de base pour éviter de charger le fichier main.c
Auteur : SAAD Yassine
Dépendances : stdio.h stdlib.h
*/


#include <stdio.h>
#include <stdlib.h>

// Couleurs pour le style Kali Linux
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"


/*
 * Fonction printPrompt : affiche le prompt du shell.
 */
void printPrompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(GREEN_COLOR "[%s]" RESET_COLOR BLUE_COLOR " $ " RESET_COLOR, cwd);
    } else {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

/*
 * Fonction free_memory : libère la mémoire allouée pour les tokens de commande.
 * Paramètres :
 *   - cmdline : chaîne de caractères représentant la ligne de commande.
 *   - cmdtoks : tableau de chaînes représentant les tokens.
 *   - cmds : tableau de structures cmd_t représentant les commandes.
 * Cette fonction parcourt le tableau cmdtoks, libère la mémoire allouée pour chaque
 * élément, puis initialise les tableaux cmdline et cmds à zéro.
 */
void free_memory(char *cmdline,char *cmdtoks[], cmd_t *cmds) {
    for (int i = 0; cmdtoks[i] != NULL; i++) {
        free(cmdtoks[i]);
    }
    memset(cmdline, 0, MAX_LINE_SIZE);
    memset(cmds, 0, sizeof(cmd_t) * MAX_CMD_SIZE);
}






/*
 * Fonction print_cmds : affiche les informations sur les commandes stockées dans un tableau de structures cmd_t.
 * Paramètre cmds : tableau de structures cmd_t.
 */
void print_cmds(cmd_t cmds[]) {
    int i = 0;
    while (cmds[i].path != NULL) {
        printf("Commande %d:\n", i + 1);
        printf("  Chemin : %s\n", cmds[i].path);
        printf("  Arguments : ");
        int j = 0;
        while (cmds[i].argv[j] != NULL) {
            printf("%s ", cmds[i].argv[j]);
            j++;
        }
        printf("\n");
        printf("  Redirection stdin : %d\n", cmds[i].stdin);
        printf("  Redirection stdout : %d\n", cmds[i].stdout);
        printf("  Redirection stderr : %d\n", cmds[i].stderr);
        printf("  Attendre : %d\n", cmds[i].wait);
        printf("  Commande suivante : %p\n", (void *)cmds[i].next);
        printf("  Commande suivante en cas de succès : %p\n", (void *)cmds[i].next_success);
        printf("  Commande suivante en cas d'échec : %p\n", (void *)cmds[i].next_failure);
        printf("\n");

        i++;
    }
}

