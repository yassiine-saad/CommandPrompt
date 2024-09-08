/*
Fichier parser.h : contient les fonctions de nettoyage et de traitement de la ligne de commande du début jusqu'au découpage en tokens
Auteur : SKALLI OUSSAMA
Dépendances : stddef.h stdio.h stdlib.h parser.h
*/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#define _PARSER_H

/*
Fonction trim
Paramètre str : pointeur vers la chaîne de caractères à traiter
Retourne 0 en cas de succès.
*/
int trim(char *str) {
    // Supprimer les espaces au début
    int debut = 0;
    while (str[debut] == ' ' || str[debut] == '\t' || str[debut] == '\n') {
        debut++;
    }

    // Supprimer les espaces à la fin
    size_t longueur = strlen(str);
    int fin = longueur - 1;
    while (fin > debut && (str[fin] == ' ' || str[fin] == '\t' || str[fin] == '\n')) {
        fin--;
    }

    // Déplacer les caractères restants au début de la chaîne
    for (int i = debut; i <= fin; i++) {
        str[i - debut] = str[i];
    }

 
    str[fin - debut + 1] = '\0';

    return 0; 
}

/*
Fonction clean
Paramètre str : pointeur vers la chaîne de caractères à traiter
Retourne 0 en cas de succès.
*/
int clean(char *str) {
    int longueur = strlen(str);
    int estDansLesGuillemets = 0;  // Variable pour suivre si nous sommes à l'intérieur des guillemets

    for (int i = 0; i < longueur - 1; i++) {
        if (str[i] == '"') {
            estDansLesGuillemets = !estDansLesGuillemets;  // Inverser l'état si nous trouvons des guillemets
        }

        if (!estDansLesGuillemets && str[i] == ' ' && str[i + 1] == ' ') {
            // Décalage des caractères vers la gauche pour supprimer le double espace
            for (int j = i + 1; j < longueur; j++) {
                str[j] = str[j + 1];
            }
            i--;  // Décrémenter i pour vérifier à nouveau à cette position
        }
    }

    return 0; 
}

/*
Fonction separate_s
Paramètre str : pointeur vers la chaîne de caractères à traiter
Paramètre s : pointeur vers ...
Paramètre max : taille maximale
Retourne 0 en cas de succès.
*/
int separate_s(char* str, char* s, size_t max) {
//strlen(str) la longueur de la chaine de caractère
  size_t len = strlen(str);
//condition si la longueur du str est supérieure à la capacité maximale
    if (len >= max) {
        // La chaîne d'origine est déjà trop grande
        return -1;
    }
//allocation dynamique pour la chaine résultat
    char* resultat = (char*)malloc(max);
    if (resultat == NULL) {
        // Échec de l'allocation mémoire
        return -1;
    }
//la position de la chaine résultante
    size_t resultat_pos = 0;
//la position de la chaine d'origine
    size_t i = 0;
// boucle qui parcourt chaque caractère de str
    while (i < len && resultat_pos < max - 1) {
   // condition de vérification du caractère actuel
        if (strchr(s, str[i]) && i >= 0 && resultat[resultat_pos - 1] != ' ' && resultat_pos < max - 3) {
            // Ajout des espaces avant et après les séparateurs
            resultat[resultat_pos++] = ' ';
            resultat[resultat_pos++] = str[i];
            resultat[resultat_pos++] = ' ';
        } else {
        // le caractère de la chaine d'origine est ajouté à la chaine résultat
            resultat[resultat_pos++] = str[i];
        }
        i++;
    }
 //ajout de lu caractère nul à la fin de la chaine résultat
    resultat[resultat_pos] = '\0';

    // On Copie le résultat dans la chaîne d'origine
    strncpy(str, resultat, max);
    str[max - 1] = '\0';
//libération de la mémoire allouée dynamiquement
    free(resultat);

    return 0;
}


