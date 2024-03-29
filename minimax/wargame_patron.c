#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define NB_LIGNES 10
#define NB_COLONNES 10
#define INFINI 10000

/*#define DEBUG*/

typedef struct pion_s
{
	int couleur;
	int valeur;
}Pion;

typedef struct
{
	int x;
	int y;
}Coordinates;

typedef struct {
	Coordinates from;
	Coordinates to;
}Move;


int depth;
Pion *plateauDeJeu;
int node;
int alphabeta;
Move move;

void f_affiche_plateau(Pion *plateau);
int f_convert_char2int(char c);
char f_convert_int2char(int i);
int negamax(Pion* jeu, int dep, int joueur, int alpha, int beta);
int f_max(Pion* jeu, int dep, int joueur, int alpha, int beta);
int f_min(Pion* jeu, int dep, int joueur, int alpha, int beta);

int f_convert_char2int(char c)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(c>='A' && c<='Z')
		return (int)(c-'A');
	if(c>='a' && c<='z')
		return (int)(c-'a');
	return -1;
}

char f_convert_int2char(int i)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	return (char)i+'A';
}

Pion *f_init_plateau(int amelioration)
{
	int i, j;
	Pion *plateau=NULL;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	plateau = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
	if(plateau == NULL)
	{
		printf("error: unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			plateau[i*NB_COLONNES+j].couleur = 0;
			plateau[i*NB_COLONNES+j].valeur = 0;
		}
	}

	plateau[9*NB_COLONNES+5].couleur = 1;
	plateau[9*NB_COLONNES+5].valeur = 1;

	plateau[9*NB_COLONNES+6].couleur = 1;
	plateau[9*NB_COLONNES+6].valeur = 2;

	plateau[9*NB_COLONNES+7].couleur = 1;
	plateau[9*NB_COLONNES+7].valeur = 3;

	plateau[9*NB_COLONNES+8].couleur = 1;
	plateau[9*NB_COLONNES+8].valeur = 2;

	plateau[9*NB_COLONNES+9].couleur = 1;
	plateau[9*NB_COLONNES+9].valeur = 1;

	plateau[8*NB_COLONNES+0].couleur = 1;
	plateau[8*NB_COLONNES+0].valeur = 1;

	plateau[8*NB_COLONNES+1].couleur = 1;
	plateau[8*NB_COLONNES+1].valeur = 3;

	plateau[8*NB_COLONNES+2].couleur = 1;
	plateau[8*NB_COLONNES+2].valeur = 3;

	plateau[8*NB_COLONNES+3].couleur = 1;
	plateau[8*NB_COLONNES+3].valeur = 1;

	plateau[8*NB_COLONNES+6].couleur = 1;
	plateau[8*NB_COLONNES+6].valeur = 1;

	plateau[8*NB_COLONNES+7].couleur = 1;
	plateau[8*NB_COLONNES+7].valeur = 1;

	plateau[8*NB_COLONNES+8].couleur = 1;
	plateau[8*NB_COLONNES+8].valeur = 1;

	plateau[7*NB_COLONNES+1].couleur = 1;
	plateau[7*NB_COLONNES+1].valeur = 1;

	plateau[7*NB_COLONNES+2].couleur = 1;
	plateau[7*NB_COLONNES+2].valeur = 1;

	plateau[2*NB_COLONNES+7].couleur = -1;
	plateau[2*NB_COLONNES+7].valeur = 1;

	plateau[2*NB_COLONNES+8].couleur = -1;
	plateau[2*NB_COLONNES+8].valeur = 1;

	plateau[1*NB_COLONNES+1].couleur = -1;
	plateau[1*NB_COLONNES+1].valeur = 1;

	plateau[1*NB_COLONNES+2].couleur = -1;
	plateau[1*NB_COLONNES+2].valeur = 1;

	plateau[1*NB_COLONNES+3].couleur = -1;
	plateau[1*NB_COLONNES+3].valeur = 1;

	plateau[1*NB_COLONNES+6].couleur = -1;
	plateau[1*NB_COLONNES+6].valeur = 1;

	plateau[1*NB_COLONNES+7].couleur = -1;
	plateau[1*NB_COLONNES+7].valeur = 3;

	plateau[1*NB_COLONNES+8].couleur = -1;
	plateau[1*NB_COLONNES+8].valeur = 3;

	plateau[1*NB_COLONNES+9].couleur = -1;
	plateau[1*NB_COLONNES+9].valeur = 1;

	plateau[0*NB_COLONNES+0].couleur = -1;
	plateau[0*NB_COLONNES+0].valeur = 1;

	plateau[0*NB_COLONNES+1].couleur = -1;
	plateau[0*NB_COLONNES+1].valeur = 2;

	plateau[0*NB_COLONNES+2].couleur = -1;
	plateau[0*NB_COLONNES+2].valeur = 3;

	plateau[0*NB_COLONNES+3].couleur = -1;
	plateau[0*NB_COLONNES+3].valeur = 2;

	plateau[0*NB_COLONNES+4].couleur = -1;
	plateau[0*NB_COLONNES+4].valeur = 1;

	if(amelioration == 1)
	{
		for(int i = 3; i <= 5; i += 1)
		{
			for(int j = 3; j <= 5; j += 1)
			{
				plateau[i*NB_COLONNES+j].couleur = -2;
				plateau[i*NB_COLONNES+j].valeur = 0;
			}
		}
	}

#ifdef DEBUG
printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

return plateau;
}

