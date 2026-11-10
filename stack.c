#include "stack.h"
#include <stdlib.h>
#include <string.h>

// Добавление элемента на вершину стека
void stack_push(stack_t **stack, const char *value) {
    if (!value) return;
    stack_t *new_node = (stack_t *)malloc(sizeof(stack_t));
    if (new_node != NULL) {
        new_node->value = strdup(value);
        new_node->next = *stack;
        *stack = new_node;
    }
}

// Извлечение элемента с вершины стека
char* stack_pop(stack_t **stack) {
    if (!stack || !*stack) return NULL;
    stack_t *temp = *stack;
    char *pop_value = temp->value;
    *stack = temp->next;
    free(temp);
    return pop_value;
}

// Проверка стека на пустоту
int stack_is_empty(stack_t *stack) {
    return (stack == NULL);
}

// Просмотр вершины стека без извлечения
char* stack_top(stack_t *stack) {
    if (stack_is_empty(stack)) return NULL;
    return stack->value;
}

// Полная очистка стека
void stack_free(stack_t **stack) {
    while (!stack_is_empty(*stack)) {
        char *val = stack_pop(stack);
        free(val);
    }
}
