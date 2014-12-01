// A dynamically-resized stack with O(1) min/max functions. Tries to be
// reasonably space efficient by only storing a new element into the min/max
// stacks whenever the min/max changes.

typedef struct Min_max_stack {
    Stack val_stack;
    Stack min_stack;
    Stack max_stack;
} Min_max_stack;

void min_max_stack_init(Min_max_stack *stack);
void min_max_stack_free(Min_max_stack *stack);
// Returns the number of elements stored in 'stack'.
size_t min_max_stack_len(Min_max_stack *stack);
// Pushes a new element onto 'stack'.
void min_max_stack_push(Min_max_stack *stack, int val);
// Returns the top element of 'stack' without modifying 'stack'.
int min_max_stack_peek(Min_max_stack *stack);
// Pops the top element off 'stack'.
int min_max_stack_pop(Min_max_stack *stack);
// Returns the maximum element in the stack.
int min_max_stack_min(Min_max_stack *stack);
// Returns the minimum element in the stack.
int min_max_stack_max(Min_max_stack *stack);
