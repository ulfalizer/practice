#include "common.h"
#include "stack.h"

#define INITIAL_BUF_LEN 16

void stack_init(Stack *stack) {
    stack->buf = emalloc(sizeof(*stack->buf)*INITIAL_BUF_LEN, "stack init");
    stack->buf_len = INITIAL_BUF_LEN;
    stack->len = 0;
}

void stack_free(Stack *stack) {
    free(stack->buf);
}

size_t stack_len(Stack *stack) {
    return stack->len;
}

static void grow(Stack *stack) {
    stack->buf_len *= 2;
    stack->buf = erealloc(stack->buf, sizeof(*stack->buf)*stack->buf_len,
      "stack grow");
}

void stack_push(Stack *stack, void *val) {
    if (stack->len == stack->buf_len)
        grow(stack);
    stack->buf[stack->len++] = val;
}

void *stack_peek(Stack *stack) {
    assert(stack->len > 0);
    return stack->buf[stack->len - 1];
}

void *stack_pop(Stack *stack) {
    assert(stack->len > 0);
    return stack->buf[--stack->len];
}
