#include <canvas.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

Canvas Canvas_empty(size_t width, size_t height) {
  Canvas canvas = {// don't forget the '\0' and the `n`
                   malloc(sizeof(char) * (width + 1) * height + 1), width,
                   height};
  for (size_t y = 0; y < height; y++) {
    memset(canvas.content + y * (width + 1), ' ', width);
    canvas.content[y * (width + 1) + width] = '\n';
  }
  canvas.content[(width + 1) * height] = '\0';
  return canvas;
}

void Canvas_clear(Canvas *canvas) {
  for (size_t y = 0; y < canvas->height; y++) {
    memset(canvas->content + y * (canvas->width + 1), ' ', canvas->width);
  }
}

static void ensure_within_range(Canvas *canvas, size_t x, size_t y) {
  assert(0 <= x && x < canvas->width && 0 <= y && y < canvas->height &&
         "out of bounds");
}

void Canvas_set(Canvas *canvas, size_t x, size_t y, char c) {
  assert(c >= ' ' && "invalid character");
  ensure_within_range(canvas, x, y);
  canvas->content[x + y * (canvas->width + 1)] = c;
}

static ptrdiff_t sign(ptrdiff_t x) { return x > 0 ? 1 : (x < 0 ? -1 : 0); }

void Canvas_draw_line(Canvas *canvas, size_t x1, size_t y1, size_t x2,
                      size_t y2, char *motif) {
  assert(*motif != '\0' && "the motif can't be empty");
  char *motif_iter = motif;
  const ptrdiff_t dx = x2 - x1;
  const ptrdiff_t dy = y2 - y1;
  assert((dx == 0 || dy == 0) && "drawing diagonals is unimplemented");
  ensure_within_range(canvas, x1, y1);
  ensure_within_range(canvas, x2, y2);

  if (dx && !dy) {
    ptrdiff_t unit = sign(dx);
    // the second point is included
    for (size_t x = x1; x != x2 + unit; x += unit) {
      Canvas_set(canvas, x, y1, *motif_iter);
      motif_iter = (*++motif_iter == '\0') ? motif : motif_iter;
    }
  } else if (!dx && dy) {
    ptrdiff_t unit = sign(dy);
    // the second point is included
    for (size_t y = y1; y != y2 + unit; y += unit) {
      Canvas_set(canvas, x1, y, *motif_iter);
      motif_iter = (*++motif_iter == '\0') ? motif : motif_iter;
    }
  }
}

void canvas_flush(Canvas *canvas, FILE *file) {
  fprintf(file, "%s", canvas->content);
};

void Canvas_destroy(Canvas *canvas) { free(canvas->content); }

#define MARGIN_TOP 2
#define MARGIN_LEFT 5
#define HORIZONTAL_SPACING 1
#define VERTICAL_SPACING 0
#define CELL_DISPLAY_WIDTH 5
#define CELL_DISPLAY_HEIGHT 4

size_t Board_canvas_width(Board *board) {
  return MARGIN_LEFT +
         board->width * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING) -
         HORIZONTAL_SPACING;
}

size_t Board_canvas_height(Board *board) {
  return MARGIN_TOP + board->height * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING) -
         VERTICAL_SPACING;
}

static void Board_draw_decoration(Board *board, Canvas *canvas) {
  // top decorations
  for (size_t x = 0; x < board->width; x++) {
    size_t xorigin =
        MARGIN_LEFT + x * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING);
    Canvas_draw_line(canvas, xorigin + 1, 0, xorigin + 3, 0, "row");
    Canvas_set(canvas, xorigin + 2, 1, 'a' + x);
  }
  // left decorations
  for (size_t y = 0; y < board->height; y++) {
    size_t yorigin = MARGIN_TOP + y * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING);
    Canvas_draw_line(canvas, 0, yorigin + 1, 3, yorigin + 1, "line");
    // writing the value
    assert(y < 10000);
    char buffer[5] = {' ', ' ', ' ', ' ', '\0'};
    size_t n = sprintf(buffer, "%zu", y + 1);
    if (n < 4)
      buffer[n] = ' ';
    Canvas_draw_line(canvas, 0, yorigin + 2, 3, yorigin + 2, buffer);
  }
}

static void Board_draw_cells(Board *board, Canvas *canvas) {
  for (size_t x = 0; x < board->width; x++) {
    for (size_t y = 0; y < board->height; y++) {
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

/*
void BoardCell_draw_at(BoardCell *cell, Canvas *canvas, size_t x, size_t y) {
  Canvas_draw_line(canvas, x + 1, y, x + 3, y, cell->trapped ? "v" : "-");
  Canvas_draw_line(canvas, x + 1, y + 3, x + 3, y + 3,
  cell->trapped ? "^" : "-");
  Canvas_draw_line(canvas, x, y + 1, x, y + 2, cell->trapped ? ">" : "|");
  Canvas_draw_line(canvas, x + 4, y + 1, x + 4, y + 2,
  cell->trapped ? "<" : "|");
  if (!HedgehogStack_is_empty(&cell->stack)) {
    Hedgehog *h = HedgehogStack_peek(&cell->stack);
    char motif[] = {'A' + h->id, '\0'}; // id is the player id ?
    Canvas_draw_line(canvas, x + 1, y + 1, x + 3, y + 1, motif);
  }
}

*/