#ifndef LIST_H
#define LIST_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>

typedef void* TElement;

typedef struct Cellule{
	TElement donnee;
	struct Cellule *suivant;
}*liste;

typedef struct {
	float x;
	float y;
}coordinates;

typedef void (*print_element)(void* element);
typedef int (*compare_element)(void* element1,void* element2);

/*(1)premitive*/
TElement valCellule(liste l);

liste suivCellule(liste l);

/*initialise une liste*/
liste initListe();

/*teste si une liste est vide ou non*/
int videListe(liste l);

/*comparaison des element*/
int compare_int(TElement element1, TElement element2);
int compare_coordinates(TElement element1, TElement element2);

/*afficher les valeurs des differentes cellules de la liste*/
void affInt(TElement elem);
void affcoordinates(TElement elem);
void affListe(liste l, print_element aff_function);

/*determine la longueur d'une liste*/
int longListe(liste l);

/* Determiner l'adresse de cellule précedente par rapport a une position donnée */
liste celPre(int pos , liste l);

/*ajoute d'une valeur en tete de la liste*/
void inserTete(TElement X,liste *l,int taille);

/*retourne l'adresse de la derniere cellule*/
liste dernierCel(liste l);

/*retourne la valeur de la derniere cellule */
TElement dernierVal(liste l);

/*ajoute d'une valeur en queue de la liste*/
void inserQueue(TElement X,liste *l,int taille);

/*insertion dans une liste d'une valeur a une position donnée*/
void inserPos(int pos,TElement X,liste *l,int taille);

/*supprime la premiere cellule*/
void suppTete(liste *l);

/*supprime la liste*/
void suppListe(liste *l);

/*suppression d'une cellule par sa position*/
void suppPos(int pos,liste *l);

/*verifier l'existence d'une valeur par raport a une liste*/
int existVal(TElement X,liste l,compare_element cmpr_function);

/*retourne l'adresse de la premier occurrence d'une valeur par raport a une liste donnée*/
liste adresseVal(TElement X,liste l,compare_element cmpr_function);

/*Concatenation avec destruction des listes initiale*/
void concatListe(liste *l1,liste *l2);

#endif