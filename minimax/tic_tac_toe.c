#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define huPlayer 'O'
#define aiPlayer 'X'
#define INFINI 10000

#define boxwin(a, param) printf("\033[%dm", a); printf("%c", param); printf("\033[0m");

//variables globales
char origBoard[9] = {'#', '#', '#', '#', '#', '#', '#', '#', '#'};
int dep;

int f_min(char* board, char player, int* index, int depth, int alpha, int beta);
int f_max(char* board, char player, int* index, int depth, int alpha, int beta);

int* emptySquares(int* som, char* board)
{
	int tmp = 0;
	for(int i = 0; i < 9; i++){
		if(board[i] == '#')
			tmp++;
	}
	int* squares = malloc(tmp*sizeof(int));

	tmp = 0;
	for(int i = 0; i < 9; i++){
		if(board[i] == '#'){
			squares[tmp] = i;
			tmp++;
		}
	}

	*som = tmp;
	return squares;
}

int checkTie(char* board)
{
	int tmp;
	emptySquares(&tmp, board);
	return (tmp == 0);
}

void teiGame()
{
	printf("match nul !\n");
	for(int i = 0; i < 3; i++){
		printf("-------\n");
		for(int j = 0; j < 3; j++) {
			printf("|");
			boxwin(32, origBoard[i*3+j]);
		}
		printf("|\n");
	}
	printf("-------\n");	
}

void gameOver(char* box, char player)
{
	int a;
	if(player == huPlayer)
		a = 33;
	else
		a = 31;

	for(int i = 0; i < 3; i++){
		printf("-------\n");
		for(int j = 0; j < 3; j++) {
			int index = i*3+j;
			int find = 0;
			for(int k = 0; k < 3 && !find; k++)
				find = (box[k] - '0') == index;
			if(find){
				printf("|");
				boxwin(a, origBoard[i*3+j]);
			}
			else
				printf("| ");

		}
		printf("|\n");
	}
	printf("-------\n");

}

void draw_scene()
{
	for(int i = 0; i < 3; i++){
		printf("-------\n");
		for(int j = 0; j < 3; j++) {
			printf("|%c", origBoard[i*3+j]);
		}
		printf("|\n");
	}
	printf("-------\n");
}

int checkWin(char* board, char player,char* box)
{
	char* winning_moves[8] = {
		"012",
		"036",
		"258",
		"345",
		"678",
		"048",
		"246",
		"147"
	};

	int i = 0;
	int end = 0;
	int grid_space0, grid_space1, grid_space2;
	while(i < 8 && !end){
		char* box_= winning_moves[i];
		grid_space0 = box_[0] - '0';
		grid_space1 = box_[1] - '0';
		grid_space2 = box_[2] - '0';

		end = (board[grid_space0] == player) && (board[grid_space1] == player) && (board[grid_space2] == player);

		i++;
	}
	box[0] = grid_space0+'0';
	box[1] = grid_space1+'0';
	box[2] = grid_space2+'0';

	int fact = (player == huPlayer) ? 1 : -1;

	return end * fact;
}

int f_bouge(char* board, int index, char player)
{
	if(index > 8 || index < 0)
		return 1;
	if(board[index] != '#')
		return 1;

	board[index] = player;
	return 0;
}

int f_gagnant(char player)
{
	int ret = 0, fin = 0;
	char box[3];

	ret = checkWin(origBoard, player, box);

	if(ret == 1 || ret == -1)
	{
		printf("joueur %c gagne!\n", player);
		gameOver(box, player);
		fin = 1;
	}

	return fin;
}

char* f_init_board()
{
	char* src = (char*)malloc(9 * sizeof(char));

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++) {
			src[i*3+j] = '#';
		}
	}
	return src;
}

void f_copie_board(char* src, char* dst)
{

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++) {
			dst[i*3+j] = src[i*3+j];
		}
	}
}

int f_eval(char* board)
{
	char box[3];
	int score;

	if(checkWin(board, huPlayer, box))
	{
		score = -10;
	}
	else if(checkWin(board, aiPlayer, box))
	{
		score = 10;
	}
	else
	{
		score = 0;
	}

	return score;
}

int f_max(char* board, char player, int* index, int depth, int alpha, int beta)
{
	if(depth <= 0 || checkTie(board))
	{
		return f_eval(board);
	}

	char* board_ = f_init_board();
	f_copie_board(board, board_);
	int value = -INFINI;
	int size;

	int* squares = emptySquares(&size, board);
	for(int i = 0; i < size; i++)
	{
		if(!f_bouge(board_, squares[i], player))
		{
			char player_ = (player == huPlayer) ? aiPlayer : huPlayer;
			int v_ = f_min(board_, player_, NULL, depth - 1, alpha, beta);
			if(value < v_)
			{
				value = v_;
				if(depth == dep)
				{
					*index = squares[i];
				}
			}
			if(alpha < value)
			{
				alpha = value;
			}
			if(alpha >= beta)
			{
				break;
			}
			board_[squares[i]] = '#';
		}
	}
	return value;
}

int f_min(char* board, char player, int* index, int depth, int alpha, int beta)
{

	if(depth <= 0 || checkTie(board))
	{
		return f_eval(board);
	}

	char* board_ = f_init_board();
	f_copie_board(board, board_);
	int value = INFINI;
	int size;

	int* squares = emptySquares(&size, board);
	for(int i = 0; i < size; i++)
	{
		if(!f_bouge(board_, squares[i], player))
		{
			char player_ = (player == huPlayer) ? aiPlayer : huPlayer;
			int v_ = f_max(board_, player_, NULL, depth - 1, alpha, beta);
			if(value > v_)
			{
				value = v_;
			}
			if(beta > value)
			{
				beta = value;
			}
			if(alpha >= beta)
			{
				break;
			}
			board_[squares[i]] = '#';
		}

	}
	return value;
}

void humain(char player)
{
	int index;
	char buffer[32];

	printf("joueur %c joue:\n", player);
	while(1)
	{
		fgets(buffer, 32, stdin);
		if(sscanf(buffer, "%i\n", &index) == 1)
		{
			if(f_bouge(origBoard, index, player) == 0)
				break;
		}
		fflush(stdin);
		printf("mauvais choix\n");
	}
}

void IA(char joueur, int depth, int alpha, int beta)
{
	dep = depth;
	int index;
	int value = f_max(origBoard, joueur, &index, depth, alpha, beta);
	if(!f_bouge(origBoard, index, joueur))
		printf("IA bouge avec la valeur : %d, index = %d\n", value, index);
	else
		printf("IA bouge avec la valeur : %d, index = %d\n", value, index);
}

int main(int argc, char const *argv[])
{
	int fin = 0, mode=0, ret, alpha = -INFINI, beta = INFINI, profondeur = 5;
	char player = huPlayer;

	printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n");
	scanf("%d",&mode);

	while (!fin)
	{
		draw_scene();
		if(mode==1)
		{
			if(player == huPlayer)
			{
				humain(player);
				fin = f_gagnant(player);
			}
			else
			{
				IA(player, profondeur, alpha, beta);
				fin = f_gagnant(player);
			}
		}
		else if(mode==2)
		{
			humain(player);
			fin = f_gagnant(player);
		}
		else
		{
			IA(player, profondeur, alpha, beta);
			fin = f_gagnant(player);
		}

		if(!fin)
		{
			if(checkTie(origBoard))
			{
				teiGame();
				fin = 1;
			}
		}

		player = (player == huPlayer) ? aiPlayer : huPlayer;
	}

	return 0;
}