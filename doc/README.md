# mini_shell_upjv

## Description
Le minishell est un shell minimaliste implémenté en langage C. Il offre une interface en ligne de commande avec des fonctionnalités de base, telles que l'exécution de commandes internes et externes, la gestion des redirections.

## Fonctionnalités
- Exécution de commandes internes telles que `cd`, `export`, `unset`, `exit`.
- Exécution de commandes externes et internes avec gestion des redirections (stdin, stdout, stderr).
- Gestion des opérateurs de contrôle de flux tels que `&&`, `||`, `;`.
- Gestion des tubes (PIPE) '|'
- Gestion des variables d'environnement

## Auteur
- SAAD Yassine
- SKALLI Oussama

## Dépendances
- parser.h
- cmd.h
- builtin.h
- autre.c


## Structure du Projet


Le projet Minishell consiste en plusieurs fichiers qui travaillent ensemble pour créer un shell basique avec des fonctionnalités limitées. Voici une brève description de chaque fichier :

- **main.c**: Le fichier principal du Minishell. Il contient les appels aux fonctions définies dans les autres fichiers.

- **parser.h**: Fichier qui regroupe les fonctions de nettoyage et de traitement de la ligne de commande.
    - `int trim(char* str)`: Supprime les espaces au début et à la fin de la chaîne.
    - `int clean(char* str)`: Nettoie la chaîne en supprimant les espaces inutiles.
    - `int separate_s(char* str, char* s, size_t max)`: Sépare la chaîne en tokens en utilisant le délimiteur spécifié.
    - `int substenv(char* str, size_t max)`: Remplace les variables d'environnement dans la chaîne.

    - `int strcut(char* str, char sep, char** tokens, size_t max)`: Coupe la chaîne en tokens en utilisant le séparateur spécifié.
    - `int ajouter_Espaces_Operateurs(char* input, size_t max)`: Ajoute des espaces autour des opérateurs.
    - `int substitutionMot(char *chaine, char *motAChercher, char *nouveauMot, size_t max)`: Remplace une occurrence d'un mot dans une chaîne.
    - `int replaceSubstring(char *original, char *substring, char *replacement, size_t max)`: Remplace une sous-chaîne par une autre uniquement si elle n'est pas incluse dans des guillemets simples ('') ou doubles ("").
    - `int addSpaceAroundSubstring(char *str, char *substring, size_t max)`: Ajoute des espaces autour d'une sous-chaîne.

- **cmd.h**: Fichier qui définit la structure et les fonctions liées à la gestion des commandes.
    - `int exec_cmd(cmd_t *p)`: Exécute une commande externe.
    - `int execute_sequence(cmd_t *cmd)`: Exécute une séquence de commandes (récursivement).
    - `int init_cmd(cmd_t* p)`: Initialise une structure de commande.
    - `int parse_cmd(char* tokens[], cmd_t* cmds, size_t max)`: Analyse les tokens et crée une structure de commandes. (remplir cmds)

- **builtin.h**: Fichier contenant les fonctions de vérification, de traitement et d'exécution des commandes intégrées.
    - `int is_builtin(const char* cmd)`: Vérifie si la commande est une commande intégrée.
    - `int builtin(cmd_t* cmd)`: Exécute une commande intégrée (cd, export, unset, exit).
    - `int cd(const char* path, int fderr)`: Change le répertoire de travail courant.
    - `int export(const char* var, const char* value, int fderr)`: Exporte une variable d'environnement.
    - `int unset_variable(char *variable)`: Supprime une variable d'environnement.
    - `void exit_shell(int ret, int fderr)`: Termine le shell.

- **autre.c**: Fichier qui contient quelques fonctions de base pour éviter de charger le fichier main.c.
    - `void printPrompt()`: Affiche le prompt du shell.
    - `void free_memory(char *cmdtoks[])`: Libère la mémoire allouée pour les tokens, cmds, et la cmdline.
   

- **Makefile**: Fichier pour automatiser la compilation du projet.


