#ifndef HEDGEHOG_STACK_H
#define HEDGEHOG_STACK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef size_t Owner;

/// Allows storage of hedgehogs (identified by their player) as a stack
typedef struct {
    Owner* player_i;
    size_t capacity;
    size_t len;
} HedgehogStack;

HedgehogStack HedgehogStack_empty();
void HedgehogStack_push(HedgehogStack* stack, Owner owner);
Owner HedgehogStack_pop(HedgehogStack* stack);
Owner HedgehogStack_peek(HedgehogStack* stack);
Owner HedgehogStack_peek_nth(HedgehogStack* stack, size_t nth);
bool HedgehogStack_is_empty(HedgehogStack* stack);
void HedgehogStack_destroy(HedgehogStack* stack);

#endif
