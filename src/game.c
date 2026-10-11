#include <assert.h>
#include <game.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


Board* init_game(int player_count, int n_lines, int n_columns) {
    assert(player_count <= MAX_PLAYER_COUNT &&
           "cannot have more than 26 players");
    Board* b = Board_init(n_columns, n_lines);
    b->players = (int*)malloc(player_count * sizeof(int));
    for (int i = 0; i < player_count; i++) {
        b->players[i] = 0;
    }
    for (int i = 0; i < player_count * HEDGEHOG_COUNT; i++) {
        int line = rand() % n_lines;
        bool not_allocated = true;
        int player;
        while (not_allocated) {
            player = rand() % player_count;
            if (b->players[player] < HEDGEHOG_COUNT) {
                HedgehogStack_push(
                    &Board_get(b, START, line)->stack, player);
                b->players[player]++;
                not_allocated = false;
            }
        }
    }
    for (int i = 0; i < player_count; i++) {
        b->players[i] = 0;
    }
    b->current_player = 0;
    return b;
}

/// returns true if current_player's hedgehog number hedgehog_index can move
bool can_move(Board* b, int player, int line, int column) {
    if (column == b->width - 1) return false;
    // we know there is at least one hedgehog in the stack
    int tophog = HedgehogStack_peek(
        &Board_get(b, column, line)->stack);
    bool test_1 = player == tophog;
    if (Board_get(b, column, line)->trapped) {
        for (int i = 0; i < column; i++) {
            BoardCell* cell = Board_get(b, line, i);
            if (cell->stack.len != 0)
                return false;
        }
    }
    return test_1;
}

bool player_can_move_vertical(Board* b) {
    for (int lines = 0 ; lines < b->height ; lines++) {
        for (int columns = 0 ; columns < b->width - 1; columns++) {
            if (can_move(b, b->current_player, lines, columns)) return true;
        }
    }
    return false;
}

bool player_can_move_horizontal(Board* b, int line) {
    // hedgehogs on the last line cannot move
    for (int column = 0 ; column < b->width - 2; column++) {
        BoardCell* current = Board_get(b, line, column);
        // If a hedgehog were to be trapped and unable to move then there exists a previous hedgehog that can move
        // Otherwise the trapped hedgehog can move
        if (!HedgehogStack_is_empty(&current->stack)) return true;
    }
    return false;
}

void move_hedgehog_vertical(Board* b, int line, int column, int answer) {
    // we map 0 to -1 and 1 to 1;
    int up_down = 2 * answer - 1;
    BoardCell* current = Board_get(b, line, column);
    int tophog = HedgehogStack_pop(&current->stack);
    BoardCell* next = Board_get(b, line + up_down, column);
    HedgehogStack_push(&next->stack, tophog);
    return;
}

void move_hedgehog_horizontal(Board* b, int line, int column) {
    BoardCell* current = Board_get(b, line, column);
    int tophog = HedgehogStack_pop(&current->stack);
    BoardCell* next = Board_get(b, line, column+1);
    HedgehogStack_push(&next->stack, tophog);
    return;
}

void to_lower_str(char* s) {
  int i = 0;
  while (s[i] != '\0') {
    s[i] = tolower(s[i]);
  }
}

/// test yes no functions return -1 for invalid answers, 0 for no and 1 for yes
int test_yes_no(char* string) {
  if (!strcmp(string, "yes")) {
    return 1;
  } else if (!strcmp(string, "no")) {
    return 0;
  }
  else 
    return -1;
}

int test_binary_string(char* string, char* string_1, char* string_0) {
  if (!strcmp(string, string_1)) {
    return 1;
  } else if (!strcmp(string, string_0)) {
    return 0;
  }
  else 
    return -1;
}

int check_coordinates_line(Board* b, char answer) {
    int line = (int)(answer - '1');
    if (line >= 0 && line < b->height) {
        return line;
    } else {
        return -1;
    } 
}
int check_coordinates_column(Board* b, char answer) {
    int column = (int)(answer - 'a');
    if (column >= 0 && column < b->width) {
        return column;
    } else {
        return -1;
    } 
}

int check_hedgehog_vertical(Board* b, int line, int column) {
    BoardCell* cell = Board_get(b, line, column);
    if (HedgehogStack_is_empty(&cell->stack)) return -1;
    if (can_move(b, b->current_player, line, column)) return 1;
    return -1;
}

int check_hedgehog_horizontal(Board* b, int line, int column) {
    BoardCell* cell = Board_get(b, line, column);
    if (HedgehogStack_is_empty(&cell->stack)) return -1;
    int tophog = HedgehogStack_peek(&cell->stack);
    if (can_move(b, tophog, line, column)) return 1;
    return -1;
}

int test_vertical_move(Board* b, int answer, char line) {
    if (line == 0 && answer == 1) return -1;
    else if (line == b->height - 1 && answer == 0) return -1;
    else return answer;
}

