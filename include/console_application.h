#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "canvas.h"
#include "game.h"

typedef struct {
    Canvas canvas;
    Game game;
} ConsoleApplication;

ConsoleApplication ConsoleApplication_init();
void ConsoleApplication_draw(ConsoleApplication* app);
void ConsoleApplication_run(ConsoleApplication* app);
void ConsoleApplication_end(ConsoleApplication* app);
void ConsoleApplication_destroy(ConsoleApplication* app);

#endif
