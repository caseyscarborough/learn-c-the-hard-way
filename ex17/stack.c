/**
 * Learn C the Hard Way
 *
 * Exercise 17 - Extra Credit (Stack Implementation)
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 25

struct Stack {
    int data[MAX_SIZE];
    int size;
};
typedef struct Stack Stack;

/* Forward declarations */
void Stack_initialize(Stack*);
void Stack_push(Stack*, int);
void Stack_push_multiple(Stack*, int, ...);
int Stack_top(Stack*);
void Stack_pop(Stack*);
void Stack_print(Stack*);

int main(int argc, char *argv[])
{
    struct Stack *s = malloc(sizeof(Stack));
    Stack_initialize(s);
    Stack_push_multiple(s, 4, 13, 1, 114, 58);
    printf("The top of the stack is: %d\n", Stack_top(s));
    
    Stack_print(s);    
    Stack_pop(s);
    printf("The top of the stack is: %d\n", Stack_top(s));
    
    Stack_push(s, 1415);
    Stack_push(s, 48);
    printf("The top of the stack is: %d\n", Stack_top(s));
    
    Stack_print(s);
    Stack_pop(s);
    printf("The top of the stack is: %d\n", Stack_top(s));

    free(s);
    return 0;
}

/**
 * This method is used to initialize the stack.
 */
void Stack_initialize(Stack *s)
{
    s->size = 0;
}

/**
 * Pushes an integer onto the top of the stack.
 */
void Stack_push(Stack *s, int n)
{
    if (s->size < MAX_SIZE) {
        printf("Pushing %d onto the stack...\n", n);
        s->data[s->size] = n;
        s->size++;
    } else {
        fprintf(stderr, "ERROR: Stack is full\n");
    }
}

/**
 * Push multiple integers onto the stack at once.
 * Parameters:
 * s - A pointer to the stack
 * num - The number of items that are being pushed onto the stack
 * ... - A list of integers to push onto the stack
 */
void Stack_push_multiple(Stack *s, int num, ...)
{
    int i;
    va_list args;
    va_start(args, num);
    for(i = 0; i < num; i++)
        Stack_push(s, va_arg(args, int));
    va_end(args);
}

/**
 * "Removes" the last integer that was pushed onto the stack.
 * This is done by decrementing the size of the stack, so that
 * the next integer push onto it will overwrite the last.
 */
void Stack_pop(Stack *s)
{
    if (s->size > 0) {
        printf("Popping %d off of the stack...\n", Stack_top(s));
        s->size--;
    } else {
        fprintf(stderr, "ERROR: Stack is empty\n");
    }
}

/**
 * Get the last integer pushed onto the stack.
 */
int Stack_top(Stack *s)
{
    if (s->size > 0) {
        return s->data[s->size-1];
    } else {
        fprintf(stderr, "ERROR: Stack is empty\n");
        return -1;
    }
}

/**
 * Print out the stack.
 */
void Stack_print(Stack *s)
{
    int i;
    for(i = s->size-1; i >= 0; i--) {
        printf("%d ", s->data[i]);
    }
    printf("\n");
}