#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_PLAYER_COUNT 26
#define HEDGEHOG_COUNT 4
#define START 0


#define DEFAULT_SIZE 4
#define GROWTH_FACTOR 2

/// Allows storage of hedgehogs (identified by their player) as a stack
typedef struct {
    int* player_i;
    size_t capacity;
    size_t len;
} HedgehogStack;

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
    uint8_t current_player;
    int* players
} Board;


Game* init_game(int player_count, int n_lines, int n_columns);
bool can_move(Game* g, int hedgehog_index);

HedgehogStack HedgehogStack_empty();
void HedgehogStack_push(HedgehogStack* stack, int player_i);
int HedgehogStack_pop(HedgehogStack* stack);
int HedgehogStack_peek(HedgehogStack* stack);
bool HedgehogStack_is_empty(HedgehogStack* stack);
void HedgehogStack_destroy(HedgehogStack* stack);

/// Creates a board with traps
Board* Board_init(size_t width, size_t height);

BoardCell* Board_get(Board* board, size_t line, size_t column);

void Board_push(Board* b, size_t line, size_t column, char ctn);
char Board_pop(Board* b, size_t line, size_t column);

#endif
