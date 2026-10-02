#include "canvas.h"
#include "hedgehog_stack.h"
#include <assert.h>
#include <board.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Board Board_new(size_t x, size_t y) {
  Board board = {
      malloc(sizeof(BoardCell) * x * y),
      x,
      y,
  };
  // TODO: add trapped cells
  for (size_t i = 0; i < x * y; i++) {
    board.cells[i].stack = HedgehogStack_empty();
    board.cells[i].trapped = false;
  }
  return board;
}

BoardCell *Board_get(Board *board, size_t x, size_t y) {
  assert(x < board->len_x && y < board->len_y && "out of bounds");
  return &board->cells[x + y * board->len_x];
}

#define MARGIN_TOP 2
#define MARGIN_LEFT 5
#define HORIZONTAL_SPACING 1
#define VERTICAL_SPACING 0
#define CELL_DISPLAY_WIDTH 5
#define CELL_DISPLAY_HEIGHT 4

size_t Board_canvas_width(Board *board) {
  return MARGIN_LEFT +
         board->len_x * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING) -
         HORIZONTAL_SPACING;
}

size_t Board_canvas_height(Board *board) {
  return MARGIN_TOP + board->len_y * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING) -
         VERTICAL_SPACING;
}

static void Board_draw_decoration(Board *board, Canvas *canvas) {
  // top decorations
  for (size_t x = 0; x < board->len_x; x++) {
    size_t xorigin =
        MARGIN_LEFT + x * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING);
    Canvas_draw_line(canvas, xorigin + 1, 0, xorigin + 3, 0, "row");
    Canvas_set(canvas, xorigin + 2, 1, 'a' + x);
  }
  // left decorations
  for (size_t y = 0; y < board->len_y; y++) {
    size_t yorigin = MARGIN_TOP + y * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING);
    Canvas_draw_line(canvas, 0, yorigin + 1, 3, yorigin + 1, "line");
    // writing the value
    assert(y < 10000);
    char buffer[5] = {' ', ' ', ' ', ' ', '\0'};
    size_t n = sprintf(buffer, "%zu", y);
    if (n < 4)
      buffer[n] = ' ';
    Canvas_draw_line(canvas, 0, yorigin + 2, 3, yorigin + 2, buffer);
  }
}

static void Board_draw_cells(Board *board, Canvas *canvas) {
  for (size_t x = 0; x < board->len_x; x++) {
    for (size_t y = 0; y < board->len_y; y++) {
      size_t xorigin =
          MARGIN_LEFT + x * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING);
      size_t yorigin =
          MARGIN_TOP + y * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING);
      BoardCell *cell = Board_get(board, x, y);
      BoardCell_draw_at(cell, canvas, xorigin, yorigin);
    }
  }
}

void Board_draw(Board *board, Canvas *canvas) {
  Board_draw_decoration(board, canvas);
  Board_draw_cells(board, canvas);
}

void Board_destroy(Board *board) {
  size_t cell_count = board->len_x * board->len_y;
  for (size_t i = 0; i < cell_count; i++) {
    HedgehogStack_destroy(&board->cells[i].stack);
  }
}

void BoardCell_draw_at(BoardCell *cell, Canvas *canvas, size_t x, size_t y) {
  Canvas_draw_line(canvas, x + 1, y, x + 3, y, cell->trapped ? "v" : "-");
  Canvas_draw_line(canvas, x + 1, y + 3, x + 3, y + 3,
                   cell->trapped ? "^" : "-");
  Canvas_draw_line(canvas, x, y + 1, x, y + 2, cell->trapped ? ">" : "|");
  Canvas_draw_line(canvas, x + 4, y + 1, x + 4, y + 2,
                   cell->trapped ? "<" : "|");
  if (!HedgehogStack_is_empty(&cell->stack)) {
    char player = HedgehogStack_peek(&cell->stack);
    char motif[] = {'A' + player, '\0'};
    Canvas_draw_line(canvas, x + 1, y + 1, x + 3, y + 1, motif);
  }
}
