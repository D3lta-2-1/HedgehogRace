#include "canvas.h"
#include "game.h"
#include <assert.h>
#include <console_application.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MARGIN_TOP 2
#define MARGIN_LEFT 5
#define HORIZONTAL_SPACING 1
#define VERTICAL_SPACING 0
#define CELL_DISPLAY_WIDTH 5
#define CELL_DISPLAY_HEIGHT 4

static size_t compute_canvas_width(Board* board) { return MARGIN_LEFT + board->width * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING) - HORIZONTAL_SPACING; }

static size_t compute_canvas_height(Board* board) { return MARGIN_TOP + board->height * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING) - VERTICAL_SPACING; }

static void draw_decoration(Board* board, Canvas* canvas) {
    // top decorations
    for (size_t x = 0; x < board->width; x++) {
        size_t xorigin = MARGIN_LEFT + x * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING);
        Canvas_draw_line(canvas, xorigin + 1, 0, xorigin + 3, 0, "row");
        Canvas_set(canvas, xorigin + 2, 1, 'a' + x);
    }
    // left decorations
    for (size_t y = 0; y < board->height; y++) {
        size_t yorigin = MARGIN_TOP + y * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING);
        Canvas_draw_line(canvas, 0, yorigin + 1, 3, yorigin + 1, "line");
        // writing the value
        assert(y < 1000);
        char buffer[5] = {' ', ' ', ' ', ' ', '\0'};
        size_t n = sprintf(buffer, "%zu", y + 1);
        if (n < 4)
            buffer[n] = ' ';
        Canvas_draw_line(canvas, 0, yorigin + 2, 3, yorigin + 2, buffer);
    }
}

static void draw_cell_at(BoardCell* cell, Canvas* canvas, size_t x, size_t y) {
    // draw cell borders
    Canvas_draw_line(canvas, x + 1, y, x + 3, y, cell->trapped ? "v" : "-");
    Canvas_draw_line(canvas, x + 1, y + 3, x + 3, y + 3, cell->trapped ? "^" : "-");
    Canvas_draw_line(canvas, x, y + 1, x, y + 2, cell->trapped ? ">" : "|");
    Canvas_draw_line(canvas, x + 4, y + 1, x + 4, y + 2, cell->trapped ? "<" : "|");
    // draw inner content
    if (HedgehogStack_is_empty(&cell->stack))
        return;
    int owner = HedgehogStack_peek(&cell->stack);
    char top_motif[] = {'A' + owner, '\0'}; // id is the player id ?
    Canvas_draw_line(canvas, x + 1, y + 1, x + 3, y + 1, top_motif);
    const size_t len = cell->stack.len;
    char second_line_motif[4] = "   ";
    switch (len) {
    case 1:
        Canvas_draw_line(canvas, x + 1, y + 2, x + 3, y + 2, top_motif);
        return; // don't print the numbers at the bottom
    case 2:
        second_line_motif[1] = 'a' + HedgehogStack_peek_nth(&cell->stack, 1);
        Canvas_draw_line(canvas, x + 1, y + 2, x + 3, y + 2, second_line_motif);
        break;
    case 3:
        second_line_motif[0] = 'a' + HedgehogStack_peek_nth(&cell->stack, 1);
        second_line_motif[2] = 'a' + HedgehogStack_peek_nth(&cell->stack, 2);
        Canvas_draw_line(canvas, x + 1, y + 2, x + 3, y + 2, second_line_motif);
        break;
    case 4:
        second_line_motif[0] = 'a' + HedgehogStack_peek_nth(&cell->stack, 1);
        second_line_motif[1] = 'a' + HedgehogStack_peek_nth(&cell->stack, 2);
        second_line_motif[2] = 'a' + HedgehogStack_peek_nth(&cell->stack, 3);
        Canvas_draw_line(canvas, x + 1, y + 2, x + 3, y + 2, second_line_motif);
        break;
    }

    char buffer[2] = "X\0";
    if (len < 10) {
        sprintf(buffer, "%zu", len);
    }
    Canvas_set(canvas, x + 2, y + 3, buffer[0]);
}

static void draw_cells(Board* board, Canvas* canvas) {
    for (size_t x = 0; x < board->width; x++) {
        for (size_t y = 0; y < board->height; y++) {
            size_t xorigin = MARGIN_LEFT + x * (CELL_DISPLAY_WIDTH + HORIZONTAL_SPACING);
            size_t yorigin = MARGIN_TOP + y * (CELL_DISPLAY_HEIGHT + VERTICAL_SPACING);
            BoardCell* cell = Board_get(board, x, y);
            draw_cell_at(cell, canvas, xorigin, yorigin);
        }
    }
}

static int dice(int k) { return rand() % k; }

static void to_lower_str(char* s) {
    for (char* i = s; *i != '\0'; i++) {
        *s = tolower(*s);
    }
}

/// test yes no functions return -1 for invalid answers, 0 for no and 1 for yes
static int test_yes_no(char* string) {
    if (!strcmp(string, "yes")) {
        return 1;
    } else if (!strcmp(string, "no")) {
        return 0;
    } else
        return -1;
}

static int test_binary_string(char* string, char* string_1, char* string_0) {
    if (!strcmp(string, string_1)) {
        return 1;
    } else if (!strcmp(string, string_0)) {
        return 0;
    } else
        return -1;
}

static int check_coordinates_line(Board* b, char answer) {
    int line = (int)(answer - '1');
    if (line >= 0 && line < b->height) {
        return line;
    } else {
        return -1;
    }
}

