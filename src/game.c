#include "hedgehog_stack.h"
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
    if (width > 2)
        Board_get(&board, 2, 0)->trapped = true;
    if (height > 1 && width > 6)
        Board_get(&board, 6, 1)->trapped = true;
    if (height > 2 && width > 4)
        Board_get(&board, 4, 2)->trapped = true;
    if (height > 3 && width > 5)
        Board_get(&board, 5, 3)->trapped = true;
    if (height > 4 && width > 3)
        Board_get(&board, 3, 4)->trapped = true;
    if (height > 5 && width > 7)
        Board_get(&board, 7, 5)->trapped = true;

    return board;
}

BoardCell* Board_get(Board* board, size_t x, size_t y) {
    assert(x < board->width && y < board->height && "out of bounds");
    return &board->cells[y + x * board->height];
}

void Board_destroy(Board* board) {
    size_t cell_count = board->width * board->height;
    for (size_t i = 0; i < cell_count; i++) {
        HedgehogStack_destroy(&board->cells[i].stack);
    }
    free(board->cells);
}

Game Game_init(int player_count, int hedgehog_count, size_t width, size_t height) {
    assert(player_count <= MAX_PLAYER_COUNT && "cannot have more than 26 players");
    Game game;
    game.board = Board_init(width, height);
    game.hedgehog_count = hedgehog_count;

    int* placed_hedgehogs = malloc(player_count * sizeof(int));
    if (!placed_hedgehogs) {
        fprintf(stderr, "allocation failed");
        abort();
    }
    for (int i = 0; i < player_count; i++) {
        placed_hedgehogs[i] = 0;
    }
    for (int i = 0; i < player_count * hedgehog_count; i++) {
        int y = rand() % height;
        bool not_allocated = true;
        Owner player;
        while (not_allocated) {
            player = rand() % player_count;
            if (placed_hedgehogs[player] < hedgehog_count) {
                BoardCell* cell = Board_get(&game.board, START, y);
                HedgehogStack_push(&cell->stack, player);
                placed_hedgehogs[player]++;
                not_allocated = false;
            }
        }
    }

    // recycle allocation
    game.finished_hedgehogs_counts = placed_hedgehogs;

    for (int i = 0; i < player_count; i++) {
        game.finished_hedgehogs_counts[i] = 0;
    }
    game.current_player = 0;
    game.player_count = player_count;
    return game;
}

bool Game_can_move(Game* game, Owner player, size_t x, size_t y) {
    if (x == game->board.width - 1)
        return false;
    HedgehogStack* stack = &Board_get(&game->board, x, y)->stack;
    if (HedgehogStack_is_empty(stack))
        return false;
    Owner tophog = HedgehogStack_peek(stack);
    bool test_1 = player == tophog;
    if (Board_get(&game->board, x, y)->trapped) {
        for (int i = 0; i < x; i++) {
            BoardCell* cell = Board_get(&game->board, i, y);
            if (cell->stack.len != 0)
                return false;
        }
    }
    return test_1;
}

bool Game_can_player_move_vertically(Game* game) {
    for (size_t y = 0; y < game->board.height; y++) {
        for (size_t x = 0; x < game->board.width - 1; x++) {
            if (Game_can_move(game, game->current_player, x, y))
                return true;
        }
    }
    return false;
}

bool Game_can_player_move_horizontally(Game* game, size_t y) {
    // hedgehogs on the last line cannot move
    for (int x = 0; x < game->board.width - 1; x++) {
        BoardCell* current = Board_get(&game->board, x, y);
        // If a hedgehog were to be trapped and unable to move then there exists a previous hedgehog that can move
        // Otherwise the trapped hedgehog can move
        if (!HedgehogStack_is_empty(&current->stack))
            return true;
    }
    return false;
}

void Game_move_hedgehog_vertical(Game* game, size_t x, size_t y, int answer) {
    BoardCell* current = Board_get(&game->board, x, y);
    Owner tophog = HedgehogStack_pop(&current->stack);
    BoardCell* next = Board_get(&game->board, x, y + answer);
    HedgehogStack_push(&next->stack, tophog);
    return;
}

void Game_move_hedgehog_horizontal(Game* game, size_t x, size_t y) {
    BoardCell* current = Board_get(&game->board, x, y);
    Owner tophog = HedgehogStack_pop(&current->stack);
    BoardCell* next = Board_get(&game->board, x + 1, y);
    HedgehogStack_push(&next->stack, tophog);
    return;
}

bool Game_check_hedgehog_vertical(Game* game, size_t x, size_t y) {
    BoardCell* cell = Board_get(&game->board, x, y);
    if (HedgehogStack_is_empty(&cell->stack))
        return false;
    return Game_can_move(game, game->current_player, x, y);
}

int Game_check_hedgehog_horizontal(Game* game, size_t x, size_t y) {
    BoardCell* cell = Board_get(&game->board, x, y);
    if (HedgehogStack_is_empty(&cell->stack))
        return -1;
    Owner tophog = HedgehogStack_peek(&cell->stack);
    if (Game_can_move(game, tophog, x, y))
        return tophog;
    return -1;
}

void Game_destroy(Game* game) {
    Board_destroy(&game->board);
    free(game->finished_hedgehogs_counts);
}
