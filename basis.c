#include "basis.h"
#include "formula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Подстановка аргументов {A} и {B} в шаблон
char* format_string(const char *tmpl, const char *A, const char *B) {
    if (!tmpl) return NULL;
    size_t len_A = A ? strlen(A) : 0;
    size_t len_B = B ? strlen(B) : 0;

    size_t total_len = 0;
    for (const char *p = tmpl; *p; ) {
        if (p[0] == '{' && p[1] == 'A' && p[2] == '}') {
            total_len += len_A;
            p += 3;
        } else if (p[0] == '{' && p[1] == 'B' && p[2] == '}') {
            total_len += len_B;
            p += 3;
        } else {
            total_len++;
            p++;
        }
    }

    char *result = (char *)malloc(total_len + 1);
    if (!result) return NULL;

    char *dst = result;
    for (const char *p = tmpl; *p; ) {
        if (p[0] == '{' && p[1] == 'A' && p[2] == '}') {
            if (len_A > 0) {
                memcpy(dst, A, len_A);
                dst += len_A;
            }
            p += 3;
        } else if (p[0] == '{' && p[1] == 'B' && p[2] == '}') {
            if (len_B > 0) {
                memcpy(dst, B, len_B);
                dst += len_B;
            }
            p += 3;
        } else {
            *dst++ = *p++;
        }
    }
    *dst = '\0';

    return result;
}

// Константа 0 в различных базисах
const char *basis_zero(int basis) {
    switch (basis) {
        case 0: return "($A [*] [!]$A)";
        case 1: return "(($A [!+] $A) [!+] (($A [!+] $A) [!+] ($A [!+] $A)))";
        case 2: return "(($A [!*] ($A [!*] $A)) [!*] ($A [!*] ($A [!*] $A)))";
        default: return "0";
    }
}

// Константа 1 в различных базисах
const char *basis_one(int basis) {
    switch (basis) {
        case 0: return "($A [+] [!]$A)";
        case 1: return "(((($A [!+] $A) [!+] $A)) [!+] ((($A [!+] $A) [!+] $A)))";
        case 2: return "($A [!*] ($A [!*] $A))";
        default: return "1";
    }
}

// Базис {И, ИЛИ, НЕ}
char *basis_to_and_or(int opcode, const char *op1, const char *op2) {
    switch (opcode) {
        case 0: return format_string("([!]{A})", op1, NULL);
        case 1: return format_string("({A} [+] {B})", op1, op2);
        case 2: return format_string("({A} [*] {B})", op1, op2);
        case 3: return format_string("(({A} [*] [!]{B}) [+] ([!]{A} [*] {B}))", op1, op2);
        case 4: return format_string("(({A} [*] {B}) [+] ([!]{A} [*] [!]{B}))", op1, op2);
        case 5: return format_string("([!]{A} [+] {B})", op1, op2);
        case 6: return format_string("({A} [*] [!]{B})", op1, op2);
        case 7: return format_string("([!]({A} [+] {B}))", op1, op2);
        case 8: return format_string("([!]({A} [*] {B}))", op1, op2);
        default: return NULL;
    }
}

// Базис {ИЛИ-НЕ} (стрелка Пирса)
char *basis_to_nor(int opcode, const char *op1, const char *op2) {
    switch (opcode) {
        case 0: return format_string("({A} [!+] {A})", op1, NULL);
        case 1: return format_string("(({A} [!+] {B}) [!+] ({A} [!+] {B}))", op1, op2);
        case 2: return format_string("(({A} [!+] {A}) [!+] ({B} [!+] {B}))", op1, op2);
        case 3: return format_string("(((({A} [!+] {B}) [!+] {B}) [!+] (({A} [!+] {B}) [!+] {A})) [!+] ((({A} [!+] {B}) [!+] {B}) [!+] (({A} [!+] {B}) [!+] {A})))", op1, op2);
        case 4: return format_string("((({A} [!+] {B}) [!+] {B}) [!+] (({A} [!+] {B}) [!+] {A}))", op1, op2);
        case 5: return format_string("((({A} [!+] {B}) [!+] {B}) [!+] (({A} [!+] {B}) [!+] {B}))", op1, op2);
        case 6: return format_string("(({A} [!+] {B}) [!+] {B})", op1, op2);
        case 7: return format_string("({A} [!+] {B})", op1, op2);
        case 8: return format_string("((({A} [!+] {A}) [!+] ({B} [!+] {B})) [!+] (({A} [!+] {A}) [!+] ({B} [!+] {B})))", op1, op2);
        default: return NULL;
    }
}

