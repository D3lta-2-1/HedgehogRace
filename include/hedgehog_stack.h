#ifndef CELL_H
#define CELL_H

#include <stdbool.h>
#include <stdint.h>

/// Player are represented by a value starting at 0
typedef uint8_t Player;

/// Comparing Hedgehog and Players make sense, they are equal if this Hedgehog
/// is owned the player.
typedef uint8_t Hedgehog;

/// a stack store a stack of Hedgoge, and implement a growable buffer stack
/// the top element is at hedgeos[stack.len]
typedef struct {
  Hedgehog *hedgeogs;
  size_t capacity;
  size_t len;

} HedgehogStack;

HedgehogStack HedgehogStack_empty();
void HedgehogStack_push(HedgehogStack *stack, Hedgehog hedgehog);
Hedgehog HedgehogStack_pop(HedgehogStack *stack);
Hedgehog HedgehogStack_peek(HedgehogStack *stack);
bool HedgehogStack_is_empty(HedgehogStack *stack);
void HedgehogStack_destroy(HedgehogStack *stack);

#endif
