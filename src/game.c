#include "game.h"
#include "board.h"
#include "hedgehog_stack.h"

// HINT: doc always use triple /

/// returns true if current_player's hedgehog number hedgehog_index can move
bool can_move(Game *g, int hedgehog_index) {
  Hedgehog current = g->players[g->current_player].hedgehogs[hedgehog_index];
  // using two convention, seriously ?
  BoardCell *cell = Board_get(g->board, current.column, current.line);
  bool test_1 = current.id == HedgehogStack_peek(&cell->stack)->id;
  bool test_2 = cell->trapped;
  return test_1 && (!test_2);
}
