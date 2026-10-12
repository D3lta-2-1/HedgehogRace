#include "canvas.h"
#include <assert.h>
#include <console_application.h>
#include <game.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void test_stack() {
    HedgehogStack stack = HedgehogStack_empty();
    assert(HedgehogStack_is_empty(&stack));
    // using 32 ensure that we grow the internal buffer many times
    for (Owner i = 0; i < 32; i++) {
        HedgehogStack_push(&stack, i);
    }
    assert(stack.len == 32);
    for (size_t i = 0; i < 32; i++) {
        assert(HedgehogStack_peek_nth(&stack, i) == 31 - i);
    }

    for (Owner i = 0; i < 32; i++) {
        assert(HedgehogStack_pop(&stack) == 31 - i);
    }
    assert(HedgehogStack_is_empty(&stack));
}

#define WIDTH 101
#define HEIGHT 31
void test_canvas() {

    Canvas canvas = Canvas_empty(WIDTH, HEIGHT);
    Canvas_draw_line(&canvas, 0, 0, WIDTH - 1, 0, "x-");
    Canvas_draw_line(&canvas, 0, HEIGHT - 1, WIDTH - 1, HEIGHT - 1, "x-");
    Canvas_flush(&canvas, stdout);
    fflush(stdout);
}

void test_game_rendering() {
    ConsoleApplication app = ConsoleApplication_init(4, 10, 5);
    //ConsoleApplication_draw(&app);
}

int main(int argc, char** argv) {
    test_stack();
    test_canvas();
    test_game_rendering();

    printf("all test passed successfully\n");
    return EXIT_SUCCESS;
}
