#include "arenasnake.c"
#include <stdlib.h>
#include <stdio.h>

t_move char_to_move(char cha) {
    if (cha == 'N') {
        return NORTH;
    } else if (cha == 'E') {
        return EAST;
    } else if (cha == 'S') {
        return SOUTH;
    } else {
        return WEST;
    }
}

void move_to_char(t_move move) {
    if (move == NORTH) {
        printf("North\n");
    } else if (move == EAST) {
        printf("East\n");
    } else if (move == SOUTH) {
        printf("South\n");
    } else {
        printf("West\n");
    }
}

int main(void) {
    connectToServer("localhost", 1234, "Yedam1"); // Connecter sur le serveur

    char gameName[50];
    int* X = malloc(sizeof(int));
    int* Y = malloc(sizeof(int)); 
    int* nbWalls = malloc(sizeof(int));
    waitForSnakeGame("TRAINING RANDOM_PLAYER", gameName, X, Y, nbWalls);
    int* walls = malloc((*nbWalls) * sizeof(int) * 4);
    int playerNumber = getSnakeArena(walls);
    t_return_code game_done = NORMAL_MOVE;
    t_move* enemy_move = malloc(sizeof(t_move));
    char cha;

    snake player0, player1;
    printf("Initializing players...\n");

    // Initialize snakes based on who starts
    if (playerNumber == 0) {
        player0 = init_snake(2, (*Y) / 2);
        player1 = init_snake((*X) - 3, (*Y) / 2);
    } else {
        player0 = init_snake((*X) - 3, (*Y) / 2);
        player1 = init_snake(2, (*Y) / 2);
    }
    arena a = init_arena(*X, *Y, *nbWalls, walls, player0, player1, playerNumber);

    printf("Initialization complete.\n");

    while (game_done == NORMAL_MOVE) {
        printArena();
        print_arena(a);
        if (playerNumber == 0) {
            printf("It's your turn\n");
            printf("turns %d body number %d\n",player0->nb_turns,player0->body_number);
            printf("Process any moves: N:North E:East S:South W:West\n");
            scanf(" %c", &cha); 
            game_done = sendMove(char_to_move(cha));
            printf("Player move: %c\n", cha);
            printf("move_snake_started\n");
            move_snake(player0, char_to_move(cha), *X, *Y,a, 0);
            printf("move_snake_finished\n");
            playerNumber = 1;
        } else {
            printf("It's Opponent's turn\n");
            printf("turns %d body number %d\n",player1->nb_turns,player1->body_number);
            game_done = getMove(enemy_move);
            printf("The Opponent moved ");
            move_to_char(*enemy_move);
            printf("\n");
            printf("move_snake_started\n");
            move_snake(player1, *enemy_move, *X, *Y, a, 1);
            printf("move_snake_finished\n");
            playerNumber = 0;
        }
    }

    // Showing Result
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
    free_arena(a,0);
    free(enemy_move);
    free(walls);
    free(nbWalls);
    free(X);
    free(Y);
    closeConnection();
    return 0;
}
