#include "board.h"
#include "game.h"
#include "hedgehog_stack.c"

//returns true if current_player's hedgehog number hedgehog_index can move
bool can_move(Game* g, int hedgehog_index) {
    Hedgehog current = g->players[g->current_player].hedgehogs[hedgehog_index];
    bool test_1 = current.id == HedgehogStack_peek(g->board->cells[cell(current.line, current.column, g->board)]).id;
    bool test_2 = g->board->cells[cell(current.line, current.column, g->board)].trapped;
    return test_1 && (!test_2);
}
