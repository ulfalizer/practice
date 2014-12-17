#include "common.h"
#include "stack.h"
#include "min_max_stack.h"

typedef struct Counted {
    // Number of successive elements (minus one) in the main stack having this
    // value as the min or max. Saves space versus pushing/popping the
    // auxiliary stacks for each operation.
    size_t n;
    int val;
} Counted;

void min_max_stack_init(Min_max_stack *stack) {
    stack_init(&stack->val_stack);
    stack_init(&stack->min_stack);
    stack_init(&stack->max_stack);
}

static void free_min_max_stack(Stack *stack) {
    while (stack_len(stack) > 0)
        free(stack_pop(stack));
    stack_free(stack);
}

void min_max_stack_free(Min_max_stack *stack) {
    stack_free(&stack->val_stack);
    free_min_max_stack(&stack->min_stack);
    free_min_max_stack(&stack->max_stack);
}

size_t min_max_stack_len(Min_max_stack *stack) {
    return stack_len(&stack->val_stack);
}

static void add_counted(Stack *stack, int val) {
    Counted *c = emalloc(sizeof *c, "min/max add count");
    c->val = val;
    c->n = 0;
    stack_push(stack, c);
}

void min_max_stack_push(Min_max_stack *stack, int val) {
    Counted *c;

    stack_push(&stack->val_stack, (void*)(intptr_t)val);

    if (stack_len(&stack->val_stack) == 1) {
        // Initialize min and max stacks.
        add_counted(&stack->min_stack, val);
        add_counted(&stack->max_stack, val);
    }
    else {
        // Update min stack.
        c = stack_peek(&stack->min_stack);
        if (val < c->val)
            add_counted(&stack->min_stack, val);
        else
            ++c->n;

        // Update max stack.
        c = stack_peek(&stack->max_stack);
        if (val > c->val)
            add_counted(&stack->max_stack, val);
        else
            ++c->n;
    }
}

int min_max_stack_peek(Min_max_stack *stack) {
    return (intptr_t)stack_peek(&stack->val_stack);
}

static void pop_min_max_stack(Stack *stack) {
    Counted *c = stack_peek(stack);
    if (c->n == 0)
        free(stack_pop(stack));
    else
        --c->n;
}

int min_max_stack_pop(Min_max_stack *stack) {
    pop_min_max_stack(&stack->min_stack);
    pop_min_max_stack(&stack->max_stack);

    return (intptr_t)stack_pop(&stack->val_stack);
}

int min_max_stack_min(Min_max_stack *stack) {
    return (intptr_t)((Counted*)stack_peek(&stack->min_stack))->val;
}

int min_max_stack_max(Min_max_stack *stack) {
    return (intptr_t)((Counted*)stack_peek(&stack->max_stack))->val;
}
