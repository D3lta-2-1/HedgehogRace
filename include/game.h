#ifndef GAME_H
#define GAME_H

#include <hedgehog_stack.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_PLAYER_COUNT 26
#define HEDGEHOG_COUNT 4
#define START 0

/// Allows quick access to each players hedgehogs and number of hedgehogs that
/// have finished the race
typedef struct {
    HedgehogStack stack;
    bool trapped;
} BoardCell;

typedef struct {
    BoardCell* cells;
    size_t width;
    size_t height;
} Board;

/// Creates a board with traps
Board Board_init(size_t width, size_t height);
BoardCell* Board_get(Board* board, size_t line, size_t column);
void Board_destroy(Board* board);

typedef struct {
    Board board;
    Owner current_player;
    size_t* finished_hedgehogs_counts;
} Game;

Game Game_init(int player_count, int n_lines, int n_columns);
/// returns true if current_player's hedgehog number ``hedgehog_index`` can move
bool Game_can_move(Game* game, int player, int line, int column);
bool Game_can_player_move_vertically(Game* game);
bool Game_can_player_move_horizontally(Game* game, int line);
void Game_move_hedgehog_vertical(Game* game, size_t line, size_t column, int answer);
void Game_move_hedgehog_horizontal(Game* game, size_t line, size_t column);
int Game_check_hedgehog_vertical(Game* game, size_t line, size_t column);
int Game_check_hedgehog_horizontal(Game* game, size_t line, size_t column);
void Game_destroy(Game* game);

#endif