/// manages all of the players answers
/// 0 -> Do you want to move a hedgehog vertically ?
/// 1 -> Which of your hedgehogs would you like to move ?
/// 2 -> Where would you like to move it ?
/// 3 -> which hedgehog would you like to move ?
int get_input(Board* b, int i, char* answers) {
  static char line_static;
  int line_1;
  int column_1;
  int line_3;
  int column_3;
  int res = -1;
  while (res == -1) {
    printf("Player %d, ", b->current_player);
    // Player input
    switch (i) {
      case 0:
        printf("do you want to move a hedgehog vertically ?\n");
        scanf("%s\n", answers);
        to_lower_str(answers);
        break;
      case 1:
        printf("which of your hedgehogs would you like to move ? (format:1a)\n");
        scanf("%s\n", answers);
        tolower(answers[1]);
        break;
      case 2:
        printf("where would you like to move it ? (up or down)\n");
        scanf("%s\n", answers);
        to_lower_str(answers);
        break;
      case 3:
        line_3 = (int)answers[0];
        printf("which hedgehog would you like to move ? (format : a)\n");
        scanf("%s\n", answers);
        tolower(answers[0]);
      default:
        break;
    }
    // Verification and player help
    switch (i) {
      case 0:
        res = test_binary_string(answers, "yes", "no");
        break;
      case 1:
        line_1 = check_coordinates_line(b, answers[0]);
        column_1 = check_coordinates_column(b, answers[1]);
        if (line_1 != -1 && column_1 != -1) {
            res = check_hedgehog_vertical(b, line_1, column_1);
            if (res == -1) {
                printf("Either that isn't a/your hedgehog or it cannot move\nTo be able to move, a hedgehog must either be at the top of a cell or, if it is trapped, the fist on that line, hence :");
            }
        } else {
            printf("Coordinates are format \nLine number Column letter, example 2c, hence :");
        }
        break;
      case 2:
        res = test_binary_string(answers, "up", "down");
        if (res != -1) {
            res = test_vertical_move(b, res, line_static);
            if (res == -1) {
                if (line_static == 0) {
                    printf("This hedgehog cannot move up, hence : ");
                } else {
                    printf("This hedgehog cannot move down, hence : ");
                }
            }
        }
        break;
      case 3:
        column_3 = check_coordinates_column(b, answers[0]);
        if (line_3 != -1 && column_3 != -1) {
            res = check_hedgehog_horizontal(b, line_3, column_3);
            if (res == -1) {
                printf("Either that isn't a hedgehog or it cannot move\nTo be able to move, a hedgehog must either be at the top of a cell or, if it is trapped, the fist on that line, hence :");
            }
        } else {
            printf("Coordinates are format \nColumn letter, example b, hence :");
        }
        break;
    }
    if (res == -1) printf("Invalid entry\n");
  }
  // Returns and data management
  switch (i) {
      case 1:
          line_static = answers[0] - '1';
          return res;
      case 3:
          return column_3;
      default:
          return res;
  }
}
                
                

HedgehogStack HedgehogStack_empty() {
    HedgehogStack stack = {NULL, 0, 0};
    return stack;
}

void Cell_grow(HedgehogStack* stack) {
    if (stack->capacity == 0) {
        stack->player_i = malloc(sizeof(int) * DEFAULT_SIZE);
        stack->capacity = DEFAULT_SIZE;
    } else {
        stack->capacity *= GROWTH_FACTOR;
        stack->player_i = realloc(stack->player_i, stack->capacity);
    }
}

void HedgehogStack_push(HedgehogStack* stack, int player_i) {
    if (stack->len == stack->capacity) {
        Cell_grow(stack);
    }
    stack->player_i[stack->len] = player_i;
    stack->len++;
}

int HedgehogStack_pop(HedgehogStack* stack) {
    assert(stack->len > 0 && "cannot pop on an empty stack");
    int player_i = stack->player_i[stack->len - 1];
    stack->len--;
    return player_i;
}

int HedgehogStack_peek(HedgehogStack* stack) {
    assert(stack->len > 0 && "cannot pop on an empty stack");
    return stack->player_i[stack->len - 1];
}

bool HedgehogStack_is_empty(HedgehogStack* stack) { return stack->len == 0; }

void HedgehogStack_destroy(HedgehogStack* stack) {
    if (stack->player_i)
        return;
    free(stack->player_i);
}

Board* Board_init(size_t width, size_t height) {
    Board* board = (Board*)malloc(sizeof(Board));
    assert(board != NULL && "Board_new Board* malloc failed");
    board->cells = (BoardCell*)malloc(sizeof(BoardCell) * width * height);
    assert(board->cells != NULL && "Board_new BoardCell* malloc failed");
    board->width = width;
    board->height = height;
    for (size_t i = 0; i < width * height; i++) {
        board->cells[i].stack = HedgehogStack_empty();
        board->cells[i].trapped = false;
    }

    // Trapped cells are fixed up to size 6x8
    if (height >= 3)
        Board_get(board, 3, 0)->trapped = true;
    if (width >= 2 && height >= 7)
        Board_get(board, 7, 2)->trapped = true;
    if (width >= 3 && height >= 5)
        Board_get(board, 5, 3)->trapped = true;
    if (width >= 4 && height >= 6)
        Board_get(board, 6, 4)->trapped = true;
    if (width >= 5 && height >= 4)
        Board_get(board, 4, 5)->trapped = true;
    if (width >= 6 && height >= 8)
        Board_get(board, 8, 6)->trapped = true;

    return board;
}

BoardCell* Board_get(Board* board, size_t x, size_t y) {
    assert(x < board->width && y < board->height && "out of bounds");
    return &board->cells[x + y * board->height];
}

void Board_destroy(Board* board) {
    size_t cell_count = board->width * board->height;
    for (size_t i = 0; i < cell_count; i++) {
        HedgehogStack_destroy(&board->cells[i].stack);
    }
}
