#include <assert.h>
#include <game.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Board Board_init(size_t width, size_t height) {
    Board board;
    board.cells = (BoardCell*)malloc(sizeof(BoardCell) * width * height);
    if (!board.cells) {
        fprintf(stderr, "Board_new BoardCell* malloc failed");
        abort();
    }
    board.width = width;
    board.height = height;
    for (size_t i = 0; i < width * height; i++) {
        board.cells[i].stack = HedgehogStack_empty();
        board.cells[i].trapped = false;
    }

    // Trapped cells are fixed up to size 6x8
    if (height >= 3)
        Board_get(&board, 3, 0)->trapped = true;
    if (width >= 2 && height >= 7)
        Board_get(&board, 7, 2)->trapped = true;
    if (width >= 3 && height >= 5)
        Board_get(&board, 5, 3)->trapped = true;
    if (width >= 4 && height >= 6)
        Board_get(&board, 6, 4)->trapped = true;
    if (width >= 5 && height >= 4)
        Board_get(&board, 4, 5)->trapped = true;
    if (width >= 6 && height >= 8)
        Board_get(&board, 8, 6)->trapped = true;

    return board;
}

BoardCell* Board_get(Board* board, size_t x, size_t y) {
    assert(x < board->width && y < board->height && "out of bounds");
    return &board->cells[x + y * board->height];
}

void Board_destroy(Board* board) {
    size_t cell_count = board->width * board->height;
    for (size_t i = 0; i < cell_count; i++) {
        HedgehogStack_destroy(&board->cells[i].stack);
    }
}

Game Game_init(int player_count, int n_lines, int n_columns) {
    assert(player_count <= MAX_PLAYER_COUNT && "cannot have more than 26 players");
    Game game;
    game.board = Board_init(n_columns, n_lines);
    game.finished_hedgehogs_counts = malloc(player_count * sizeof(size_t));
    if (!game.finished_hedgehogs_counts) {
        fprintf(stderr, "allocation failed");
        abort();
    }
    for (int i = 0; i < player_count; i++) {
        game.finished_hedgehogs_counts[i] = 0;
    }
    for (int i = 0; i < player_count * HEDGEHOG_COUNT; i++) {
        int line = rand() % n_lines;
        bool not_allocated = true;
        Owner player;
        while (not_allocated) {
            player = rand() % player_count;
            if (game.finished_hedgehogs_counts[player] < HEDGEHOG_COUNT) {
                BoardCell* cell = Board_get(&game.board, START, line);
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
    return game;
}

bool Game_can_move(Game* game, int player, int line, int column) {
    if (column == game->board.width - 1)
        return false;
    // we know there is at least one hedgehog in the stack
    int tophog = HedgehogStack_peek(&Board_get(&game->board, column, line)->stack);
    bool test_1 = player == tophog;
    if (Board_get(&game->board, column, line)->trapped) {
        for (int i = 0; i < column; i++) {
            BoardCell* cell = Board_get(&game->board, line, i);
            if (cell->stack.len != 0)
                return false;
        }
    }
    return test_1;
}

bool Game_can_player_move_vertically(Game* game) {
    for (int lines = 0; lines < game->board.height; lines++) {
        for (int columns = 0; columns < game->board.width - 1; columns++) {
            if (Game_can_move(game, game->current_player, lines, columns))
                return true;
        }
    }
    return false;
}

bool Game_can_player_move_horizontally(Game* game, int line) {
    // hedgehogs on the last line cannot move
    for (int column = 0; column < game->board.width - 2; column++) {
        BoardCell* current = Board_get(&game->board, line, column);
        // If a hedgehog were to be trapped and unable to move then there exists a previous hedgehog that can move
        // Otherwise the trapped hedgehog can move
        if (!HedgehogStack_is_empty(&current->stack))
            return true;
    }
    return false;
}

void Game_move_hedgehog_vertical(Game* game, size_t line, size_t column, int answer) {
    // we map 0 to -1 and 1 to 1;
    int up_down = 2 * answer - 1;
    BoardCell* current = Board_get(&game->board, line, column);
    int tophog = HedgehogStack_pop(&current->stack);
    BoardCell* next = Board_get(&game->board, line + up_down, column);
    HedgehogStack_push(&next->stack, tophog);
    return;
}

void Game_move_hedgehog_horizontal(Game* game, size_t line, size_t column) {
    BoardCell* current = Board_get(&game->board, line, column);
    int tophog = HedgehogStack_pop(&current->stack);
    BoardCell* next = Board_get(&game->board, line, column + 1);
    HedgehogStack_push(&next->stack, tophog);
    return;
}

int Game_check_hedgehog_vertical(Game* game, size_t line, size_t column) {
    BoardCell* cell = Board_get(&game->board, line, column);
    if (HedgehogStack_is_empty(&cell->stack))
        return -1;
    if (Game_can_move(game, game->current_player, line, column))
        return 1;
    return -1;
}

int Game_check_hedgehog_horizontal(Game* game, size_t line, size_t column) {
    BoardCell* cell = Board_get(&game->board, line, column);
    if (HedgehogStack_is_empty(&cell->stack))
        return -1;
    int tophog = HedgehogStack_peek(&cell->stack);
    if (Game_can_move(game, tophog, line, column))
        return 1;
    return -1;
}

void Game_destroy(Game* game) {
    Board_destroy(&game->board);
    free(game->finished_hedgehogs_counts);
}
