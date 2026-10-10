#include <canvas.h>
#include <game.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int dice(int k) { return rand() % k; }

int main(int argc, char** argv) {
    srand(time(NULL));
    /*
    if (argc < 4) return 1;
    char player_count = *argv[1];
    char n_lines = *argv[2];
    char n_columns = *argv[3];
    // convert to int ?
    */
    int player_count = 2;
    int n_lines = 6;
    int n_columns = 9;

    Board* board = init_game(player_count, n_lines, n_columns);

    size_t width = Board_canvas_width(board);
    size_t height = Board_canvas_height(board);
    Canvas canvas = Canvas_empty(width, height);


    char answers[256];
    bool running = true;
    while (running) {
        Board_draw(board, &canvas);
        canvas_flush(&canvas, stdout);
        fflush(stdout);

        int dice_line = dice(n_lines);
        printf("Player %d, the dice rolled a %d !\n", current_player, dice_line+1);
        if (player_can_move_vertical(board)) {
            // 0 -> Do you want to move a hedgehog vertically ?
            if (get_input(g, 0, answers)) {
                // 1 -> Which of your hedgehogs would you like to move ? 
                get_input(board, 1, answers)
                int line = (int)(answers[0] - '1');
                int column = (int)(answers[1] - 'a');
                // 2 -> Where would you like to move it ?
                move_hedgehog_vertical(board, line, column, get_input(g, 2, answers));
            }
        } else {
            printf("Player %d, you cannot move any hedgehogs vertically\n", current_player);
        }
        if (player_can_move_horizontal(board, dice_line)) {
            // 3 Which hedgehog would you like to move ?
            answers[0] = (char)dice_line;
            int column = get_input(board, 3, answers);
            move_hedgehog_horizontal(board, dice_line, column);
        } else {
            printf("Player %d, there are no movable hedgehogs on line %d, your turn is skipped\n", current_player, dice_line);
        }
    }
    return 0;
}
