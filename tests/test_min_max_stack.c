#include "common.h"
#include "stack.h"
#include "min_max_stack.h"

void test_min_max_stack(void) {
    Min_max_stack stack;
    min_max_stack_init(&stack);

    #define VERIFY_PUSH(val, min, max)             \
        min_max_stack_push(&stack, val);           \
        VERIFY(min_max_stack_peek(&stack) == val); \
        VERIFY(min_max_stack_min(&stack) == min);  \
        VERIFY(min_max_stack_max(&stack) == max);

    #define VERIFY_POP(val, min, max)              \
        VERIFY(min_max_stack_min(&stack) == min);  \
        VERIFY(min_max_stack_max(&stack) == max);  \
        VERIFY(min_max_stack_peek(&stack) == val); \
        VERIFY(min_max_stack_pop(&stack) == val);

    VERIFY(min_max_stack_len(&stack) == 0);
    VERIFY_PUSH(3,  3, 3);
    VERIFY_PUSH(4,  3, 4);
    VERIFY_PUSH(5,  3, 5);
    VERIFY_PUSH(6,  3, 6);
    VERIFY_PUSH(5,  3, 6);
    VERIFY_PUSH(4,  3, 6);
    VERIFY_PUSH(3,  3, 6);
    VERIFY_PUSH(2,  2, 6);
    VERIFY_PUSH(1,  1, 6);
    VERIFY_PUSH(2,  1, 6);
    VERIFY_PUSH(3,  1, 6);
    VERIFY_PUSH(7,  1, 7);
    VERIFY_PUSH(0,  0, 7);
    VERIFY(min_max_stack_len(&stack) == 13);
    VERIFY_POP(0,  0, 7);
    VERIFY_POP(7,  1, 7);
    VERIFY_POP(3,  1, 6);
    VERIFY_POP(2,  1, 6);
    VERIFY_POP(1,  1, 6);
    VERIFY_POP(2,  2, 6);
    VERIFY_POP(3,  3, 6);
    VERIFY_POP(4,  3, 6);
    VERIFY_POP(5,  3, 6);
    VERIFY_POP(6,  3, 6);
    VERIFY_POP(5,  3, 5);
    VERIFY_POP(4,  3, 4);
    VERIFY_POP(3,  3, 3);
    VERIFY(min_max_stack_len(&stack) == 0);

    #undef VERIFY_PUSH
    #undef VERIFY_POP

    min_max_stack_free(&stack);
}
