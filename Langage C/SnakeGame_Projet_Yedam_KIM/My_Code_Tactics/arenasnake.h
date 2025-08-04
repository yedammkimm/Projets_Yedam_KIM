#include "snakeAPI.h"


/* Coordinates structure to represent the position of an element in the arena */
struct position_{
    int x; // X-coordinate
    int y; // Y-coordinate
};

typedef struct position_ position;

/* Body structure to represent each segment of the snake's body */
/* Linked list of positions */
typedef struct body_{
    position pos; // Position of the body segment
    struct body_* next; // Pointer to the next segment in the body
}* body;

/* Snake structure to represent the snake */
typedef struct snake_{
    body head; // Head of the snake (first segment)
    int body_number; // Number of segments in the snake's body
    int last_direction; // Last direction the snake moved
    int nb_turns; // Number of turns the snake can move without growing
}* snake;

/* Enumeration to represent the status of a field in the arena */
typedef enum {
	ROAD = -1,   // Empty space
	PLAYER0 = 0, // Space occupied by player 0
	PLAYER1 = 1, // Space occupied by player 1
} field_stat;

/* Cell structure to represent each cell in the arena */
typedef struct {
    field_stat status; // Status of the cell (ROAD, PLAYER0, PLAYER1)
    int has_right_wall; // 1 if there is a wall on the right side, 0 otherwise
    int has_left_wall;  // 1 if there is a wall on the left side, 0 otherwise
    int has_bottom_wall; // 1 if there is a wall on the bottom side, 0 otherwise
    int has_top_wall; // 1 if there is a wall on the top side, 0 otherwise
} cell_t;

/* Arena structure to represent the game area */
struct arena_ {
    int width; // Width of the arena
    int height; // Height of the arena
    cell_t** battlefield; // 2D array representing the arena
    snake player1; // Player 1's snake
    snake player2; // Player 2's snake
};

typedef struct arena_* arena;

/**
 * Initialize a new snake at the given starting position.
 * @param startx The starting x-coordinate of the snake's head.
 * @param starty The starting y-coordinate of the snake's head.
 * @return A pointer to the initialized snake.
 */
snake init_snake(int startx, int starty);

/**
 * Initialize a new arena with given dimensions, walls, and player snakes.
 * @param width The width of the arena.
 * @param height The height of the arena.
 * @param nbWalls The number of walls in the arena.
 * @param walls An array representing the walls' positions.
 * @param player1 Player 1's snake.
 * @param player2 Player 2's snake.
 * @param playerNumber The player number (0 or 1).
 * @return A pointer to the initialized arena.
 */
arena init_arena(int width, int height, int nbWalls, int* walls, snake player1, snake player2, int playerNumber);

/**
 * Print the current state of the arena.
 * @param a The arena to print.
 */
void print_arena(arena a);

/**
 * Free the memory allocated for a snake.
 * @param s1 The snake to free.
 */
void free_snake(snake s1);

/**
 * Free the memory allocated for an arena.
 * @param a The arena to free.
 * @param free_snakes 1 to free the snakes in the arena, 0 otherwise.
 */
void free_arena(arena a, int free_snakes);

/**
 * Free the memory allocated for a copy of the arena.
 * @param a The arena copy to free.
 */
void free_arena_copy(arena a);

/**
 * Move the snake in the given direction within the arena.
 * @param s The snake to move.
 * @param direction The direction to move the snake.
 * @param width The width of the arena.
 * @param height The height of the arena.
 * @param a The arena in which the snake is moving.
 * @param player The player number (0 or 1).
 */
void move_snake(snake s, t_move direction, int width, int height, arena a, int player);


