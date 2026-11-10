#pragma once
#include <stdio.h>
#include "stack.h"

// Определение кода операции по строковому представлению
short get_opcode(const char *op);

// Проверка, является ли строка оператором
int is_operator(const char *op);

// Приоритет операций
short check_priority(int opcode);

// Выполнение логической операции
char perform_operation(int opcode, int a, int b);

// Получение значения переменной/константы по текущему набору значений
int symbol_to_value(const char *symbol, const char values[]);

// Перевод формулы из файла в постфиксную запись (ОПЗ)
stack_t* formula_to_postfix(FILE *file);

// Вычисление значения формулы для заданного целочисленного набора values
int compute_formula(stack_t *formula, int values, const char *symbols);
