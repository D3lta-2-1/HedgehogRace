#include <canvas.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Canvas Canvas_empty(size_t len_x, size_t len_y) {
    Canvas canvas = {
        // don't forget the '\0' and the `n`
        malloc(sizeof(char) * (len_x + 1) * len_y + 1),
        len_x,
        len_y
    };
    for(size_t y = 0; y < len_y; y++) {
        memset(canvas.content + y * (len_x + 1) , ' ', len_x);
        canvas.content[y * (len_x + 1) + len_x]= '\n';
    }
    canvas.content[(len_x + 1) * len_y ]= '\0';
    return canvas;
}

void Canvas_clear(Canvas *canvas){
    for(size_t y = 0; y < canvas->len_y; y++) {
        memset(canvas->content + y * (canvas->len_x + 1) , ' ', canvas->len_x);
    }
}

void Canvas_set(Canvas *canvas, size_t x, size_t y, char c) {
    assert(0 <= x && x < canvas->len_x && 0 <= y && y < canvas->len_y && "out of bounds");
    canvas->content[x + y * (canvas->len_x + 1)] = c;
}

void canvas_flush(Canvas *canvas, FILE *file) {
    fprintf(file, "%s", canvas->content);
};

void Canvas_destroy(Canvas *canvas) {
    free(canvas->content);
}
