#ifndef GAME_H
#define GAME_H

#include <hedgehog_stack.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_PLAYER_COUNT 26
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
BoardCell* Board_get(Board* board, size_t x, size_t y);
void Board_destroy(Board* board);

typedef struct {
    Board board;
    Owner current_player;
    int player_count;
    int hedgehog_count;
    size_t* finished_hedgehogs_counts;
} Game;

typedef enum { OPTION_1 = 1, OPTION_2 = 0, ERROR = -1 } Result;

Game Game_init(int player_count, int hedgehog_count, size_t width, size_t height);
/// returns true if player's hedgehog at line,column can move
bool Game_can_move(Game* game, Owner player, size_t x, size_t y);
bool Game_can_player_move_vertically(Game* game);
bool Game_can_player_move_horizontally(Game* game, size_t y);
void Game_move_hedgehog_vertical(Game* game, size_t x, size_t y, int answer);
void Game_move_hedgehog_horizontal(Game* game, size_t x, size_t y);
Result Game_check_hedgehog_vertical(Game* game, size_t x, size_t y);
Result Game_check_hedgehog_horizontal(Game* game, size_t x, size_t y);
void Game_destroy(Game* game);

#endif
