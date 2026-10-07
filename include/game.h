#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_PLAYER_COUNT 26
#define HEDGEHOG_COUNT 4
#define START 0

/// Stores a unique hedgehog id and current position on the board
typedef struct {
    int id;
    int line;
    int column;
} Hedgehog;

#define DEFAULT_SIZE 4
#define GROWTH_FACTOR 2
/// allows storage of pointers to hedgehogs as a stack
typedef struct {
  Hedgehog **hedgehogs;
  size_t capacity;
  size_t len;
} HedgehogStack;



/// Allows quick access to each players hedgehogs and number of hedgehogs that have finished the race
typedef struct {
    Hedgehog hedgehogs[HEDGEHOG_COUNT];
    int finished;
} Player;

typedef struct {
  HedgehogStack stack;
  bool trapped;
} BoardCell;

typedef struct {
  BoardCell *cells;
  size_t width;
  size_t height;
} Board;

/// Stores major information about the game
typedef struct {
    Board* board;
    Player* players;
    uint8_t current_player;
} Game;

Game* init_game(char player_count, char n_lines, char n_columns);
bool can_move_vertically(Game* g, int hedgehog_index);





HedgehogStack HedgehogStack_empty();
void HedgehogStack_push(HedgehogStack *stack, Hedgehog* hedgehog);
Hedgehog* HedgehogStack_pop(HedgehogStack *stack);
Hedgehog *HedgehogStack_peek(HedgehogStack *stack);
bool HedgehogStack_is_empty(HedgehogStack *stack);
void HedgehogStack_destroy(HedgehogStack *stack);

/// Creates a board with traps
Board* Board_init(size_t width, size_t height);

BoardCell *Board_get(Board *board, size_t line, size_t column);


void Board_push(Board *b, size_t line, size_t column, char ctn);
char Board_pop(Board *b, size_t line, size_t column);

#endif