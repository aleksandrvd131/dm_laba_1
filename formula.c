#include "formula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
  Коды операций:
  [!]   -> 0 (НЕ / NOT, унарный, приоритет 6, ассоциативность справа налево)
  [+]   -> 1 (ИЛИ / OR, приоритет 3)
  [*]   -> 2 (И / AND, приоритет 5)
  [^]   -> 3 (XOR / сложение по модулю 2, приоритет 4)
  [=]   -> 4 (Эквивалентность / XNOR, приоритет 1)
  [->]  -> 5 (Импликация, приоритет 2)
  [!->] -> 6 (Коимпликация / запрет, приоритет 2)
  [!+]  -> 7 (Стрелка Пирса / NOR, приоритет 3)
  [!*]  -> 8 (Штрих Шеффера / NAND, приоритет 5)
*/

short get_opcode(const char *op) {
    if (strcmp(op, "!") == 0) return 0;
    if (strcmp(op, "+") == 0) return 1;
    if (strcmp(op, "*") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "=") == 0) return 4;
    if (strcmp(op, "->") == 0) return 5;
    if (strcmp(op, "!->") == 0) return 6;
    if (strcmp(op, "!+") == 0) return 7;
    if (strcmp(op, "!*") == 0) return 8;
    return -1;
}

int is_operator(const char *op) {
    short code = get_opcode(op);
    return (code >= 0 && code <= 8);
}

// Приоритет операций (от высшего к низшему)
short check_priority(int opcode) {
    switch (opcode) {
        case 0: return 6; // [!]
        case 2: case 8: return 5; // [*], [!*]
        case 3: return 4; // [^]
        case 1: case 7: return 3; // [+], [!+]
        case 5: case 6: return 2; // [->], [!->]
        case 4: return 1; // [=]
        default: return 0;
    }
}

// Вычисление результата бинарной/унарной операции
char perform_operation(int opcode, int a, int b) {
    switch (opcode) {
        case 0: return (!a);        // NOT (!a)
        case 1: return (a || b);     // OR
        case 2: return (a && b);     // AND
        case 3: return (a ^ b);      // XOR
        case 4: return (a == b);     // EQUIV
        case 5: return (!a || b);    // IMPLICATION (a -> b)
        case 6: return (a && !b);    // COIMPLICATION (a !-> b)
        case 7: return !(a || b);    // NOR
        case 8: return !(a && b);    // NAND
        default: return 0;
    }
}

// Получение числового значения операнда (0 или 1)
int symbol_to_value(const char *symbol, const char values[]) {
    if (!symbol || symbol[0] == '\0') return 0;
    if (symbol[0] == '0' || symbol[0] == '1') {
        return symbol[0] - '0';
    }
    char ch = (char)toupper((unsigned char)symbol[0]);
    if (ch >= 'A' && ch <= 'Z') {
        return values[ch - 'A'] - '0';
    }
    return 0;
}

// Алгоритм Дейкстры (сортировочная станция): инфиксная запись -> ОПЗ
stack_t* formula_to_postfix(FILE *file) {
    stack_t *op_stack = NULL;
    stack_t *output_queue = NULL;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '$') {
            // Переменная или константа
            int sym = fgetc(file);
            if (sym != EOF) {
                char token_operand[2];
                token_operand[0] = (char)sym;
                token_operand[1] = '\0';
                stack_push(&output_queue, token_operand);
            }
        } else if (ch == '[') {
            // Оператор в квадратных скобках
            char token_operator[8];
            int i = 0;
            int op_ch;
            while ((op_ch = fgetc(file)) != EOF && op_ch != ']' && i < 7) {
                token_operator[i++] = (char)op_ch;
            }
            token_operator[i] = '\0';

            int curr_opcode = get_opcode(token_operator);
            int curr_prio = check_priority(curr_opcode);

            // Выталкиваем из стека операторы с большим или равным приоритетом.
            // Для оператора '!' ассоциативность правая, поэтому выталкиваем только если строго больше (>).
            while (!stack_is_empty(op_stack)) {
                char *top = stack_top(op_stack);
                if (top && strcmp(top, "(") == 0) break;

                int top_opcode = get_opcode(top);
                int top_prio = check_priority(top_opcode);

                if (curr_opcode == 0) { // унарный NOT
                    if (top_prio > curr_prio) {
                        char *popped = stack_pop(&op_stack);
                        stack_push(&output_queue, popped);
                        free(popped);
                    } else {
                        break;
                    }
                } else {
                    if (top_prio >= curr_prio) {
                        char *popped = stack_pop(&op_stack);
                        stack_push(&output_queue, popped);
                        free(popped);
                    } else {
                        break;
                    }
                }
            }
            stack_push(&op_stack, token_operator);
        } else if (ch == '(') {
            stack_push(&op_stack, "(");
        } else if (ch == ')') {
            while (!stack_is_empty(op_stack)) {
                char *top = stack_top(op_stack);
                if (top && strcmp(top, "(") == 0) break;
                char *popped = stack_pop(&op_stack);
                stack_push(&output_queue, popped);
                free(popped);
            }
            if (!stack_is_empty(op_stack)) {
                char *opening = stack_pop(&op_stack);
                free(opening);
            }
        }
    }

    // Выталкиваем оставшиеся операторы
    while (!stack_is_empty(op_stack)) {
        char *popped = stack_pop(&op_stack);
        stack_push(&output_queue, popped);
        free(popped);
    }

    rewind(file);

    // Разворачиваем стек, чтобы получить порядок от начала формулы к концу
    stack_t *formula = NULL;
    while (!stack_is_empty(output_queue)) {
        char *popped = stack_pop(&output_queue);
        stack_push(&formula, popped);
        free(popped);
    }

    return formula;
}

// Вычисление значения постфиксной формулы
int compute_formula(stack_t *formula, int values, const char *symbols) {
    char symbols_with_values[27];
    strncpy(symbols_with_values, symbols, 26);
    symbols_with_values[26] = '\0';

    // Раскладываем биты числа values по присутствующим переменным справа налево
    for (int i = 25; i >= 0; --i) {
        if (symbols_with_values[i] == '1') {
            symbols_with_values[i] = (char)('0' + (values % 2));
            values /= 2;
        }
    }

    stack_t *expression = NULL;
    for (stack_t *cur = formula; cur != NULL; cur = cur->next) {
        if (!is_operator(cur->value)) {
            stack_push(&expression, cur->value);
        } else {
            int opcode = get_opcode(cur->value);
            if (opcode == 0) { // Унарное отрицание [!]
                char *op1 = stack_pop(&expression);
                int op1_val = symbol_to_value(op1, symbols_with_values);
                free(op1);

                char calc_res[2];
                calc_res[0] = (char)(perform_operation(opcode, op1_val, 0) + '0');
                calc_res[1] = '\0';
                stack_push(&expression, calc_res);
            } else { // Бинарные операции
                char *op2 = stack_pop(&expression);
                char *op1 = stack_pop(&expression);
                int op2_val = symbol_to_value(op2, symbols_with_values);
                int op1_val = symbol_to_value(op1, symbols_with_values);
                free(op2);
                free(op1);

                char calc_res[2];
                calc_res[0] = (char)(perform_operation(opcode, op1_val, op2_val) + '0');
                calc_res[1] = '\0';
                stack_push(&expression, calc_res);
            }
        }
    }

    if (stack_is_empty(expression)) return -1;

    char *expr_top = stack_pop(&expression);
    int return_val = symbol_to_value(expr_top, symbols_with_values) + '0';
    free(expr_top);

    return return_val;
}
