#ifndef BOARD_H
#define BOARD_H

#include <hedgehog_stack.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  HedgehogStack stack;
  bool trapped;
} BoardCell;

typedef struct {
  BoardCell *cells;
  size_t len_x;
  size_t len_y;
} Board;

Board Board_new(size_t x, size_t y);
BoardCell *Board_get(Board *board, size_t x, size_t y);
void Board_destroy(Board *board);

#endif
