#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define huPlayer 'O'
#define aiPlayer 'X'

#define boxwin(a, param) printf("\033[%dm", a); printf("%c", param); printf("\033[0m");

typedef struct {
	int score;
	int index;
}moves;

//variables globales
char origBoard[9] = {'O', '#', 'X', 'X', '#', 'X', '#', 'O', '#'};

int* emptySquares(int* som, char* board){
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

int checkTie(){
	int tmp;
	emptySquares(&tmp, origBoard);
	return (tmp == 0);
}

void teiGame(){
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

void gameOver(char* box, char player){
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

void draw_scene(){
	for(int i = 0; i < 3; i++){
		printf("-------\n");
		for(int j = 0; j < 3; j++) {
			printf("|%c", origBoard[i*3+j]);
		}
		printf("|\n");
	}
	printf("-------\n");
}

int checkWin(char* board, char player,char* box){
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
	
	int fact = (player == aiPlayer) ? 2 : 1;

	return end * fact;
}

moves minimax(char* newBoard, char player){
	int s;
	moves m;
	char* b = malloc(3*sizeof(char));

	char* board = malloc(9*sizeof(char));
	for(int i = 0; i < 9; i++)
		board[i] = newBoard[i];

	int* availSpots = emptySquares(&s, board);

	if(checkWin(board, player, b)){
		m.score = -10;
		return m;
	}
	else if(checkWin(board, aiPlayer, b)){
		m.score = 10;
		return m;
	}
	else if(s == 0){
		m.score = 0;
		return m;
	}

	moves* move = malloc(s*sizeof(moves));
	for(int i = 0; i < s; i++){
		moves m_;
		m_.index = availSpots[i];
		board[availSpots[i]] = player;

		if(player == aiPlayer){
			moves result = minimax(board, huPlayer);
			m_.score = result.score;
		}
		else{
			moves result = minimax(board, aiPlayer);
			m_.score = result.score;	
		}
		move[i] = m_;
	}

	int bestMove;
	if(player = aiPlayer){
		int bestScore = -10000;
		for(int i = 0; i < s; i++){
			if(move[i].score > bestScore){
				bestScore = move[i].score;
				bestMove = i;
			}
		}
	}
	else{
		int bestScore = 10000;
		for(int i = 0; i < s; i++){
			if(move[i].score < bestScore){
				bestScore = move[i].score;
				bestMove = i;
			}
		}		
	}

	return move[bestMove];
}

int bestSpot(){
	int id = minimax(origBoard, aiPlayer).index;
	return id;
}

int turn(int id, char player, char* box){
	origBoard[id] = player;
	int w_ = checkWin(origBoard, player, box);

	return w_;
}

void turnKey(){
	int fin, replay, end_game, end_game_ai = 0;
	int id;
	char* box = malloc(3*sizeof(char));
	char* box_ia = malloc(3*sizeof(char));
	do{
		do{
			printf("enter your choice\n");	
			scanf("%d", &id);
			
			replay = (id == -1);
			fin = ( (origBoard[id] == '#') && (id >= 0) && (id <= 8) ) || replay;
		}while(!fin);
		
		if(replay){
			for(int i = 0; i < 9; i++)
			origBoard[i] = '#';
		}
		else{
			end_game = turn(id, huPlayer, box);
			if(!checkTie())
				end_game_ai = turn(bestSpot(), aiPlayer, box_ia);
			else
				end_game = end_game_ai = -1;
		}

		draw_scene();
	}while(!end_game && end_game_ai!=2 && end_game!=-1);

	if(end_game){
		gameOver(box, huPlayer);
	}
	else if(end_game_ai == 2){
		gameOver(box_ia, aiPlayer);
	}
	else if(end_game_ai == -1 || end_game == -1){
		teiGame();
	}
}

int main(int argc, char const *argv[])
{
	draw_scene();
	turnKey();
	return 0;
}