/*
Fichier parser.h : contient les fonctions de nettoyage et de traitement de la ligne de commande du début jusqu'au découpage en tokens
Auteur : SKALLI OUSSAMA
Dépendances : string.h
*/


#include <string.h>

#ifndef _PARSER_H
#define _PARSER_H

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256

/*
  Voir le TP sur les chaînes de caractères.
  Commentaires à compléter.
*/

int trim(char* str);
int clean(char* str);
int separate_s(char* str, char* s, size_t max);
int substenv(char* str, size_t max);

int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
// int strcut_s(char* str, char sep, char** tokens, size_t max);

// costum

int ajouter_Espaces_Operateurs(char* input,size_t max);
int substitutionMot(char *chaine, char *motAChercher,char *nouveauMot,size_t max);
int replaceSubstring(char *original,char *substring,char *replacement,size_t max);
int addSpaceAroundSubstring(char *str,char *substring,size_t max);
#endif
