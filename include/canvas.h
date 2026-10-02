#ifndef CANVAS_H
#define CANVAS_H

#include <stddef.h>
#include <stdio.h>

/// A buffer where to draw before display to the screen
// the internal storage need a bit of care, '\n' and '\0' need to be placed at
// some places
typedef struct {
  char *content;
  // this is the "inner size", since ext '\n' are required, len_x + 1 is often
  // used
  size_t len_x;
  size_t len_y;
} Canvas;

Canvas Canvas_empty(size_t len_x, size_t len_y);
void Canvas_clear(Canvas *canvas);
void Canvas_set(Canvas *canvas, size_t x, size_t y, char c);
void canvas_flush(Canvas *canvas, FILE *file);
void Canvas_destroy(Canvas *canvas);

#endif
