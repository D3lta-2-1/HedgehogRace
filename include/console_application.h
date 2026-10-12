#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "canvas.h"
#include "game.h"

typedef struct {
    Canvas canvas;
    Game game;
} ConsoleApplication;

ConsoleApplication ConsoleApplication_init(int player_count, size_t width, size_t height);
void ConsoleApplication_draw(ConsoleApplication* app);
void ConsoleApplication_run(ConsoleApplication* app);
void ConsoleApplication_end(ConsoleApplication* app);
void ConsoleApplication_destroy(ConsoleApplication* app);

#endif
