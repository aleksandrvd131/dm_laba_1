#pragma once
#include "stack.h"

// Форматирование шаблона с заменой {A} и {B}
char* format_string(const char *tmpl, const char *A, const char *B);

// Представление константы 0 в заданном базисе
const char *basis_zero(int basis);

// Представление константы 1 в заданном базисе
const char *basis_one(int basis);

// Перевод операций в базис {И, ИЛИ, НЕ}
char *basis_to_and_or(int opcode, const char *op1, const char *op2);

// Перевод операций в базис {ИЛИ-НЕ (стрелка Пирса)}
char *basis_to_nor(int opcode, const char *op1, const char *op2);

// Перевод операций в базис {И-НЕ (штрих Шеффера)}
char *basis_to_nand(int opcode, const char *op1, const char *op2);

// Преобразование формулы в заданный базис (0: AND/OR/NOT, 1: NOR, 2: NAND)
void basis_change(stack_t *formula, int basis);
