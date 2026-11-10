#pragma once
#include <stdio.h>
#include "stack.h"

// Построение и вывод СДНФ (Совершенная дизъюнктивная нормальная форма)
void pdnf(stack_t *formula, int symbols_cnt, const char *symbols);

// Построение и вывод СКНФ (Совершенная конъюнктивная нормальная форма)
void pcnf(stack_t *formula, int symbols_cnt, const char *symbols);
