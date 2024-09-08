# README - Fonctions d'Exécution des Commandes Externes et Parsing - cmd.c

## Auteur
SAAD Yassine

## Description du Fichier
Le fichier `cmd.c` contient les fonctions relatives à l'exécution des commandes externes, à l'initialisation des structures `cmd_t`, et au parsing de ces structures.

### Fonctions d'Exécution des Commandes Externes

1. `int exec_cmd(cmd_t *p)`
   - Exécute une commande externe.
   - Paramètre : `p` - Pointeur vers une structure `cmd_t` représentant la commande.
   - Retourne : Un booléen indiquant le succès de l'exécution.

2. `int execute_sequence(cmd_t *cmd)`
   - Exécute une séquence de commandes, gérant les commandes internes et externes.
   - Paramètre : `cmd` - Pointeur vers une structure `cmd_t` représentant la première commande de la séquence.
   - Retourne : Un booléen indiquant le succès de l'exécution de la séquence.

### Fonctions d'Initialisation

3. `int init_cmd(cmd_t* p)`
   - Initialise une structure `cmd_t` avec des valeurs par défaut.
   - Paramètre : `p` - Pointeur vers une structure `cmd_t`.
   - Retourne : Un entier indiquant le succès de l'initialisation.

### Fonctions de Parsing

4. `int parse_cmd(char* tokens[], cmd_t* cmds, size_t max)`
   - Analyse les tokens de la ligne de commande et génère des structures `cmd_t`.
   - Paramètres : `tokens` - Tableau de chaînes de caractères représentant les tokens de la ligne de commande.
                 `cmds` - Tableau de structures `cmd_t` pour stocker les commandes.
                 `max` - La taille maximale du tableau `cmds`.
   - Retourne : Un entier indiquant le succès du parsing.

