#include <list.h>

/*premitive*/

TElement valCellule(liste l){
	return l->donnee;
}

liste suivCellule(liste l){
	return l->suivant;
}

/*initialise une liste*/

liste initListe(){
	return NULL;
}

/*teste si une liste est vide ou non*/

int videListe(liste l){
	return l == NULL;
}

/*comparaison des element*/
int compare_int(TElement element1, TElement element2){
	int elem1 = *( int*)element1;
	int elem2 = *( int*)element2;

	return elem1 - elem2 == 0;
}

int compare_coordinates(TElement element1, TElement element2){
	coordinates elem1 = *( coordinates*)element1;
	coordinates elem2 = *( coordinates*)element2;
	
	return (elem1.x - elem2.x) + (elem1.y - elem2.y) == 0.000000000f;
}

/*afficher les valeurs des differentes cellules de la liste*/
void affInt(TElement elem){
	printf("%d ", *( int*)elem);
}

void affcoordinates(TElement elem){
	coordinates element = *( coordinates*)elem;
	printf("{%f, %f} ", element.x, element.y);
}

void affListe(liste l, print_element aff_function){
	
	while(!videListe(l)){
		aff_function(valCellule(l));
		l = suivCellule(l);
	}
	printf("\n");
}

/*determine la longueur d'une liste*/

int longListe(liste l){
	int lg = 0;
	
	while(!videListe(l)){
		lg = lg+1;
		l = suivCellule(l);
	}

	return lg;
}

/*Determiner l'adresse de cellule précedente par rapport a une position donnée */

liste celPre(int pos , liste l){

	while(!videListe(l) && pos > 0){
		l = suivCellule(l);
		pos--;
	}

	return l;
} 

/*ajoute d'une valeur en tete de la liste*/

void inserTete(TElement X,liste *l,int taile_TElement){

liste cel;

	//création de la cellule

	cel = (liste) malloc (sizeof(struct Cellule));
	cel->donnee= malloc (taile_TElement);

	//remplissage de la cellule

	memcpy(cel->donnee, X, taile_TElement);

	//chainage

	cel->suivant = *l;

	*l = cel ;

}

/*retourne l'adresse de la derniere cellule*/

liste dernierCel(liste l){
	
	while(!videListe(l->suivant)){
		l = suivCellule(l);
	}

	return l;
}

/*retourne la valeur de la derniere cellule */

TElement dernierVal(liste l){

	while(!videListe(l->suivant)){
		l = suivCellule(l);
	}

	return valCellule(l);
}

/*ajoute d'une valeur en queue de la liste*/

void inserQueue(TElement X,liste *l,int taile_TElement){

liste cel;

liste der;

	der = dernierCel(*l);

	//creation de la cellule

	cel = (liste) malloc (sizeof(struct Cellule));
	cel->donnee= malloc (taile_TElement);

	//remplissage de la cellule

	memcpy(cel->donnee, X, taile_TElement);

	//chainage

	cel->suivant = NULL;

	der->suivant = cel;

}

/*insertion dans une liste d'une valeur a une position donnée*/

void inserPos(int pos,TElement X,liste *l,int taile_TElement){
	liste celpre;

	if(pos<=longListe(*l)){
		celpre = celPre(pos-1,*l);
		inserTete(X,&(celpre->suivant),taile_TElement);
	}
	else{
		printf("Erreur\n");
	}

}
/*supprime la premiere cellule*/

void suppTete(liste *l){
	liste cel ;

	if(!videListe(*l)){

		//préservation de la premier cellule de la liste 
	
		cel = *l;

		//realisation du chainage

		*l = suivCellule(*l);

		//liberation de la memeoire de la premier cellule

		free(cel);
	}

}

/*supprime la liste*/

void suppListe(liste *l){

	if(!videListe(*l)){
		while(!videListe(*l)){
			suppTete(l);
		}
	}
}

/*suppression d'une cellule par sa position*/

void suppPos(int pos,liste *l){
	liste celpre;

	if(pos<=longListe(*l)){

		celpre = celPre(pos-1,*l);

		suppTete(&(celpre->suivant));
	}
	else{
		printf("Erreur\n");
	}

}
/*verifier l'existence d'une valeur par raport a une liste*/

int existVal(TElement X,liste l,compare_element cmpr_function){

	if(!videListe(l)){
		while( !videListe(l) && !cmpr_function(valCellule(l),X) ){
			l = suivCellule(l);
		}
	}

	return !videListe(l);
}

/*retourne l'adresse de la premier occurrence d'une valeur par raport a une liste donnée*/

liste adresseVal(TElement X,liste l,compare_element cmpr_function){
	if(existVal(X,l,cmpr_function)){
		while( !videListe(l) && !cmpr_function(valCellule(l),X) ){
			l = suivCellule(l);
		}
		return l;

	}
	else
		return NULL;
}

/*Concatenation avec destruction des listes initiale*/

void concatListe(liste *l1,liste *l2){
	liste der;

	if(videListe(*l1))
		*l1 = *l2;
	else{
		der = dernierCel(*l1);
		der->suivant = *l2;
	}
	*l2 = initListe();
}