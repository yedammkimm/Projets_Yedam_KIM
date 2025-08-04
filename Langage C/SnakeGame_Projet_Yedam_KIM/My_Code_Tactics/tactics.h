#include "arenasnake.h"
#include <stdbool.h>

/**
 * Check for collision based on the current direction of the snake.
 * @param a The arena.
 * @param s The snake.
 * @param player The player number (0 or 1).
 * @param direction The direction to check for collision.
 * @return 1 if there is a collision, otherwise 0.
 */
int check_collision(arena a, snake s, int player, int direction);

/**
 * Calculate the distance from the given position to the nearest wall.
 * @param a The arena.
 * @param pos The position to check.
 * @return The distance to the nearest wall.
 */
int dist_to_nearest_wall(arena a, position pos);

/**
 * Calculate the available space for the snake to move in the arena.
 * @param a The arena.
 * @param s The snake.
 * @return The available space for the snake.
 */
int available_space(arena a, snake s);

/**
 * Calculate the proximity of the opponent's snake to the given position.
 * @param a The arena.
 * @param pos The position to check.
 * @param player The player number (0 or 1).
 * @return The distance to the nearest part of the opponent's snake.
 */
int opponent_proximity(arena a, position pos, int player);

/**
 * Calculate the reduction in available space for the opponent's snake based on potential moves.
 * @param a The arena.
 * @param s The player's snake.
 * @param opponent The opponent's snake.
 * @param player The player number (0 or 1).
 * @return The reduction in available space for the opponent's snake.
 */
int reduce_opponent_space(arena a, snake s, snake opponent, int player);

/**
 * Calculate the increase in available space for the player's snake based on potential moves.
 * @param a The arena.
 * @param s The player's snake.
 * @param player The player number (0 or 1).
 * @return The increase in available space for the player's snake.
 */
int increase_my_space(arena a, snake s, int player);

/**
 * Calculate the risk of being forced into a path that leads to a collision.
 * @param a The arena.
 * @param s The snake.
 * @return The risk value.
 */
int forced_path_risk(arena a, snake s);

/**
 * Calculate the heuristic value for the current state of the game.
 * @param a The arena.
 * @param s The player's snake.
 * @param player The player number (0 or 1).
 * @return The heuristic value.
 */
int heuristic(arena a, snake s, int player);

/**
 * Create a deep copy of the given snake.
 * @param s The snake to copy.
 * @return A deep copy of the snake.
 */
snake deep_copy_snake(snake s);

/**
 * Create a deep copy of the given arena.
 * @param a The arena to copy.
 * @return A deep copy of the arena.
 */
arena deep_copy_arena(arena a);

/**
 * Determine if the player should switch to an aggressive strategy.
 * @param s The player's snake.
 * @param opponent The opponent's snake.
 * @param a The arena.
 * @param player The player number (0 or 1).
 * @return True if the player should switch to aggressive, otherwise false.
 */
bool should_switch_to_aggressive(snake s, snake opponent, arena a, int player);

/**
 * Perform the minimax algorithm to find the best move.
 * @param a The arena.
 * @param s The player's snake.
 * @param opponent The opponent's snake.
 * @param depth The depth of the search.
 * @param alpha The alpha value for alpha-beta pruning.
 * @param beta The beta value for alpha-beta pruning.
 * @param player The player number (0 or 1).
 * @return The heuristic value of the best move.
 */
int minimax(arena a, snake s, snake opponent, int depth, int alpha, int beta, int player);

/**
 * Check if the current state is a terminal state (end of the game).
 * @param a The arena.
 * @param s The player's snake.
 * @param opponent The opponent's snake.
 * @return True if it is a terminal state, otherwise false.
 */
bool is_terminal_state(arena a, snake s, snake opponent);

/**
 * Get the best move for the player's snake based on the current state of the game.
 * @param a The arena.
 * @param s The player's snake.
 * @param opponent The opponent's snake.
 * @param player The player number (0 or 1).
 * @return The best move direction.
 */
int get_best_move(arena a, snake s, snake opponent, int player);


