#include <assert.h>
#include <canvas.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
