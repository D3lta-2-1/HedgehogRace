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


Board* init_game(int player_count, int n_lines, int n_columns);
bool can_move(Board* b, int player, int line, int column);
bool player_can_move_vertical(Board* b);
bool player_can_move_horizontal(Board* b, int line);
void move_hedgehog_vertical(Board* b, int line, int column, int answer);
void move_hedgehog_horizontal(Board* b, int line, int column);
void to_lower_str(char* s);
int test_yes_no(char* string);
int test_binary_string(char* string, char* string_1, char* string_0);
int check_coordinates_line(Board* b, char answer);
int check_coordinates_column(Board* b, char answer);
int check_hedgehog_vertical(Board* b, int line, int column);
int check_hedgehog_horizontal(Board* b, int line, int column);
int test_vertical_move(Board* b, int answer, char line);
int get_input(Board* b, int i, char* answers);

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
