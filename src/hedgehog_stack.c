#include <assert.h>
#include <hedgehog_stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 4
#define GROWTH_FACTOR 2

HedgehogStack HedgehogStack_empty() {
    HedgehogStack stack = {NULL, 0, 0};
    return stack;
}

static void grow(HedgehogStack* stack) {
    if (stack->capacity == 0) {
        stack->hedgehogs = malloc(sizeof(Owner) * DEFAULT_SIZE);
        stack->capacity = DEFAULT_SIZE;
    } else {
        stack->capacity *= GROWTH_FACTOR;
        Owner* new_buffer = malloc(stack->capacity * sizeof(Owner));
        if (!new_buffer) {
            fprintf(stderr, "stack growth failed");
            abort();
        }
        memcpy(new_buffer, stack->hedgehogs, stack->len * sizeof(Owner));
        free(stack->hedgehogs);
        stack->hedgehogs = new_buffer;
    }
}

void HedgehogStack_push(HedgehogStack* stack, Owner owner) {
    if (stack->len == stack->capacity) {
        grow(stack);
    }
    stack->hedgehogs[stack->len] = owner;
    stack->len++;
}

Owner HedgehogStack_pop(HedgehogStack* stack) {
    assert(stack->len > 0 && "cannot pop on an empty stack");
    Owner player_i = stack->hedgehogs[stack->len - 1];
    stack->len--;
    return player_i;
}

Owner HedgehogStack_peek(HedgehogStack* stack) { return HedgehogStack_peek_nth(stack, 0); }

Owner HedgehogStack_peek_nth(HedgehogStack* stack, size_t nth) {
    assert(stack->len > 0 && nth < stack->len);
    return stack->hedgehogs[stack->len - 1 - nth];
}

bool HedgehogStack_is_empty(HedgehogStack* stack) { return stack->len == 0; }

void HedgehogStack_destroy(HedgehogStack* stack) {
    if (stack->hedgehogs)
        return;
    free(stack->hedgehogs);
}
