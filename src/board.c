#include "canvas.h"
#include "hedgehog_stack.h"
#include <assert.h>
#include <board.h>
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
    printf("player: %i\n", 'A' + player);
    char motif[] = {'A' + player, '\0'};
    Canvas_draw_line(canvas, x + 1, y + 1, x + 3, y + 1, motif);
  }
}
