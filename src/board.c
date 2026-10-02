#include "hedgehog_stack.h"
#include <board.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

Board Board_new(size_t x, size_t y) {
    Board board = {
        malloc(sizeof(BoardCell) * x * y),
        x,
        y,
    };
    //TODO: add trapped cells
    for(size_t i = 0; i < x * y; i++) {
        board.cells[i].stack = HedgehogStack_empty();
        board.cells[i].trapped = false;
    }
    return board;
}

BoardCell *Board_get(Board* board, size_t x, size_t y) {
    assert(x < board.len_x && y < board.len_y && "out of bounds");
    return &board->cells[x + y * board->len_x];
}

void Board_destroy(Board *board) {
    size_t cell_count = board->len_x * board->len_y;
    for(size_t i = 0; i < cell_count; i++) {
        HedgehogStack_destroy(&board->cells[i].stack);
    }
}
