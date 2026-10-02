#include "hedgehog_stack.h"
#include <board.h>
#include <canvas.h>
#include <stdio.h>

int main(int argc, char **argv) {
  Canvas canvas = Canvas_empty(20, 10);
  Board board = Board_new(3, 3);
  Board_get(&board, 1, 0)->trapped = true;

  BoardCell *c1 = Board_get(&board, 0, 0);
  HedgehogStack_push(&c1->stack, 0);
  HedgehogStack_push(&c1->stack, 1);
  BoardCell *c2 = Board_get(&board, 1, 0);

  BoardCell_draw_at(c1, &canvas, 0, 0);
  BoardCell_draw_at(c2, &canvas, 6, 0);

  canvas_flush(&canvas, stdout);
  fflush(stdout);

  return 0;
}
