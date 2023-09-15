#include "asteroids.h" // Include the header file for the Asteroids game
#include <limits.h> // Include the header file for the INT_MAX constant
#include <stdbool.h> // Include the header file for the bool type

#define MAX_DEPTH 10 // Define the maximum depth for the DFS traversal

void DepthFirstSearch(int field[][FIELD_WIDTH], int row, int col, int depth, int *min_cost, int cost, int memo[][FIELD_WIDTH]) {
    // Base case: return if out of bounds or exceeding the maximum depth
    if (row < 0 || row >= FIELD_HEIGHT || col >= FIELD_WIDTH || depth > MAX_DEPTH) {
        return;
    }

    // If the result for this cell has already been calculated, return it from the memo
    if (memo[row][col] != -1 && cost + memo[row][col] >= *min_cost) {
        return;
    }

    // If there's an asteroid in the current cell, increase the cost
    if (field[row][col] == ASTEROID_VAL) {
        cost += 1;
    }

    // If we have reached the last column or the maximum depth, update the minimum cost
    if (col == FIELD_WIDTH - 1 || depth == MAX_DEPTH) {
        if (cost < *min_cost) {
            *min_cost = cost;
        }
        // Store the result in the memo
        memo[row][col] = *min_cost - cost;
        return;
    }

    // Recursively call the DFS function for the upper row, same column, and the next depth
    DepthFirstSearch(field, row - 1, col + 1, depth + 1, min_cost, cost, memo);
    // Recursively call the DFS function for the same row, next column, and the next depth
    DepthFirstSearch(field, row, col + 1, depth + 1, min_cost, cost, memo);
    // Recursively call the DFS function for the lower row, same column, and the next depth
    DepthFirstSearch(field, row + 1, col + 1, depth + 1, min_cost, cost, memo);

    // Store the result for this cell in the memo
    memo[row][col] = *min_cost - cost;
}

// Define the move_ship function, which determines the best move for the ship
struct ship_action move_ship(int field[][FIELD_WIDTH], void *ship_state) {
    struct ship_action action;
    int row;

    // Find the current row of the ship
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        if (field[i][SHIP_WIDTH - 1] == SHIP_VAL) {
            row = i;
            break;
        }
    }

    // Initialize the minimum cost variables for each possible move
    int min_cost_up = INT_MAX;
    int min_cost_down = INT_MAX;
    int min_cost_no_move = INT_MAX;
    // Initialize the memo array
    int memo[FIELD_HEIGHT][FIELD_WIDTH];
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            memo[i][j] = -1;
        }
    }

    // Call the DFS function for each possible move and calculate the minimum cost
    DepthFirstSearch(field, row - 1, SHIP_WIDTH, 1, &min_cost_up, 0, memo);
    DepthFirstSearch(field, row, SHIP_WIDTH, 1, &min_cost_no_move, 0, memo);
    DepthFirstSearch(field, row + 1, SHIP_WIDTH, 1, &min_cost_down, 0, memo);


    // Choose the best move based on the minimum cost
    if (min_cost_no_move <= min_cost_up && min_cost_no_move <= min_cost_down) {
        action.move = MOVE_NO;
    } else if (min_cost_up <= min_cost_no_move && min_cost_up <= min_cost_down) {
        action.move = MOVE_UP;
    } else {
        action.move = MOVE_DOWN;
    }

    // Set the ship action state to NULL and return the action
    action.state = NULL;
    return action;
}