void f_affiche_plateau(Pion *plateau)
{
	int i,j,k;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("%2c ",f_convert_int2char(k));
	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("-- ");
	printf("\n");
	for(i=NB_LIGNES-1; i>=0; i--)
	{
		printf("%2d ", i);
		for(j=0; j<NB_COLONNES; j++)
		{
			printf("|");
			switch(plateau[i*NB_COLONNES+j].couleur)
			{
			case -1:
				printf("%do",plateau[i*NB_COLONNES+j].valeur);
				break;
			case 1:
				printf("%dx",plateau[i*NB_COLONNES+j].valeur);
				break;
			case -2:
				printf(" a");
				break;
			default:
				printf("  ");
			}
		}
		printf("|\n    ");
		for(k=0; k<NB_COLONNES; k++)
			printf("-- ");
		printf("\n");
	}
	printf("    ");

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


int f_gagnant()
{
	int i, j, somme1=0, somme2=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	//Quelqu'un est-il arrive sur la ligne de l'autre
	for(i=0; i<NB_COLONNES; i++)
	{
		if(plateauDeJeu[i].couleur == 1)
			return 1;
		if(plateauDeJeu[(NB_LIGNES-1)*NB_COLONNES+i].couleur == -1)
			return -1;
	}

	//taille des armees
	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			if(plateauDeJeu[i*NB_COLONNES+j].couleur == 1)
				somme1++;
			if(plateauDeJeu[i*NB_COLONNES+j].couleur == -1)
				somme2++;
		}
	}
	if(somme1==0)
		return -1;
	if(somme2==0)
		return 1;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend comme argument la ligne et la colonne de la case
 * 	pour laquelle la bataille a lieu
 * Renvoie le couleur du gagnant
 * */
