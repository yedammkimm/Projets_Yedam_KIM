#include <stdlib.h>
#include <stdio.h>
#include "snakeAPI.c"
#include "clientAPI.c"

t_move char_to_move(char cha){
    if(cha == 'N'){
        return NORTH;
    }else if(cha == 'E'){
        return EAST;
    }else if(cha == 'S'){
        return SOUTH;
    }else{
        return WEST;
    }
}


void move_to_char(t_move move){
    if (move == NORTH){
        printf("North\n");
    }else if(move == EAST){
        printf("EAST\n");
    }else if(move == SOUTH){
        printf("SOUTH\n");
    }else{
        printf("WEST\n");
    }
}

int main(void){
    connectToServer("localhost",1234,"Yedam"); //Connecter sur le serveur
    char gameName[50];
    int* X = malloc(sizeof(int)); 
    int* Y = malloc(sizeof(int)); 
    int* nbWalls = malloc(sizeof(int));
    waitForSnakeGame("TRAINING RANDOM_PLAYER",gameName,X,Y,nbWalls);
    int* walls = malloc((*nbWalls)*sizeof(int)*4);
    int turn = getSnakeArena(walls);
    t_return_code game_done = NORMAL_MOVE;
    t_move* enemy_move = malloc(sizeof(t_move));
    char cha;
    while(!game_done){
        printArena();
        if(!turn){
            printf("It's your turn\n");
            printf("Process any moves: N:North E:East S:South W:West\n");
            scanf("\n%c",&cha);
            game_done = sendMove(char_to_move(cha));
            turn = (turn+1) % 2;
        }else{
            printf("It's Opponent's turn\n");
            game_done = getMove(enemy_move);
            printf("The Opponent moved ");
            move_to_char(*enemy_move);
            turn = (turn+1) % 2;
        }
    }
    //Showing Result
    if(game_done == 1){
        if(!turn){ //The winning move had been made by the opponent not you 
            printf("The Opponent won the game\n");
        }else{ //The winning move had been made by the you not opponent 
            printf("You won the game\n");
        }
    }else{ //the only case when gamedone == -1
        if(!turn){ //The losing move had been made by the opponent not you 
            printf("The Opponent lost the game\n");
        }else{ //The losing move had been made by the you not opponent 
            printf("You lost the game\n");
        }
    }
   free(walls);
   free(nbWalls);
   free(X);
   free(Y);
   closeConnection();
}
