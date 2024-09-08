# README - Fonctions de Commandes Intégrées - builtin.c

## Auteur
SAAD Yassine

## Description du Fichier
Le fichier `builtin.c` contient les fonctions de vérification, de traitement et d'exécution des commandes intégrées du Minishell.

### Fonctions de Vérification

1. `int is_builtin(const char* cmd)`
   - Vérifie si une commande donnée est une commande intégrée.
   - Paramètre : `cmd` - Chaîne de caractères représentant la commande à vérifier.
   - Retourne : Un entier (1 si la commande est intégrée, 0 sinon).

### Fonctions de Traitement et d'Exécution

2. `int builtin(cmd_t* cmd)`
   - Exécute une commande intégrée.
   - Paramètre : `cmd` - Pointeur vers une structure `cmd_t` représentant la commande intégrée.
   - Retourne : Un entier indiquant le succès de l'exécution.

3. `int cd(const char* path, int fderr)`
   - Change le répertoire courant.
   - Paramètres : `path` - Chemin vers le nouveau répertoire.
                  `fderr` - Descripteur de fichier pour l'erreur standard.
   - Retourne : Un entier indiquant le succès de l'opération.

4. `int export(const char* var, const char* value, int fderr)`
   - Exporte une variable d'environnement.
   - Paramètres : `arg` - Nom et valeur de la variable.
                  `fderr` - Descripteur de fichier pour l'erreur standard.
   - Retourne : Un entier indiquant le succès de l'exportation.

5. `int unset_variable(char *variable)`
   - Supprime une variable d'environnement.
   - Paramètre : `variable` - Nom de la variable à supprimer.
   - Retourne : Un entier indiquant le succès de la suppression.

6. `void exit_shell(int ret, int fderr)`
   - Termine le shell avec un code de retour.
   - Paramètres : `ret` - Code de retour.
                  `fderr` - Descripteur de fichier pour l'erreur standard.
   - Ne retourne rien, termine le programme.