int f_bataille(int l, int c)
{
	int i, j, mini, maxi, minj, maxj;
	int somme=0;

#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	mini = l-1<0?0:l-1;
	maxi = l+1>NB_LIGNES-1?NB_LIGNES-1:l+1;
	minj = c-1<0?0:c-1;
	maxj = c+1>NB_COLONNES-1?NB_COLONNES-1:c+1;

	for(i=mini; i<=maxi; i++)
	{
		for(j=minj; j<=maxj; j++)
		{
			somme += plateauDeJeu[i*NB_COLONNES+j].couleur*plateauDeJeu[i*NB_COLONNES+j].valeur;
		}
	}
	somme -= plateauDeJeu[l*NB_COLONNES+c].couleur*plateauDeJeu[l*NB_COLONNES+c].valeur;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(somme < 0)
		return -1;
	if(somme > 0)
		return 1;

	return plateauDeJeu[l*NB_COLONNES+c].couleur;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
	printf("de (%d,%d) vers (%d,%d)\n", l1, c1, l2, c2);
#endif
	/* Erreur, hors du plateau */
	if(l1 < 0 || l1 >= NB_LIGNES || l2 < 0 || l2 >= NB_LIGNES ||
			c1 < 0 || c1 >= NB_COLONNES || c2 < 0 || c2 >= NB_COLONNES)
		return 1;
	/* Erreur, il n'y a pas de pion a deplacer ou le pion n'appartient pas au joueur*/
	if(plateau[l1*NB_COLONNES+c1].valeur == 0 || plateau[l1*NB_COLONNES+c1].couleur != couleur)
		return 1;
	/* Erreur, tentative de tir fratricide */
	if(plateau[l2*NB_COLONNES+c2].couleur == plateau[l1*NB_COLONNES+c1].couleur)
		return 1;

	if(l1-l2 >1 || l2-l1 >1 || c1-c2 >1 || c2-c1 >1 || (l1==l2 && c1==c2))
		return 1;

	if(plateau[l2*NB_COLONNES+c2].couleur == -2)
		return 1;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 *  et effectue le trantement de l'operation demandée
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
	int gagnant=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	if(f_test_mouvement(plateau, l1, c1, l2, c2, couleur) != 0)
		return 1;


	/* Cas ou il n'y a personne a l'arrivee */
	if(plateau[l2*NB_COLONNES+c2].valeur == 0)
	{
		plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
		plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
		plateau[l1*NB_COLONNES+c1].couleur = 0;
		plateau[l1*NB_COLONNES+c1].valeur = 0;
	}
	else
	{
		gagnant=f_bataille(l2, c2);
		/* victoire */
		if(gagnant == couleur)
		{
			plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
			plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
		/* defaite */
		else if(gagnant != 0)
		{
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}

//Calcul du nombre de pions sur le plateau du joueur
int f_nbPions(Pion* jeu, int joueur)
{
	int nbPion=0;
	int i, j;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				++nbPion;
			}
		}
	}
	return nbPion;
}

//Calcul de la valeur de tous les pions du joueur
int f_valeur(Pion* jeu, int joueur)
{
	int i, j;
	int valeur=0;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				valeur += jeu[i * NB_COLONNES + j].valeur;
			}
		}
	}
	return valeur;
}

//fonction d'évaluation
int f_eval(Pion* jeu,int joueur)
{
	int playerDist = 0, adversaryDist = 0;
	for (int i = 0; i < NB_LIGNES; i++)
	{
		for (int j = 0; j < NB_COLONNES; j++)
		{
			int col = jeu[i * NB_COLONNES + j].couleur;
			int goal = (col == 1)? 9: 0;
			if(col == joueur)
			{
				playerDist += abs(goal - i);
			} 
			else if(col == -joueur)
			{
				adversaryDist += abs(goal - i);
			}
		}
	}
	int distDiff = playerDist - adversaryDist;
	int valDiff = f_valeur(jeu, joueur) - f_valeur(jeu, -joueur);
	return distDiff + 75 * valDiff + (rand()%10 == 0);
}

//copie du plateau
void f_copie_plateau(Pion* source, Pion* destination)
{
	int i, j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			destination[i * NB_COLONNES + j].couleur = source[i * NB_COLONNES + j].couleur;
			destination[i * NB_COLONNES + j].valeur = source[i * NB_COLONNES + j].valeur;
		}
	}
}

//mise a zero du plateau
Pion* f_raz_plateau()
{
	Pion* jeu = NULL;
	int i, j;
	jeu = (Pion *) malloc(NB_LIGNES * NB_COLONNES * sizeof (Pion));
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			jeu[i * NB_COLONNES + j].couleur = 0;
			jeu[i * NB_COLONNES + j].valeur = 0;
		}
	}
	return jeu;
}

