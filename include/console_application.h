#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "canvas.h"
#include "game.h"

typedef struct {
    Canvas canvas;
    Game game;
} ConsoleApplication;

typedef enum { VERTICAL_QUESTION = 0, VERTICAL_COORDINATES = 1, UP_DOWN = 2, HORIZONTAL_COORDINATE = 3 } Input_state;

size_t ConsoleApplication_compute_canvas_width(Board* board);
size_t ConsoleApplication_compute_canvas_height(Board* board);

ConsoleApplication ConsoleApplication_init(int player_count, size_t width, size_t height);
void ConsoleApplication_draw(ConsoleApplication* app);
void ConsoleApplication_run(ConsoleApplication* app);
void ConsoleApplication_end(ConsoleApplication* app);
void ConsoleApplication_destroy(ConsoleApplication* app);

#endif
