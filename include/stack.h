// A dynamically-resized stack.

typedef struct Stack {
    void **buf;
    size_t buf_len;
    size_t len;
} Stack;

void stack_init(Stack *stack);
void stack_free(Stack *stack);

// Returns the number of elements stored in 'stack'.
size_t stack_len(Stack *stack);

// Pushes a new element onto 'stack'.
void stack_push(Stack *stack, void *val);

// Returns the top element of 'stack' without modifying 'stack'. Must not be
// called with an empty stack.
void *stack_peek(Stack *stack);

// Pops the top element off 'stack'. Must not be called with an empty stack.
void *stack_pop(Stack *stack);