//Algorithm optimizations for minimax
int negamax(Pion* jeu, int dep, int joueur, int alpha, int beta)
{
	if(dep <= 0)
	{
		return f_eval(jeu, joueur);
	}

	node += 1;

	Pion* jeu_ = f_raz_plateau();
	f_copie_plateau(jeu, jeu_);
	int value = -INFINI;
	int i, j, _i, _j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			if(jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				for(int k = 0; k < 9; k++)
				{
					if(k == 4)
						continue;

					_i = i + k/3 - 1;
					_j = j + k%3 - 1;

					if(!f_bouge_piece(jeu_, i, j, _i, _j, joueur))
					{
						int v_ = -negamax(jeu_, dep - 1, -joueur, -beta, -alpha);
						if(value < v_)
						{
							value = v_;
							if(dep == depth)
							{
								move.from.x = i;
								move.from.y = j;
								move.to.x = _i;
								move.to.y = _j;
							}
						}
						if(alphabeta == 1)
						{
							if(alpha < value)
							{
								alpha = value;
							}
							if(alpha >= beta)
							{
								free(jeu_);
								return value;
							}
						}
						jeu_[i*NB_COLONNES+j] = jeu[i*NB_COLONNES+j];
						jeu_[_i*NB_COLONNES+_j] = jeu[_i*NB_COLONNES+_j];
					}
				}
			}
		}
	}

	free(jeu_);
	return value;
}

//Fonction min trouve le minimum des noeuds fils
int f_min(Pion* jeu, int dep, int joueur, int alpha, int beta)
{
	if(dep <= 0)
	{
		return f_eval(jeu, joueur);
	}

	node += 1;

	Pion* jeu_ = f_raz_plateau();
	f_copie_plateau(jeu, jeu_);
	int value = INFINI;
	int i, j, _i, _j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			if(jeu[i * NB_COLONNES + j].couleur == -joueur)
			{
				for(int k = 0; k < 9; k++)
				{
					if(k == 4)
						continue;

					_i = i + k/3 - 1;
					_j = j + k%3 - 1;

					if(!f_bouge_piece(jeu_, i, j, _i, _j, -joueur))
					{
						int v_ = f_max(jeu_, dep - 1, joueur, alpha, beta);
						if(value > v_)
						{
							value = v_;
						}
						if(alphabeta == 1)
						{
							if(beta > value)
							{
								beta = value;
							}
							if(alpha >= beta)
							{
								free(jeu_);
								return value;
							}
						}
						jeu_[i*NB_COLONNES+j] = jeu[i*NB_COLONNES+j];
						jeu_[_i*NB_COLONNES+_j] = jeu[_i*NB_COLONNES+_j];
					}
				}
			}
		}
	}

	free(jeu_);
	return value;
}

//Fonction max trouve le maximum des noeuds fils
int f_max(Pion* jeu, int dep, int joueur, int alpha, int beta)
{
	if(dep <= 0)
	{
		return f_eval(jeu, joueur);
	}

	node += 1;

	Pion* jeu_ = f_raz_plateau();
	f_copie_plateau(jeu, jeu_);
	int value = -INFINI;
	int i, j, _i, _j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			if(jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				for(int k = 0; k < 9; k++)
				{
					if(k == 4)
						continue;
					
					_i = i + k/3 - 1;
					_j = j + k%3 - 1;

					if(!f_bouge_piece(jeu_, i, j, _i, _j, joueur))
					{
						int v_ = f_min(jeu_, dep - 1, joueur, alpha, beta);
						if(value < v_)
						{
							value = v_;
							if(dep == depth)
							{
								move.from.x = i;
								move.from.y = j;
								move.to.x = _i;
								move.to.y = _j;
							}
						}
						if(alphabeta == 1)
						{
							if(alpha < value)
							{
								alpha = value;
							}
							if(alpha >= beta)
							{
								free(jeu_);
								return value;
							}
						}
						jeu_[i*NB_COLONNES+j] = jeu[i*NB_COLONNES+j];
						jeu_[_i*NB_COLONNES+_j] = jeu[_i*NB_COLONNES+_j];
					}
				}
			}
		}
	}

	free(jeu_);
	return value;
}

/**
 * Calcule et joue le meilleur cout
 * */
void f_IA(int joueur, int dep, int alpha, int beta, int algorithm_type)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	depth = dep;
	int value;
	if(algorithm_type == 1)
		value = f_max(plateauDeJeu, dep, joueur, alpha, beta);
	else
		value == negamax(plateauDeJeu, dep, joueur, alpha, beta);

	if(!f_bouge_piece(plateauDeJeu, move.from.x, move.from.y, move.to.x, move.to.y, joueur))
		printf("IA bouge avec la valeur : %d, depart x = %d, depart y = %d, arrive x = %d, arrive y = %d\n", 
			value, move.from.x, move.from.y, move.to.x, move.to.y);
	else
		printf("IA bouge avec la valeur : %d, depart x = %d, depart y = %d, arrive x = %d, arrive y = %d\n", 
			value, move.from.x, move.from.y, move.to.x, move.to.y);

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


