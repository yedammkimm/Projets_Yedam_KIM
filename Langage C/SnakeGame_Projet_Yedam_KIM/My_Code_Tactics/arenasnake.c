#include "arenasnake.h"
#include <stdlib.h>
#include <stdio.h>


/*Comments of Functions in the arenasnake.h*/

snake init_snake(int startx, int starty) {
    /* Initialize Player Snake */
    snake s = malloc(sizeof(struct snake_));
    body init_body = malloc(sizeof(struct body_));
    init_body->pos.x = startx;
    init_body->pos.y = starty; 
    init_body->next = NULL;
    s->head = init_body;
    s->body_number = 1;
    s->last_direction = -1;
    s->nb_turns = 0;
    return s;
}

/*Walls = Tableau de Position*/
arena init_arena(int width, int height, int nbWalls, int* walls, snake player0, snake player1, int playerNumber) {
    arena a = malloc(sizeof(struct arena_));
    a->width = width;
    a->height = height;
    a->battlefield = malloc(a->height * sizeof(cell_t*));

    for (int y = 0; y < a->height; y++) {
        a->battlefield[y] = malloc(a->width * sizeof(cell_t));
        for (int x = 0; x < a->width; x++) {
            a->battlefield[y][x].status = ROAD;
            a->battlefield[y][x].has_right_wall = 0;
            a->battlefield[y][x].has_left_wall = 0;
            a->battlefield[y][x].has_bottom_wall = 0;
            a->battlefield[y][x].has_top_wall = 0;
        }
    }
    for(int x = 0 ; x < a->width; x++){
        if(x == 0){
            a->battlefield[0][x].has_left_wall = 1;
            a->battlefield[a->height-1][x].has_left_wall = 1; 
        }
        a->battlefield[0][x].has_top_wall = 1;
        if(x == a->width-1){
            a->battlefield[0][x].has_right_wall = 1;
            a->battlefield[a->height-1][x].has_right_wall = 1; 
        }
        a->battlefield[a->height-1][x].has_bottom_wall = 1; 
    }
    for(int y = 1; y < a->height-1; y++){
            a->battlefield[y][0].has_left_wall = 1;
            a->battlefield[y][a->width-1].has_right_wall = 1;
    }

    for (int i = 0; i < nbWalls; i++) {
        int x1 = walls[4 * i];
        int y1 = walls[4 * i + 1];
        int x2 = walls[4 * i + 2];
        int y2 = walls[4 * i + 3];
        if (x1 > x2) {
            int temp = x1; x1 = x2; x2 = temp;
        }
        if (y1 > y2) {
            int temp = y1; y1 = y2; y2 = temp;
        }
        for (int x = x1; x <= x2; x++) {
            for (int y = y1; y <= y2; y++) {
                if (x > x1) {
                    a->battlefield[y][x].has_left_wall = 1;
                }
                if (x < x2) {
                    a->battlefield[y][x].has_right_wall = 1;
                }
                if (y > y1) {
                    a->battlefield[y][x].has_top_wall = 1;
                }
                if (y < y2) {
                    a->battlefield[y][x].has_bottom_wall = 1;
                }
            }
        }
    }
    int player0_x = player0->head->pos.x;
    int player0_y = player0->head->pos.y;
    int player1_x = player1->head->pos.x;
    int player1_y = player1->head->pos.y;

    if (playerNumber == 0) {
        a->battlefield[player0_y][player0_x].status = PLAYER0;
        a->battlefield[player1_y][player1_x].status = PLAYER1;
    } else {
        a->battlefield[player0_y][player0_x].status = PLAYER1;
        a->battlefield[player1_y][player1_x].status = PLAYER0;
    }
    a->player1 = player0;
    a->player2 = player1;
    return a;
}


void print_arena(arena a) {
    for (int x = 0; x < a->width; x++) {
        printf("+-");
    }
    printf("+\n");
    for (int y = 0; y < a->height; y++) {
        for (int x = 0; x < a->width; x++) {
            if (a->battlefield[y][x].has_left_wall) {
                printf("|");
            } else {
                printf(" ");
            }
            if (a->battlefield[y][x].status == PLAYER0) {
                printf("0");
            } else if (a->battlefield[y][x].status == PLAYER1) {
                printf("1");
            } else {
                printf(" ");
            }
        }
        printf("|\n"); 
        for (int x = 0; x < a->width; x++) {
            printf("+");
            if (a->battlefield[y][x].has_bottom_wall) {
                printf("-");
            } else {
                printf(" ");
            }
        }
        printf("+\n");
    }
}

void free_snake(snake s1) {
    if (!s1) {
        return;
    }
    body current = s1->head;
    while (current) {
        body next = current->next;
        free(current);
        current = next;
    }
    free(s1);
}

void free_arena_copy(arena a) {
    free_arena(a, 0);
}


void free_arena(arena a, int free_snakes) {
    if (!a) {
        return;
    }
    for (int i = 0; i < a->height; i++) {
        free(a->battlefield[i]);
    }
    free(a->battlefield);
    if (free_snakes) {
        free_snake(a->player1);
        free_snake(a->player2);
    }
    free(a);
}


void move_snake(snake s, t_move direction, int width, int height, arena a, int player) {
    position new_pos;
    switch (direction) {
        case NORTH:
            new_pos.x = s->head->pos.x;
            new_pos.y = s->head->pos.y - 1;
            break;
        case EAST:
            new_pos.x = s->head->pos.x + 1;
            new_pos.y = s->head->pos.y;
            break;
        case SOUTH:
            new_pos.x = s->head->pos.x;
            new_pos.y = s->head->pos.y + 1;
            break;
        case WEST:
            new_pos.x = s->head->pos.x - 1;
            new_pos.y = s->head->pos.y;
            break;
    }

    if (new_pos.x < 0 || new_pos.x >= width || new_pos.y < 0 || new_pos.y >= height) {
        return;
    }

    if (s->nb_turns == 0){ 
        body new_segment = malloc(sizeof(struct body_));
        if (new_segment == NULL) {
            return;
        }
        new_segment->pos = new_pos;
        new_segment->next = s->head;
        s->head = new_segment;
        s->body_number++;
        body current = s->head;
        while (current != NULL) {
            a->battlefield[current->pos.y][current->pos.x].status = player;
            current = current->next;
        }

        if (player == 0) {
            a->player1->head = s->head;
        } else {
            a->player2->head = s->head;
        }
        s->nb_turns = 9;
    }else{
        position prev_pos = s->head->pos;
        s->head->pos = new_pos;
        a->battlefield[new_pos.y][new_pos.x].status = player;

        body current = s->head->next;
        while (current != NULL) {
            position temp_pos = current->pos;
            current->pos = prev_pos;
            prev_pos = temp_pos;
            a->battlefield[current->pos.y][current->pos.x].status = player;
            current = current->next;
        }
        a->battlefield[prev_pos.y][prev_pos.x].status = ROAD;
        s->nb_turns -= 1;
    }

}
