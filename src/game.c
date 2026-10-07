#include <game.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



Game* init_game(char player_count, char n_lines, char n_columns) {
  assert(player_count <= MAX_PLAYER_COUNT && "cannot have more than 26 players");
  
  Game* g = (Game*)malloc(sizeof(Game));
  assert(g != NULL && "init_game Game* malloc failed");
  
  g->board = Board_init(n_columns, n_lines);
  g->players = (Player*)malloc(player_count * sizeof(Player));

  // here we use finished to count the number of hedgehogs already placed for each player
  for (int i = 0 ; i < player_count ; i++) {
    g->players[i].finished = 0;
  }
  for (int i = 0 ; i < player_count * HEDGEHOG_COUNT; i++) {
    int line = rand()%n_lines;
    bool not_allocated = true;
    Player* player;
    while(not_allocated) {
      int player_i = rand() % player_count;
      player = &g->players[player_i];
      if (player->finished < HEDGEHOG_COUNT) {
        player->hedgehogs[player->finished] = (Hedgehog){i, line, START};
        (void)HedgehogStack_push(&Board_get(g->board, START, line)->stack, &player->hedgehogs[player->finished]);
        player->finished++;
        not_allocated = false;
        printf("%d\n",i);
      }
    }
  }
  for (int i = 0 ; i < player_count ; i++) {
    g->players[i].finished = 0;
  }
  return g;
}


/// returns true if current_player's hedgehog number hedgehog_index can move
bool can_move(Game *g, int hedgehog_index) {
  Hedgehog* local = &g->players[g->current_player].hedgehogs[hedgehog_index];
  Hedgehog* other = HedgehogStack_peek(&Board_get(g->board, local->column, local->line)->stack);
  bool test_1 = local->id == other->id;
  if (Board_get(g->board, local->column, local->line)->trapped) {
    for (int column = 0 ; column < local->column ; column++) {
      BoardCell* cell = Board_get(g->board, local->line, column);
      if (cell->stack.len != 0) return false;
    }
  }
  return test_1;
}


HedgehogStack HedgehogStack_empty() {
  HedgehogStack stack = {NULL, 0, 0};
  return stack;
}


void Cell_grow(HedgehogStack *stack) {
  if (stack->capacity == 0) {
    stack->hedgehogs = malloc(sizeof(Hedgehog*) * DEFAULT_SIZE);
    stack->capacity = DEFAULT_SIZE;
  } else {
    stack->capacity *= GROWTH_FACTOR;
    stack->hedgehogs = realloc(stack->hedgehogs, stack->capacity);
  }
}


void HedgehogStack_push(HedgehogStack *stack, Hedgehog* hedgehog) {
  if (stack->len == stack->capacity) {
    Cell_grow(stack);
  }
  stack->hedgehogs[stack->len] = hedgehog;
  stack->len++;
}

Hedgehog* HedgehogStack_pop(HedgehogStack *stack) {
  assert(stack->len > 0 && "cannot pop on an empty stack");
  Hedgehog* hedgehog = stack->hedgehogs[stack->len - 1];
  stack->len--;
  return hedgehog;
}

Hedgehog *HedgehogStack_peek(HedgehogStack *stack) {
  assert(stack->len > 0 && "cannot pop on an empty stack");
  return stack->hedgehogs[stack->len - 1];
}

bool HedgehogStack_is_empty(HedgehogStack *stack) { return stack->len == 0; }

void HedgehogStack_destroy(HedgehogStack *stack) {
  if (stack->hedgehogs)
    return;
  free(stack->hedgehogs);
}


Board* Board_init(size_t width, size_t height) {
  Board* board = (Board*)malloc(sizeof(Board));
  assert(board != NULL && "Board_new Board* malloc failed");
  board->cells = (BoardCell*)malloc(sizeof(BoardCell) * width * height);
  assert(board->cells != NULL && "Board_new BoardCell* malloc failed");
  board->width = width;
  board->height = height;
  for (size_t i = 0; i < width * height; i++) {
    board->cells[i].stack = HedgehogStack_empty();
    board->cells[i].trapped = false;
  }

  // Trapped cells are fixed up to size 6x8
  if (height >= 3)
    Board_get(board, 3, 0)->trapped = true;
  if (width >= 2 && height >= 7)
    Board_get(board, 7, 2)->trapped = true;
  if (width >= 3 && height >= 5)
    Board_get(board, 5, 3)->trapped = true;
  if (width >= 4 && height >= 6)
    Board_get(board, 6, 4)->trapped = true;
  if (width >= 5 && height >= 4)
    Board_get(board, 4, 5)->trapped = true;
  if (width >= 6 && height >= 8)
    Board_get(board, 8, 6)->trapped = true;

  return board;
}

BoardCell *Board_get(Board *board, size_t x, size_t y) {
  assert(x < board->width && y < board->height && "out of bounds");
  return &board->cells[x + y * board->height];
}



void Board_destroy(Board *board) {
  size_t cell_count = board->width * board->height;
  for (size_t i = 0; i < cell_count; i++) {
    HedgehogStack_destroy(&board->cells[i].stack);
  }
}
