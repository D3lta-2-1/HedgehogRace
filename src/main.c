#include <canvas.h>
#include <game.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int de() { return rand() % 6 + 1; }

int main(int argc, char** argv) {
    srand(time(NULL));
    /*
    if (argc < 4) return 1;
    char player_count = *argv[1];
    char n_lines = *argv[2];
    char n_columns = *argv[3];
    */
    char player_count = 2;
    char n_lines = 6;
    char n_columns = 9;

    Game* game = init_game(player_count, n_lines, n_columns);

    size_t width = Board_canvas_width(game->board);
    size_t height = Board_canvas_height(game->board);
    Canvas canvas = Canvas_empty(width, height);

    char answers[64];
    bool running = true;
    while (running) {
        Board_draw(game->board, &canvas);
        canvas_flush(&canvas, stdout);
        fflush(stdout);

        bool player_can_move = false;
        for (int i = 0; i < HEDGEHOG_COUNT; i++) {
            player_can_move = player_can_move || can_move(game, i);
        }
        if (player_can_move) {
            if (get_input(0)) {
                
            }
            

        }
    return 0;
}
