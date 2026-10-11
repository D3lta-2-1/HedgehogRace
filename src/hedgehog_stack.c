#include <assert.h>
#include <hedgehog_stack.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SIZE 4
#define GROWTH_FACTOR 2

HedgehogStack HedgehogStack_empty() {
    HedgehogStack stack = {NULL, 0, 0};
    return stack;
}

static void grow(HedgehogStack* stack) {
    if (stack->capacity == 0) {
        stack->player_i = malloc(sizeof(int) * DEFAULT_SIZE);
        stack->capacity = DEFAULT_SIZE;
    } else {
        stack->capacity *= GROWTH_FACTOR;
        stack->player_i = realloc(stack->player_i, stack->capacity);
    }
    if (!stack->player_i) {
        fprintf(stderr, "stack growth failed");
        abort();
    }
}

void HedgehogStack_push(HedgehogStack* stack, Owner owner) {
    if (stack->len == stack->capacity) {
        grow(stack);
    }
    stack->player_i[stack->len] = owner;
    stack->len++;
}

Owner HedgehogStack_pop(HedgehogStack* stack) {
    assert(stack->len > 0 && "cannot pop on an empty stack");
    int player_i = stack->player_i[stack->len - 1];
    stack->len--;
    return player_i;
}

Owner HedgehogStack_peek(HedgehogStack* stack) { return HedgehogStack_peek_nth(stack, 0); }

Owner HedgehogStack_peek_nth(HedgehogStack* stack, size_t nth) {
    assert(stack->len > 0 && nth < stack->len);
    return stack->player_i[stack->len - 1 - nth];
}

bool HedgehogStack_is_empty(HedgehogStack* stack) { return stack->len == 0; }

void HedgehogStack_destroy(HedgehogStack* stack) {
    if (stack->player_i)
        return;
    free(stack->player_i);
}
