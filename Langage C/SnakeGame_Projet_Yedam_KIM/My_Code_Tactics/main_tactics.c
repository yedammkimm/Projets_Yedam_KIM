#include "tactics.h"
#include <stdlib.h>

int main(void) {
    connectToServer("localhost", 3456, "MIDOSAMA_Yedam"); // Connecter sur le serveur
    while(1){
        char gameName[50];
        int* X = malloc(sizeof(int));
        int* Y = malloc(sizeof(int)); 
        int* nbWalls = malloc(sizeof(int));
        waitForSnakeGame("TOURNAMENT EI", gameName, X, Y, nbWalls);
        int* walls = malloc((*nbWalls) * sizeof(int) * 4);
        int playerNumber = getSnakeArena(walls);
        t_return_code game_done = NORMAL_MOVE;
        t_move* enemy_move = malloc(sizeof(t_move));
        snake player0, player1;
        if (playerNumber == 0) {
            player0 = init_snake(2, (*Y) / 2);
            player1 = init_snake((*X) - 3, (*Y) / 2);
        } else {
            player0 = init_snake((*X) - 3, (*Y) / 2);
            player1 = init_snake(2, (*Y) / 2);
        }
        arena a = init_arena(*X, *Y, *nbWalls, walls, player0, player1, playerNumber);
        while (game_done == NORMAL_MOVE) {
            printArena();
            if (playerNumber == 0) {
                t_move mymove = get_best_move(a,player0,player1,playerNumber);
                game_done = sendMove(mymove);
                move_snake(player0,mymove, *X, *Y,a, 0);
                playerNumber = 1;
            } else {
                game_done = getMove(enemy_move);
                move_snake(player1, *enemy_move, *X, *Y, a, 1);
                playerNumber = 0;
            }
        }
        if (game_done == WINNING_MOVE) {
            if (playerNumber == 0) {
                printf("The Opponent won the game\n");
            } else {
                printf("You won the game\n");
            }
        } else { // the only case when gamedone == LOSING_MOVE
            if (playerNumber == 0) {
                printf("The Opponent lost the game\n");
            } else {
                printf("You lost the game\n");
            }
        }
        free_snake(player0);
        free_snake(player1);
        free_arena(a, 0); // Do not free snakes again
        free(enemy_move);
        free(walls);
        free(nbWalls);
        free(X);
        free(Y);
    }
    closeConnection();
    return 0;
}