// Базис {И-НЕ} (штрих Шеффера)
char *basis_to_nand(int opcode, const char *op1, const char *op2) {
    switch (opcode) {
        case 0: return format_string("({A} [!*] {A})", op1, NULL);
        case 1: return format_string("(({A} [!*] {A}) [!*] ({B} [!*] {B}))", op1, op2);
        case 2: return format_string("(({A} [!*] {B}) [!*] ({A} [!*] {B}))", op1, op2);
        case 3: return format_string("((({A} [!*] {B}) [!*] {A}) [!*] (({A} [!*] {B}) [!*] {B}))", op1, op2);
        case 4: return format_string("((((({A} [!*] {B}) [!*] {A}) [!*] (({A} [!*] {B}) [!*] {B})) [!*] ((({A} [!*] {B}) [!*] {A}) [!*] (({A} [!*] {B}) [!*] {B}))))", op1, op2);
        case 5: return format_string("({A} [!*] ({B} [!*] {B}))", op1, op2);
        case 6: return format_string("(({A} [!*] ({B} [!*] {B})) [!*] ({A} [!*] ({B} [!*] {B})))", op1, op2);
        case 7: return format_string("((({A} [!*] {A}) [!*] ({B} [!*] {B})) [!*] (({A} [!*] {A}) [!*] ({B} [!*] {B})))", op1, op2);
        case 8: return format_string("({A} [!*] {B})", op1, op2);
        default: return NULL;
    }
}

// Преобразование формулы в заданный базис
void basis_change(stack_t *formula, int basis) {
    stack_t *str_stack = NULL;

    for (stack_t *cur = formula; cur != NULL; cur = cur->next) {
        if (is_operator(cur->value)) {
            int opcode = get_opcode(cur->value);
            char *op_res = NULL;

            if (opcode == 0) { // Унарное отрицание !
                char *op1 = stack_pop(&str_stack);
                switch (basis) {
                    case 0: op_res = basis_to_and_or(opcode, op1, NULL); break;
                    case 1: op_res = basis_to_nor(opcode, op1, NULL); break;
                    case 2: op_res = basis_to_nand(opcode, op1, NULL); break;
                }
                free(op1);
            } else { // Бинарные операции
                char *op2 = stack_pop(&str_stack);
                char *op1 = stack_pop(&str_stack);
                switch (basis) {
                    case 0: op_res = basis_to_and_or(opcode, op1, op2); break;
                    case 1: op_res = basis_to_nor(opcode, op1, op2); break;
                    case 2: op_res = basis_to_nand(opcode, op1, op2); break;
                }
                free(op2);
                free(op1);
            }

            if (op_res) {
                stack_push(&str_stack, op_res);
                free(op_res);
            }
        } else {
            // Операнд (константа или переменная)
            if (strcmp(cur->value, "0") == 0) {
                stack_push(&str_stack, basis_zero(basis));
            } else if (strcmp(cur->value, "1") == 0) {
                stack_push(&str_stack, basis_one(basis));
            } else {
                char var_token[32];
                snprintf(var_token, sizeof(var_token), "$%s", cur->value);
                stack_push(&str_stack, var_token);
            }
        }
    }

    if (stack_is_empty(str_stack)) return;

    char *result = stack_pop(&str_stack);
    switch (basis) {
        case 0: printf("AND, OR, NOT basis: "); break;
        case 1: printf("NOR basis: "); break;
        case 2: printf("NAND basis: "); break;
        case 3: printf("XOR, AND, 1 basis: "); break;
    }
    printf("%s\n", result);
    free(result);
}
