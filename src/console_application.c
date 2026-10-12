#include "canvas.h"
#include "game.h"
#include <assert.h>
#include <console_application.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
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
        char buffer[64] = {0};
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
    char top_motif[] = {'A' + owner, '\0'};
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
        *i = tolower(*i);
    }
}

static Result both_not_error(Result r1, Result r2) {
    if (r1 != ERROR && r2 != ERROR)
        return OPTION_1;
    else
        return ERROR;
}
/// returns OPTION_1 if string = string_1, OPTION_2 if string = string_2 and ERROR otherwise
static Result test_binary_string(char* string, char* string_1, char* string_2) {
    if (!strcmp(string, string_1)) {
        return OPTION_1;
    } else if (!strcmp(string, string_2)) {
        return OPTION_2;
    } else
        return ERROR;
}

static Result check_coordinates_y(Board* b, int y) {
    if (y >= 0 && y < b->height) {
        return y;
    } else {
        return ERROR;
    }
}

static Result check_coordinates_x(Board* b, int x) {
    if (x >= 0 && x < b->width) {
        return x;
    } else {
        return ERROR;
    }
}

static Result test_vertical_move(Board* b, Result res, int y) {
    if (y == 0 && res == OPTION_1)
        return ERROR;
    else if (y == b->height - 1 && res == OPTION_2)
        return ERROR;
    else
        return res;
}

/// manages all of the players answers
/// 0 -> Do you want to move a hedgehog vertically ?
/// 1 -> Which of your hedgehogs would you like to move ?
/// 2 -> Where would you like to move it ?
/// 3 -> which hedgehog would you like to move ?
static int get_input(Game* game, Input_state input, char* answers) {
    static int y_static;
    int y_1;
    int x_1;
    int y_3;
    int x_3;
    char* yes = "yes";
    char* no = "no";
    char* up = "up";
    char* down = "down";
    Result res = ERROR;
    printf("Player %c, ", (char)game->current_player + 'A');
    while (res == ERROR) {
        // Player input
        switch (input) {
        case VERTICAL_QUESTION:
            printf("do you want to move a hedgehog vertically ?\n");
            scanf("%s", answers);
            to_lower_str(answers);
            break;
        case VERTICAL_COORDINATES:
            printf("which of your hedgehogs would you like to move ? (format:a1)\n");
            scanf("%s", answers);
            answers[0] = tolower(answers[0]);
            break;
        case UP_DOWN:
            printf("where would you like to move it ? (up or down)\n");
            scanf("%s", answers);
            to_lower_str(answers);
            break;
        case HORIZONTAL_COORDINATE:
            y_3 = (int)answers[0];
            printf("which hedgehog would you like to move ? (format : a)\n");
            scanf("%s", answers);
            answers[0] = tolower(answers[0]);
        default:
            break;
        }
        // Verification and player help
        switch (input) {
        case VERTICAL_QUESTION:
            res = test_binary_string(answers, yes, no);
            break;
        case VERTICAL_COORDINATES:
            y_1 = (int)(answers[1] - '1');
            x_1 = (int)(answers[0] - 'a');
            res = both_not_error(check_coordinates_y(&game->board, y_1), check_coordinates_x(&game->board, x_1));
            if (res != ERROR) {
                res = Game_check_hedgehog_vertical(game, x_1, y_1);
                if (res == ERROR) {
                    printf("Either that isn't a/your hedgehog or it cannot move\nTo be able to move, a hedgehog must either be at the top of a cell or, if it is trapped, the fist on that line, hence "
                           ": ");
                }
            } else {
                printf("Coordinates are format : Column letter Line number, example c2, hence : ");
            }
            break;
        case UP_DOWN:
            res = test_binary_string(answers, up, down);
            if (res != ERROR) {
                res = test_vertical_move(&game->board, res, y_static);
                if (res == ERROR) {
                    if (y_static == 0) {
                        printf("This hedgehog cannot move up, hence : ");
                    } else {
                        printf("This hedgehog cannot move down, hence : ");
                    }
                }
            }
            break;
        case HORIZONTAL_COORDINATE:
            x_3 = (int)(answers[0] - 'a');
            res = check_coordinates_x(&game->board, x_3);
            if (y_3 != ERROR) {
                res = Game_check_hedgehog_horizontal(game, x_3, y_3);
                if (res == ERROR) {
                    printf("Either that isn't a hedgehog or it cannot move\nTo be able to move, a hedgehog must either be at the top of a cell or, if it is trapped, the fist on that line, hence : ");
                }
            } else {
                printf("Please enter column letter, hence : ");
            }
            break;
        }
        if (res == ERROR)
            printf("Invalid entry\n So, ");
    }
    // Returns and data management
    switch (input) {
    case VERTICAL_QUESTION:
        if (res == OPTION_1)
            return 1;
        else
            return 0;
    case VERTICAL_COORDINATES:
        y_static = (int)answers[1] - '1';
        return 0;
    case UP_DOWN:
        if (res == OPTION_1)
            return -1;
        else
            return 1;
    case HORIZONTAL_COORDINATE:
        return x_3;
    default:
        return 0;
    }
}