/*
Fonction substenv
Paramètre str : pointeur vers la chaîne de caractères à traiter
Paramètre max : taille maximale de str
Retourne 0 en cas de succès.
*/
int substenv(char* str, size_t max) {
    char* dollar_sign = str;

    while (*dollar_sign != '\0') {
        // Recherche du caractère '$'
        dollar_sign = strchr(dollar_sign, '$');

        if (dollar_sign == NULL) {
            break;  // Si '$' n'est pas trouvé, on quitte la boucle
        }

        // Si le caractère suivant est '{', alors on a une variable au format ${VAR}
        if (*(dollar_sign + 1) == '{') {
            char* closing_brace = strchr(dollar_sign, '}');

            if (closing_brace != NULL) {
                // Extraire le nom de la variable
                size_t var_len = closing_brace - (dollar_sign + 2);
                char variable[var_len + 1];
                strncpy(variable, dollar_sign + 2, var_len);
                variable[var_len] = '\0';

              

                char* value = getenv(variable);

                char resultat[var_len+3];  

                // Utiliser sprintf pour formater la chaîne
                sprintf(resultat, "${%s}", variable);

        
                if (value != NULL) {
                    if (substitutionMot(str,resultat,value,max) != 0){
                        printf("taille max dépassé");
                        return 1;
                    }
                }



                // Déplacer le pointeur après la variable pour continuer la recherche
                dollar_sign = closing_brace + 1;
            } else {
                // Si '}' n'est pas trouvé, ignorer le '$' et continuer la recherche
                dollar_sign++;
            }
        } else {
            // Si le caractère suivant n'est pas '{', alors on a une variable au format $VAR
            char* space_or_end = strpbrk(dollar_sign + 1, " \t\n\r$.");

            size_t var_len;
            if (space_or_end != NULL) {
                var_len = space_or_end - (dollar_sign + 1);
            } else {
                var_len = strlen(dollar_sign + 1);
            }

            char variable[var_len + 1];
            strncpy(variable, dollar_sign + 1, var_len);
            variable[var_len] = '\0';

            char* value = getenv(variable);

            char resultat[var_len+1]; 

            // Utiliser sprintf pour formater la chaîne
            sprintf(resultat, "$%s", variable);

        
            if (value != NULL) {
                if (substitutionMot(str,resultat,value,max) != 0){
                    printf("taille max dépassé");
                    return 1;
                }
            }

            // Déplacer le pointeur après la variable pour continuer la recherche
            dollar_sign += var_len + 1;
        }
    }

    return 0;
}



/*
Fonction strcut
Paramètre str : pointeur vers la chaîne de caractères à découper
Paramètre sep : séparateur
Paramètre tokens : tableau de pointeurs vers des chaînes pour stocker les tokens
Paramètre max : nombre maximal de tokens à extraire
Retourne 0 en cas de succès, 1 si la fin de la chaîne est atteinte.
*/
int strcut(char *str, char sep, char **tokens, size_t max) {
    char *token = str;
    int i = 0;

    // Parcourt la chaîne jusqu'à la fin ou jusqu'à atteindre le nombre maximal de tokens moins un
    while (*token != '\0' && i < max - 1) {
     
        // Ignore les espaces au début du token
        while (*token == ' ') {
            token++;
        }

        // Vérifie les guillemets simples ou doubles
        if (*token == '\'' || *token == '"') {
            char quote = *token;
            token++; // Avance après le guillemet

            // Alloue de l'espace pour le token entre guillemets
            tokens[i] = malloc(strlen(token) + 1);

            // Trouve le guillemet de fermeture
            char *closing_quote = strchr(token, quote);

            if (closing_quote != NULL) {
                // Copie le token entre guillemets dans le tableau
                strncpy(tokens[i], token, closing_quote - token);
                tokens[i][closing_quote - token] = '\0';
                token = closing_quote + 1; // Avance après le guillemet de fermeture
            } else {
                // Copie le reste du token si le guillemet de fermeture n'est pas trouvé
                strcpy(tokens[i], token);
                token += strlen(token);
            }
        } else {
            // Trouve le prochain espace, le séparateur spécifié ou la fin de la chaîne
            char *next_sep = strchr(token, sep);
            char *next_space = strchr(token, ' ');

            if (next_sep != NULL && (next_space == NULL || next_sep < next_space)) {
                // Alloue de l'espace pour le token jusqu'au prochain séparateur
                tokens[i] = malloc(next_sep - token + 1);
                strncpy(tokens[i], token, next_sep - token);
                tokens[i][next_sep - token] = '\0';
                token = next_sep + 1; // Avance après le séparateur
            } else if (next_space != NULL) {
                // Alloue de l'espace pour le token jusqu'au prochain espace
                tokens[i] = malloc(next_space - token + 1);
                strncpy(tokens[i], token, next_space - token);
                tokens[i][next_space - token] = '\0';
                token = next_space + 1; // Avance après l'espace
            } else {
                // Alloue de l'espace pour le reste du token
                tokens[i] = strdup(token);
                token += strlen(token);
            }
        }

        i++;
    }

    // Définit le dernier élément du tableau de tokens à NULL
    tokens[i] = NULL;

    // Retourne 0 si la fin de la chaîne est atteinte, sinon 1
    return (*token == '\0') ? 0 : 1;
}



// // Optionnel
// int strcut_s(char* str, char sep, char** tokens, size_t max) {

