#include "hedgehog_stack.h"
#include <board.h>
#include <canvas.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char **argv) {
  Board board = Board_new(3, 3);

  size_t width = Board_canvas_width(&board);
  size_t height = Board_canvas_height(&board);
  Canvas canvas = Canvas_empty(width, height);

  Board_get(&board, 1, 0)->trapped = true;

  BoardCell *c1 = Board_get(&board, 0, 0);
  HedgehogStack_push(&c1->stack, 0);
  HedgehogStack_push(&c1->stack, 1);
  BoardCell *c2 = Board_get(&board, 1, 0);

  Board_draw(&board, &canvas);

  canvas_flush(&canvas, stdout);
  fflush(stdout);

  return 0;
}
