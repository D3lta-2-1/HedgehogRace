#include "canvas.h"
#include <assert.h>
#include <console_application.h>
#include <game.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

Game Game_init_test(int player_count, size_t width, size_t height) {
    assert(player_count <= MAX_PLAYER_COUNT && "cannot have more than 26 players");
    Game game;
    game.board = Board_init(width, height);
    game.finished_hedgehogs_counts = malloc(player_count * sizeof(size_t));
    if (!game.finished_hedgehogs_counts) {
        fprintf(stderr, "allocation failed");
        abort();
    }
    for (int i = 0; i < player_count; i++) {
        game.finished_hedgehogs_counts[i] = 0;
    }
    for (int i = 0; i < player_count * HEDGEHOG_COUNT; i++) {
        int y = rand() % height;
        bool not_allocated = true;
        Owner player;
        while (not_allocated) {
            player = rand() % player_count;
            if (game.finished_hedgehogs_counts[player] < HEDGEHOG_COUNT) {
                BoardCell* cell = Board_get(&game.board, START, y);
                HedgehogStack_push(&cell->stack, player);
                game.finished_hedgehogs_counts[player]++;
                not_allocated = false;
            }
        }
    }
    for (int i = 0; i < player_count; i++) {
        game.finished_hedgehogs_counts[i] = 0;
    }
    game.current_player = 0;
    game.player_count = player_count;

    // extra hedgehogs for testing
    BoardCell* cell;
    // up
    cell = Board_get(&game.board, START, 0);
    HedgehogStack_push(&cell->stack, 0);
    // down
    cell = Board_get(&game.board, START, game.board.height - 1);
    HedgehogStack_push(&cell->stack, 0);
    // traps
    cell = Board_get(&game.board, 2, 0);
    HedgehogStack_push(&cell->stack, 0);
    return game;
}

ConsoleApplication ConsoleApplication_init_test(int player_count, size_t width, size_t height) {
    srand(time(NULL));

    ConsoleApplication app;
    app.game = Game_init_test(player_count, width, height);
    size_t canvas_width = ConsoleApplication_compute_canvas_width(&app.game.board);
    size_t canvas_height = ConsoleApplication_compute_canvas_height(&app.game.board);
    app.canvas = Canvas_empty(canvas_width, canvas_height);
    return app;
}

void test_game_rendering() {
    ConsoleApplication app = ConsoleApplication_init(4, 10, 5);
    ConsoleApplication_draw(&app);
}

int main(int argc, char** argv) {
    ConsoleApplication app = ConsoleApplication_init_test(3, 6, 9);
    ConsoleApplication_run(&app);
    ConsoleApplication_destroy(&app);

    printf("all test passed successfully\n");
    return EXIT_SUCCESS;
}
