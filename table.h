#pragma once
#include <stdio.h>
#include "stack.h"

// Построение и вывод таблицы истинности
// Возвращает строку результатов (вектор истинности)
char* print_table(FILE *file, stack_t *formula, int *symbols_cnt, char *symbols);