// }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
Fonction substitutionMot
Paramètre chaine : pointeur vers la chaîne de caractères à traiter
Paramètre motAChercher : pointeur vers le mot à rechercher
Paramètre nouveauMot : pointeur vers le nouveau mot à insérer
*/
int substitutionMot(char *chaine, char *motAChercher,char *nouveauMot,size_t max ) {
    char *position = chaine;
    int motAChercherLongueur = strlen(motAChercher);
    int nouveauMotLongueur = strlen(nouveauMot);

    if ((strlen(chaine) - motAChercherLongueur + nouveauMotLongueur) >= max){
        return 1;
    }

    while ((position = strstr(position, motAChercher)) != NULL) {
        memmove(position + nouveauMotLongueur, position + motAChercherLongueur, strlen(position + motAChercherLongueur) + 1);
        memcpy(position, nouveauMot, nouveauMotLongueur);
        position += nouveauMotLongueur;
    }

    return 0;
}



/*
Fonction replaceSubstring
Paramètre str : pointeur vers la chaîne de caractères à traiter
Paramètre oldSubstr : pointeur vers la sous-chaîne à remplacer
Paramètre newSubstr : pointeur vers la nouvelle sous-chaîne à insérer
*/
int replaceSubstring(char *str, char *oldSubstr, char *newSubstr,size_t max) {
    char result[max];
    int resultIndex = 0;

    if (( strlen(str) - strlen(oldSubstr) + strlen(newSubstr) ) >= max){
        return 1;
    }

    for (int i = 0; str[i] != '\0'; ++i) {
        if (strstr(str + i, oldSubstr) == str + i) {
            // Vérifier si la sous-chaîne est en dehors des guillemets et des apostrophes
            if ((i == 0 || (str[i - 1] != '"' && str[i - 1] != '\'')) && (str[i + strlen(oldSubstr)] == '\0' || (str[i + strlen(oldSubstr)] != '"' && str[i + strlen(oldSubstr)] != '\''))) {
                for (int j = 0; newSubstr[j] != '\0'; ++j) {
                    result[resultIndex++] = newSubstr[j];
                }
                i += strlen(oldSubstr) - 1;  // Pour sauter la sous-chaîne remplacée
            } else {
                result[resultIndex++] = str[i];
            }
        } else {
            result[resultIndex++] = str[i];
        }
    }

    result[resultIndex] = '\0';
    strcpy(str, result);

    return 0;
}






/*
Fonction addSpaceAroundSubstring
Paramètre str : pointeur vers la chaîne de caractères à traiter
Paramètre substring : pointeur vers la sous-chaîne autour de laquelle ajouter des espaces
Retourne 0 en cas de succès, 1 si la chaîne n'a pas été modifiée.
*/
int addSpaceAroundSubstring(char *str, char *substring,size_t max) {
    char result[max]; 
    int resultIndex = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (strstr(str + i, substring) == str + i) {
            // Vérifier si la sous-chaîne est en dehors des guillemets et des apostrophes
            if ((i == 0 || (str[i - 1] != '"' && str[i - 1] != '\'')) &&
                (str[i + strlen(substring)] == '\0' || (str[i + strlen(substring)] != '"' && str[i + strlen(substring)] != '\''))) {
                // Ajouter un espace avant la sous-chaîne
                if (i > 0 && str[i - 1] != ' ') {
                    result[resultIndex++] = ' ';
                }

                // Ajouter la sous-chaîne
                for (int j = 0; substring[j] != '\0'; ++j) {
                    result[resultIndex++] = substring[j];
                }

                // Ajouter un espace après la sous-chaîne
                if (str[i + strlen(substring)] != ' ' && str[i + strlen(substring)] != '\0') {
                    result[resultIndex++] = ' ';
                }

                i += strlen(substring) - 1;  // Pour sauter la sous-chaîne ajoutée
            } else {
                result[resultIndex++] = str[i];
            }
        } else {
            result[resultIndex++] = str[i];
        }
    }

    result[resultIndex] = '\0';
    strcpy(str, result);

  return 0;
}


/*
Fonction ajouter_Espaces_Operateurs
Paramètre input : pointeur vers la chaîne de caractères à traiter
Retourne 0 en cas de succès, 1 si la chaîne n'a pas été modifiée.
*/
int ajouter_Espaces_Operateurs(char* input,size_t max) {
    replaceSubstring(input,"2>&1","EO",max);
    replaceSubstring(input,">&2","OE",max);
    replaceSubstring(input,"2>>","TGT",max);
    replaceSubstring(input,">>>","3GT",max);
    replaceSubstring(input,"&&","AND",max);
    replaceSubstring(input,"||","OR",max);
    replaceSubstring(input,">>","2GT",max);
    replaceSubstring(input,"<<","2LT",max);
    replaceSubstring(input,"2>","EGT",max);
    // Liste des opérateurs à prendre en charge
    char* operateurs[] = {"AND","OR","2GT","3GT","2LT","EGT","TGT","EO","OE","&", "|", ">","<","!",";"};

    int taille = sizeof(operateurs) / sizeof(operateurs[0]);
    int status=0;

    for (int i = 0; i < taille; i++) {
       status = addSpaceAroundSubstring(input,operateurs[i],max);
    }

    return status;
}






