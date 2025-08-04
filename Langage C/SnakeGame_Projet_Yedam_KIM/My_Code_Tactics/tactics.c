#include "tactics.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif



/*Comments of Functions in the tactics.h*/

int check_collision(arena a, snake s, int player, int direction) {
    body head = s->head;

    if (a->battlefield[head->pos.y][head->pos.x].has_left_wall && direction == 3){
        return 1; 
    }else if(a->battlefield[head->pos.y][head->pos.x].has_right_wall && direction == 1){
        return 1;
    }else if(a->battlefield[head->pos.y][head->pos.x].has_top_wall && direction == 0){
        return 1;
    }else if(a->battlefield[head->pos.y][head->pos.x].has_bottom_wall && direction == 2) {
        return 1; 
    }
    
    // Check collision with its own body
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
    body current = s->head->next;
    while (current != NULL) {
        if (new_pos.x == current->pos.x && new_pos.y == current->pos.y) {
            return 1; 
        }
        current = current->next;
    }
    
    // Check collision with opponent's body
    snake opponent = (player == PLAYER0) ? a->player2 : a->player1;
    current = opponent->head;
    while (current != NULL) {
        if (new_pos.x == current->pos.x && new_pos.y == current->pos.y) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}


int dist_to_nearest_wall(arena a, position pos) {
    int min_dist = MIN(pos.x, a->width - pos.x - 1);
    min_dist = MIN(min_dist, pos.y);
    min_dist = MIN(min_dist, a->height - pos.y - 1);

    for (int y = 0; y < a->height; y++) {
        for (int x = 0; x < a->width; x++) {
            if (a->battlefield[y][x].has_left_wall) {
                int dist = abs(pos.x - x) + abs(pos.y - y);
                if (pos.x > x) {
                    min_dist = MIN(min_dist, dist);
                }
            }
            if (a->battlefield[y][x].has_right_wall) {
                int dist = abs(pos.x - (x + 1)) + abs(pos.y - y);
                if (pos.x < x + 1) {
                    min_dist = MIN(min_dist, dist);
                }
            }
            if (a->battlefield[y][x].has_top_wall) {
                int dist = abs(pos.x - x) + abs(pos.y - y);
                if (pos.y > y) {
                    min_dist = MIN(min_dist, dist);
                }
            }
            if (a->battlefield[y][x].has_bottom_wall) {
                int dist = abs(pos.x - x) + abs(pos.y - (y + 1));
                if (pos.y < y + 1) {
                    min_dist = MIN(min_dist, dist);
                }
            }
        }
    }
    return min_dist;
}


int available_space(arena a, snake s) {
    int space = 0;
    int visited[a->height][a->width];
    for (int y = 0; y < a->height; y++) {
        for (int x = 0; x < a->width; x++) {
            visited[y][x] = 0;
        }
    }
    // Mark the snake's body parts as visited
    body current = s->head;
    while (current != NULL) {
        visited[current->pos.y][current->pos.x] = 1;
        current = current->next;
    }

    position head_pos = s->head->pos;
    position queue[a->width * a->height];
    int front = 0, rear = 0;
    queue[rear++] = head_pos;
    visited[head_pos.y][head_pos.x] = 1;

    while (front < rear) {
        position current = queue[front++];
        space++;
        position directions[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int new_x = current.x + directions[i].x;
            int new_y = current.y + directions[i].y;
            if (new_x >= 0 && new_x < a->width && new_y >= 0 && new_y < a->height &&
                !visited[new_y][new_x] && a->battlefield[new_y][new_x].status == ROAD &&
                !a->battlefield[new_y][new_x].has_left_wall && !a->battlefield[new_y][new_x].has_right_wall &&
                !a->battlefield[new_y][new_x].has_top_wall && !a->battlefield[new_y][new_x].has_bottom_wall) {
                visited[new_y][new_x] = 1;
                queue[rear++] = (position){new_x, new_y};
            }
        }
    }
    return space;
}


int opponent_proximity(arena a, position pos, int player) {
    int min_dist = 1000;
    snake opponent = (player == PLAYER0) ? a->player2 : a->player1;
    body current = opponent->head;
    while (current != NULL) {
        int dist = abs(pos.x - current->pos.x) + abs(pos.y - current->pos.y);
        min_dist = MIN(min_dist, dist);
        current = current->next;
    }
    return min_dist;
}

int reduce_opponent_space(arena a, snake s, snake opponent, int player) {
    int original_space = available_space(a, opponent);
    int directions[4] = {NORTH, EAST, SOUTH, WEST};
    int reduced_space = original_space;

    for (int i = 0; i < 4; i++) {
        arena a_copy = deep_copy_arena(a);
        snake s_copy = deep_copy_snake(s);
        snake opponent_copy = deep_copy_snake(opponent);

        if (!check_collision(a_copy, s_copy, player, directions[i])) {
            move_snake(s_copy, directions[i], a_copy->width, a_copy->height, a_copy, player);
            int new_opp_space = available_space(a_copy, opponent_copy);
            if (new_opp_space < reduced_space) {
                reduced_space = new_opp_space;
            }
        }

        free_snake(s_copy);
        free_snake(opponent_copy);
        free_arena(a_copy,0);
    }

    return original_space - reduced_space;
}

int increase_my_space(arena a, snake s, int player) {
    int original_space = available_space(a, s);
    int directions[4] = {NORTH, EAST, SOUTH, WEST};
    int increased_space = original_space;

    for (int i = 0; i < 4; i++) {
        arena a_copy = deep_copy_arena(a);
        snake s_copy = deep_copy_snake(s);

        if (!check_collision(a_copy, s_copy, player, directions[i])) {
            move_snake(s_copy, directions[i], a_copy->width, a_copy->height, a_copy, player);
            int new_space = available_space(a_copy, s_copy);
            if (new_space > increased_space) {
                increased_space = new_space;
            }
        }

        free_snake(s_copy);
        free_arena(a_copy,0);
    }

    return increased_space - original_space;
}

int forced_path_risk(arena a, snake s) {
    body head = s->head;
    int forced_paths = 0;
    position directions[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    for (int i = 0; i < 4; i++) {
        int new_x = head->pos.x + directions[i].x;
        int new_y = head->pos.y + directions[i].y;
        if (new_x >= 0 && new_x < a->width && new_y >= 0 && new_y < a->height) {
            if (a->battlefield[new_y][new_x].status != ROAD || a->battlefield[new_y][new_x].has_left_wall || 
                a->battlefield[new_y][new_x].has_right_wall || a->battlefield[new_y][new_x].has_top_wall || 
                a->battlefield[new_y][new_x].has_bottom_wall) {
                forced_paths++;
            }
        }
    }
    return forced_paths;
}

int heuristic(arena a, snake s, int player) {
    body head = s->head;
    int space = available_space(a, s);
    int wall_dist = dist_to_nearest_wall(a, head->pos);
    int opp_proximity = opponent_proximity(a, head->pos, player);
    snake opponent = (player == PLAYER0) ? a->player2 : a->player1;
    int opp_space = available_space(a, opponent);
    int opp_space_reduction = reduce_opponent_space(a, s, opponent, player);
    int my_space_increase = increase_my_space(a, s, player);
    int forced_path = forced_path_risk(a, s);

    int space_weight = 30;
    int wall_dist_weight = 10;
    int opp_proximity_weight = 5;
    int opp_space_weight = 5;
    int opp_space_reduction_weight = 25;
    int my_space_increase_weight = 8;
    int forced_path_weight = 15;  

    int heuristic_value = (space_weight * space) 
                        + (wall_dist_weight * wall_dist) 
                        - (opp_proximity_weight * opp_proximity) 
                        - (opp_space_weight * opp_space)
                        + (opp_space_reduction_weight * opp_space_reduction)
                        + (my_space_increase_weight * my_space_increase)
                        - (forced_path_weight * forced_path);

    return heuristic_value;
}


snake deep_copy_snake(snake s) {
    if (!s) {
        return NULL;
    }
    snake s_copy = malloc(sizeof(struct snake_));
    if (!s_copy) {
        return NULL;
    }

    body current = s->head;
    body head_copy = malloc(sizeof(struct body_));
    if (!head_copy) {
        free(s_copy);
        return NULL;
    }

    head_copy->pos = current->pos;
    head_copy->next = NULL;
    s_copy->head = head_copy;
    s_copy->body_number = s->body_number;
    s_copy->last_direction = s->last_direction;
    s_copy->nb_turns = s->nb_turns;

    current = current->next;
    body current_copy = head_copy;
    while (current != NULL) {
        body new_body = malloc(sizeof(struct body_));
        if (!new_body) {
            free_snake(s_copy); 
            return NULL;
        }
        new_body->pos = current->pos;
        new_body->next = NULL;
        current_copy->next = new_body;
        current_copy = new_body;
        current = current->next;
    }
    return s_copy;
}

arena deep_copy_arena(arena a) {
    arena a_copy = malloc(sizeof(struct arena_));
    if (!a_copy) {
        return NULL;
    }

    a_copy->width = a->width;
    a_copy->height = a->height;
    a_copy->battlefield = malloc(a_copy->height * sizeof(cell_t*));
    if (!a_copy->battlefield) {
        free(a_copy);
        return NULL;
    }

    for (int y = 0; y < a_copy->height; y++) {
        a_copy->battlefield[y] = malloc(a_copy->width * sizeof(cell_t));
        if (!a_copy->battlefield[y]) {
            for (int j = 0; j < y; j++) {
                free(a_copy->battlefield[j]);
            }
            free(a_copy->battlefield);
            free(a_copy);
            return NULL;
        }
        for (int x = 0; x < a_copy->width; x++) {
            a_copy->battlefield[y][x] = a->battlefield[y][x];
        }
    }

    a_copy->player1 = deep_copy_snake(a->player1);
    if (!a_copy->player1) {
        free_arena(a_copy, 0); 
        return NULL;
    }

    a_copy->player2 = deep_copy_snake(a->player2);
    if (!a_copy->player2) {
        free_snake(a_copy->player1); 
        free_arena(a_copy, 0); 
        return NULL;
    }

    return a_copy;
}



int minimax(arena a, snake s, snake opponent, int depth, int alpha, int beta, int player) {
    if (depth == 0) {
        return heuristic(a, s, player);
    }
    int directions[4] = {NORTH, EAST, SOUTH, WEST};
    int maxEval = (player == 0) ? -10000 : 10000;

    for (int i = 0; i < 4; i++) {
        snake s_copy = deep_copy_snake(s);
        snake opponent_copy = deep_copy_snake(opponent);
        arena a_copy = deep_copy_arena(a);
        if (!check_collision(a_copy, s_copy, player, directions[i])) {
            move_snake(s_copy, directions[i], a->width, a->height, a_copy, player);  
            int eval = minimax(a_copy, opponent_copy, s_copy, depth - 1, alpha, beta, !player);
            if (player == 0) {
                maxEval = MAX(maxEval, eval);
                alpha = MAX(alpha, eval);
            } else {
                maxEval = MIN(maxEval, eval);
                beta = MIN(beta, eval);
            }
            if (beta <= alpha) {
                free_snake(s_copy);
                free_snake(opponent_copy);
                free_arena_copy(a_copy);
                break;
            }
        }
        free_snake(s_copy);
        free_snake(opponent_copy);
        free_arena_copy(a_copy);
    }
    return maxEval;
}


bool is_terminal_state(arena a, snake s, snake opponent) {
    return check_collision(a, s, 0, s->last_direction) || check_collision(a, opponent, 1, opponent->last_direction);
}

int get_best_move(arena a, snake s, snake opponent, int player) {
    int best_move = -1;
    int best_value = -10000; 
    int directions[4] = {NORTH, EAST, SOUTH, WEST};
    int valid_directions[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        snake s_copy = deep_copy_snake(s);
        snake opponent_copy = deep_copy_snake(opponent);
        arena a_copy = deep_copy_arena(a);
        if (!check_collision(a_copy, s_copy, player, directions[i])) {
            valid_directions[i] = 1; 
        }

        if (s_copy->last_direction == NORTH) {
            valid_directions[2] = 0;
        } else if (s_copy->last_direction == EAST) {
            valid_directions[3] = 0;
        } else if (s_copy->last_direction == SOUTH) {
            valid_directions[0] = 0;
        } else if (s_copy->last_direction == WEST) {
            valid_directions[1] = 0;
        }
        free_snake(s_copy);
        free_snake(opponent_copy);
        free_arena_copy(a_copy);
    }

    // Calculate heuristic and determine the best move
    bool valid_move_found = false;
    for (int i = 0; i < 4; i++) {
        if (valid_directions[i]) {
            snake s_copy = deep_copy_snake(s);
            snake opponent_copy = deep_copy_snake(opponent);
            arena a_copy = deep_copy_arena(a);
            move_snake(s_copy, directions[i], a->width, a->height, a_copy, player);
            int move_value = minimax(a_copy, s_copy, opponent_copy, 8, -10000, 10000, player);

            // Debug print to show the value of each move
            if (!valid_move_found || move_value > best_value) {
                best_value = move_value;
                best_move = directions[i];
                valid_move_found = true;
            }
            free_snake(s_copy);
            free_snake(opponent_copy);
            free_arena_copy(a_copy);
        }
    }
    
    // If no valid move found, return the first valid direction
    if (!valid_move_found) {
        for (int i = 0; i < 4; i++) {
            if (valid_directions[i]) {
                best_move = directions[i];
                break;
            }
        }
    }

    s->last_direction = best_move;
    return best_move;
}
