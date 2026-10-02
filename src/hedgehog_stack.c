#include <assert.h>
#include <hedgehog_stack.h>
#include <stdlib.h>

HedgehogStack HedgehogStack_empty() {
    HedgehogStack stack = { NULL, 0, 0 };
    return stack;
}

#define GROWTH_FACTOR 2
#define DEFAULT_SIZE 4

void Cell_grow(HedgehogStack* stack) {
    if(stack->capacity == 0) {
        stack->hedgeogs = malloc(sizeof(Hedgehog) * DEFAULT_SIZE);
        stack->capacity = DEFAULT_SIZE;
    } else {
        stack->capacity *= GROWTH_FACTOR;
        stack->hedgeogs = realloc(stack->hedgeogs, stack->capacity);
    }
}

void HedgehogStack_push(HedgehogStack *stack, Hedgehog hedgehog) {
    if(stack->len == stack->capacity) {
        Cell_grow(stack);
    }
    stack->hedgeogs[stack->len] = hedgehog;
    stack->len++;
}

Hedgehog HedgehogStack_pop(HedgehogStack *stack) {
    assert(stack.len > 0 && "cannot pop on an empty stack");
    Hedgehog hedgehog = stack->hedgeogs[stack->len];
    stack->len--;
    return hedgehog;
}

void HedgehogStack_destroy(HedgehogStack *stack) {
    if(stack->hedgeogs) return;
    free(stack->hedgeogs);
}
