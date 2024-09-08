# README - Fonctions de Parsing - parser.h

## Auteur
Yassine SAAD

## Description du Fichier
Le fichier `parser.h` contient les fonctions de nettoyage, de traitement et de découpage de la ligne de commande du début jusqu'au découpage en des tokens.

### Fonctions de Nettoyage

1. `int trim(char* str)`
   - Supprime les espaces en début et fin de chaîne.
   - Paramètre : `str` - Chaîne de caractères à nettoyer.
   - Retourne : Un entier indiquant le succès de l'opération.

2. `int clean(char* str)`
   - Nettoie une chaîne de caractères en supprimant les espaces inutiles.
   - Paramètre : `str` - Chaîne de caractères à nettoyer.
   - Retourne : Un entier indiquant le succès de l'opération.

### Fonctions de Traitement

3. `int separate_s(char* str, char* s, size_t max)`
   - Sépare une chaîne de caractères en utilisant un délimiteur.
   - Paramètres : `str` - Chaîne de caractères à séparer.
                  `s` - Délimiteur de séparation.
                  `max` - Taille maximale du tableau résultant.
   - Retourne : Un entier indiquant le nombre de morceaux résultants.

4. `int substenv(char* str, size_t max)`
   - Remplace les variables d'environnement dans une chaîne.
   - Paramètres : `str` - Chaîne de caractères avec des variables d'environnement.
                  `max` - Taille maximale de la chaîne résultante.
   - Retourne : Un entier indiquant le succès de l'opération.

### Fonctions de Découpage

5. `int strcut(char* str, char sep, char** tokens, size_t max)`
   - Coupe une chaîne de caractères en utilisant un séparateur et stocke les morceaux dans un tableau.
   - Paramètres : `str` - Chaîne de caractères à découper.
                  `sep` - Séparateur.
                  `tokens` - Tableau pour stocker les morceaux résultants.
                  `max` - Taille maximale du tableau.
   - Retourne : Un entier indiquant le nombre de morceaux résultants.

6. `int ajouter_Espaces_Operateurs(char* input, size_t max)`
   - Ajoute des espaces autour des opérateurs dans une chaîne de caractères.
   - Paramètres : `input` - Chaîne de caractères à traiter.
                  `max` - Taille maximale de la chaîne résultante.
   - Retourne : Un entier indiquant le succès de l'opération.

7. `int substitutionMot(char *chaine, char *motAChercher, char *nouveauMot, size_t max)`
   - Remplace une occurrence spécifique dans une chaîne par un nouveau mot.
   - Paramètres : `chaine` - Chaîne de caractères à modifier.
                  `motAChercher` - Mot à rechercher dans la chaîne.
                  `nouveauMot` - Nouveau mot de remplacement.
                  `max` - Taille maximale de la chaîne résultante.
   - Retourne : Un entier indiquant le succès de l'opération.

8. `int replaceSubstring(char *original, char *substring, char *replacement, size_t max)`
   - Remplace une sous-chaîne spécifique dans une chaîne par une nouvelle sous-chaîne.
   - Paramètres : `original` - Chaîne de caractères originale.
                  `substring` - Sous-chaîne à remplacer.
                  `replacement` - Nouvelle sous-chaîne de remplacement.
                  `max` - Taille maximale de la chaîne résultante.
   - Retourne : Un entier indiquant le succès de l'opération.

9. `int addSpaceAroundSubstring(char *str, char *substring, size_t max)`
   - Ajoute des espaces autour d'une sous-chaîne spécifique dans une chaîne de caractères.
   - Paramètres : `str` - Chaîne de caractères à traiter.
                  `substring` - Sous-chaîne autour de laquelle ajouter des espaces.
                  `max` - Taille maximale de la chaîne résultante.
   - Retourne : Un entier indiquant le succès de l'opération.

