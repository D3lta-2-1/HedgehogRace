#ifndef BOARD_H
#define BOARD_H

#include "canvas.h"
#include "hedgehog_stack.h"
#include <stdbool.h>
#include <stddef.h>

#define MAX_PLAYER_COUNT 26
#define MAX_HEGDEHOG_COUNT 4

typedef struct {
  HedgehogStack stack;
  bool trapped;
} BoardCell;

typedef struct {
  BoardCell *cells;
  size_t width;
  size_t height;
} Board;

Board Board_new(size_t width, size_t height);

BoardCell *Board_get(Board *board, size_t line, size_t column);
size_t Board_canvas_height(Board *board);
size_t Board_canvas_width(Board *board);
void Board_draw(Board *board, Canvas *canvas);
void Board_destroy(Board *board);
/// draw a cell by it top left corner
void BoardCell_draw_at(BoardCell *cell, Canvas *canvas, size_t x, size_t y);

void Board_push(Board *b, size_t line, size_t column, char ctn);
char Board_pop(Board *b, size_t line, size_t column);

#endif