/**
 * Demande le choix du joueur humain et calcule le coup demande
 * */
void f_humain(int joueur)
{
	char c1, c2;
	char buffer[32];
	int l1, l2;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("joueur ");
	switch(joueur)
	{
	case -1:
		printf("o ");
		break;
	case 1:
		printf("x ");
		break;
	default:
		printf("inconnu ");
	}
	printf("joue:\n");
	while(1)
	{
		fgets(buffer, 32, stdin);
		if(sscanf(buffer, "%c%i%c%i\n", &c1, &l1, &c2, &l2) == 4)
		{
			if(f_bouge_piece(plateauDeJeu, l1, f_convert_char2int(c1), l2, f_convert_char2int(c2), joueur) == 0)
				break;
		}
		fflush(stdin);
		printf("mauvais choix\n");
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

int main(int argv, char *argc[])
{
	srand(time(0));

	int profondeur_MAX = 6;
	int prof = 3;
	int nb_pion;
	int amelioration;
	int data[profondeur_MAX][2];
	int test, mode, algorithm_type;

	printf("1 play with amelioration\n2 play without amelioration\n");
	scanf("%d", &amelioration);

	printf("1 play with minimax Algorithm\n2 play with negamax Algorithm\n");
	scanf("%d", &algorithm_type);

	printf("1 play with alpha-beta plugin\n2 play without alpha-beta plugin\n");
	scanf("%d", &alphabeta);

	if(amelioration == 2)
	{
		printf("1 generate test data\n2 play game\n");
		scanf("%d", &test);
	}
	else
	{
		test = 2;
	}

	nb_pion = (amelioration == 1) ? 3 : 1;

	if(test == 2)
	{
		printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n");
		scanf("%d",&mode);
	}

	for(int profondeur = (test == 2) ? prof : 1; profondeur <= profondeur_MAX; profondeur++)
	{
		int fin, ret, joueur = 1, alpha = -50, beta = 50;
		mode = (test == 2) ? mode : 3;

		plateauDeJeu = f_init_plateau(amelioration);
		node = 0;
		fin = 0;

		while (!fin)
		{
			for(int i = 0; i < nb_pion; i++)
			{
				f_affiche_plateau(plateauDeJeu);
				if(mode==1)
				{
					if(joueur>0)
						f_humain(joueur);
					else
						f_IA(joueur, profondeur, alpha, beta, algorithm_type);
				}
				else if(mode==2)
				{
					f_humain(joueur);
				}
				else
				{
					f_IA(joueur, profondeur, alpha, beta, algorithm_type);
				}

				if ((ret = f_gagnant()) != 0)
				{
					switch (ret)
					{
					case 1:
						f_affiche_plateau(plateauDeJeu);
						printf("joueur x gagne!\n");
						fin = 1;
						break;
					case -1:
						f_affiche_plateau(plateauDeJeu);
						printf("joueur o gagne!\n");
						fin = 1;
						break;
					}
				}
			}
			joueur = -joueur;
		}
		if(test == 1)
		{
			data[profondeur - 1][0] = profondeur;
			data[profondeur - 1][1] = node / 1000;			
		}
		else
		{
			break;
		}
	}

	
	if(test == 1)
	{
		char* file_name;
		if(algorithm_type == 1)
			file_name = (alphabeta == 1) ? "data_minimax_alphabeta_analyse.dat" : "data_minimax_analyse.dat";
		else
			file_name = (alphabeta == 1) ? "data_negamax_alphabeta_analyse.dat" : "data_negamax_analyse.dat";
		FILE* file=fopen(file_name, "w+");
	
		for(int i = 0; i < profondeur_MAX; i++)
			fprintf(file, "%d %d\n", data[i][0], data[i][1]);
	
		fclose(file);
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

	return 0;
}