static int check_coordinates_column(Board* b, char answer) {
    int column = (int)(answer - 'a');
    if (column >= 0 && column < b->width) {
        return column;
    } else {
        return -1;
    }
}

static int test_vertical_move(Board* b, int answer, char line) {
    if (line == 0 && answer == 1)
        return -1;
    else if (line == b->height - 1 && answer == 0)
        return -1;
    else
        return answer;
}

/// manages all of the players answers
/// 0 -> Do you want to move a hedgehog vertically ?
/// 1 -> Which of your hedgehogs would you like to move ?
/// 2 -> Where would you like to move it ?
/// 3 -> which hedgehog would you like to move ?
static int get_input(Game* game, int i, char* answers) {
    static char line_static;
    int line_1;
    int column_1;
    int line_3;
    int column_3;
    int res = -1;
    while (res == -1) {
        printf("Player %zu, ", game->current_player);
        // Player input
        switch (i) {
        case 0:
            printf("do you want to move a hedgehog vertically ?\n");
            scanf("%s\n", answers);
            to_lower_str(answers);
            break;
        case 1:
            printf("which of your hedgehogs would you like to move ? (format:1a)\n");
            scanf("%s\n", answers);
            answers[1] = tolower(answers[1]);
            break;
        case 2:
            printf("where would you like to move it ? (up or down)\n");
            scanf("%s\n", answers);
            to_lower_str(answers);
            break;
        case 3:
            line_3 = (int)answers[0];
            printf("which hedgehog would you like to move ? (format : a)\n");
            scanf("%s\n", answers);
            answers[0] = tolower(answers[0]);
        default:
            break;
        }
        // Verification and player help
        switch (i) {
        case 0:
            res = test_binary_string(answers, "yes", "no");
            break;
        case 1:
            line_1 = check_coordinates_line(&game->board, answers[0]);
            column_1 = check_coordinates_column(&game->board, answers[1]);
            if (line_1 != -1 && column_1 != -1) {
                res = Game_check_hedgehog_vertical(game, line_1, column_1);
                if (res == -1) {
                    printf(
                        "Either that isn't a/your hedgehog or it cannot move\nTo be able to move, a hedgehog must either be at the top of a cell or, if it is trapped, the fist on that line, hence :");
                }
            } else {
                printf("Coordinates are format \nLine number Column letter, example 2c, hence :");
            }
            break;
        case 2:
            res = test_binary_string(answers, "up", "down");
            if (res != -1) {
                res = test_vertical_move(&game->board, res, line_static);
                if (res == -1) {
                    if (line_static == 0) {
                        printf("This hedgehog cannot move up, hence : ");
                    } else {
                        printf("This hedgehog cannot move down, hence : ");
                    }
                }
            }
            break;
        case 3:
            column_3 = check_coordinates_column(&game->board, answers[0]);
            if (line_3 != -1 && column_3 != -1) {
                res = Game_check_hedgehog_horizontal(game, line_3, column_3);
                if (res == -1) {
                    printf("Either that isn't a hedgehog or it cannot move\nTo be able to move, a hedgehog must either be at the top of a cell or, if it is trapped, the fist on that line, hence :");
                }
            } else {
                printf("Coordinates are format \nColumn letter, example b, hence :");
            }
            break;
        }
        if (res == -1)
            printf("Invalid entry\n");
    }
    // Returns and data management
    switch (i) {
    case 1:
        line_static = answers[0] - '1';
        return res;
    case 3:
        return column_3;
    default:
        return res;
    }
}

ConsoleApplication ConsoleApplication_init() {
    srand(time(NULL));

    ConsoleApplication app;
    // TODO: make game configurable
    app.game = Game_init(2, 6, 9);
    size_t width = compute_canvas_width(&app.game.board);
    size_t height = compute_canvas_height(&app.game.board);
    app.canvas = Canvas_empty(width, height);
    return app;
}

void ConsoleApplication_draw(ConsoleApplication* app) {
    Canvas_clear(&app->canvas);
    draw_decoration(&app->game.board, &app->canvas);
    draw_cells(&app->game.board, &app->canvas);

    Canvas_flush(&app->canvas, stdout);
    fflush(stdout);
}

void ConsoleApplication_run(ConsoleApplication* app) {
    char answers[256];
    bool running = true;
    while (running) {
        ConsoleApplication_draw(app);

        int dice_line = dice(app->game.board.height);
        printf("Player %zu, the dice rolled a %d !\n", app->game.current_player, dice_line + 1);
        if (Game_can_player_move_vertically(&app->game)) {
            // 0 -> Do you want to move a hedgehog vertically ?
            if (get_input(&app->game, 0, answers)) {
                // 1 -> Which of your hedgehogs would you like to move ?
                get_input(&app->game, 1, answers);
                int line = (int)(answers[0] - '1');
                int column = (int)(answers[1] - 'a');
                // 2 -> Where would you like to move it ?
                Game_move_hedgehog_vertical(&app->game, line, column, get_input(&app->game, 2, answers));
            }
        } else {
            printf("Player %zu, you cannot move any hedgehogs vertically\n", app->game.current_player);
        }
        if (Game_can_player_move_horizontally(&app->game, dice_line)) {
            // 3 Which hedgehog would you like to move ?
            answers[0] = (char)dice_line;
            int column = get_input(&app->game, 3, answers);
            Game_move_hedgehog_horizontal(&app->game, dice_line, column);
        } else {
            printf("Player %zu, there are no movable hedgehogs on line %d, your turn is skipped\n", app->game.current_player, dice_line);
        }
    }
}

void ConsoleApplication_destroy(ConsoleApplication* app) {
    Game_destroy(&app->game);
    Canvas_destroy(&app->canvas);
}