ConsoleApplication ConsoleApplication_init(int player_count, size_t width, size_t height) {
    srand(time(NULL));

    ConsoleApplication app;
    app.game = Game_init(player_count, width, height);
    size_t canvas_width = compute_canvas_width(&app.game.board);
    size_t canvas_height = compute_canvas_height(&app.game.board);
    app.canvas = Canvas_empty(canvas_width, canvas_height);
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
    bool finished = false;
    while (running) {
        ConsoleApplication_draw(app);

        int dice_y = dice(app->game.board.height);
        printf("Player %c, the dice rolled a %d !\n", (char)app->game.current_player + 'A', dice_y + 1);
        if (Game_can_player_move_vertically(&app->game)) {
            if (get_input(&app->game, VERTICAL_QUESTION, answers)) {
                get_input(&app->game, VERTICAL_COORDINATES, answers);
                int x = (int)(answers[0] - 'a');
                int y = (int)(answers[1] - '1');
                Game_move_hedgehog_vertical(&app->game, x, y, get_input(&app->game, UP_DOWN, answers));
            }
        } else {
            printf("Player %c, you cannot move any hedgehogs vertically\n", (char)app->game.current_player + 'A');
        }
        if (Game_can_player_move_horizontally(&app->game, dice_y)) {
            answers[0] = (char)dice_y;
            int x = get_input(&app->game, HORIZONTAL_COORDINATE, answers);
            Game_move_hedgehog_horizontal(&app->game, x, dice_y);
            if (x == app->game.board.width - 1)
                app->game.finished_hedgehogs_counts[app->game.current_player]++;
            if (app->game.finished_hedgehogs_counts[app->game.current_player] == HEDGEHOG_COUNT - 1)
                finished = true;
        } else {
            printf("Player %c, there are no movable hedgehogs on line %d, your turn is skipped\n", (char)app->game.current_player + 'A', dice_y + 1);
        }
        if (app->game.current_player == app->game.player_count && finished) {
            ConsoleApplication_end(app);
        } else
            app->game.current_player = (app->game.current_player + 1) % app->game.player_count;
    }
}

static size_t max_under(size_t* tab, int len, size_t cutoff) {
    size_t max = cutoff;
    for (int i = 0; i < len; i++) {
        if (tab[i] > max && tab[i] < cutoff)
            max = tab[i];
        else if (max == cutoff && tab[i] < cutoff)
            max = tab[i];
    }
    return max;
}

void ConsoleApplication_end(ConsoleApplication* app) {
    size_t max = HEDGEHOG_COUNT + 1;
    for (int i = 0; i < app->game.player_count; i++) {
        bool virgule = false;
        printf("- Place #%d : ", i + 1);
        max = max_under(app->game.finished_hedgehogs_counts, app->game.player_count, max);
        for (int j = 0; j < app->game.player_count; j++) {
            if (app->game.finished_hedgehogs_counts[j] == max) {
                if (!virgule) {
                    printf("Équipe %c", (char)j + 'A');
                    virgule = true;
                } else {
                    printf(", équipe %c", (char)j + 'A');
                }
                i++;
            }
        }
        if (max == 1) {
            printf("(avec 1 hérisson)\n");
        } else if (max == 0) {
            printf("(avec aucun hérisson)\n");
        } else {
            printf("(avec %zu hérissons)\n", max);
        }
    }
}
void ConsoleApplication_destroy(ConsoleApplication* app) {
    Game_destroy(&app->game);
    Canvas_destroy(&app->canvas);
}
