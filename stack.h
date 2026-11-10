#pragma once

// Узел стека
typedef struct stack_node {
    char *value;
    struct stack_node *next;
} stack_t;

// Функции для работы со стеком
void stack_push(stack_t **stack, const char *value);
char* stack_pop(stack_t **stack);
int stack_is_empty(stack_t *stack);
char* stack_top(stack_t *stack);
void stack_free(stack_t **stack);
