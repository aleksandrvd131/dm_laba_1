#include "table.h"
#include "formula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* print_table(FILE *file, stack_t *formula, int *symbols_cnt, char *symbols) {
    char symbols_t[] = "00000000000000000000000000"; // Флаги наличия переменных 'A'..'Z'
    *symbols_cnt = 0;

    // Ищем все уникальные переменные в файле
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '$') {
            int var_ch = fgetc(file);
            if (var_ch != EOF && isalpha(var_ch)) {
                int idx = toupper(var_ch) - 'A';
                if (symbols_t[idx] == '0') {
                    symbols_t[idx] = '1';
                    (*symbols_cnt)++;
                }
            }
        }
    }
    rewind(file);

    // Копируем маску переменных в выходной буфер
    strcpy(symbols, symbols_t);

    // Если переменных нет (только константы 0/1)
    if (*symbols_cnt == 0) {
        int res = compute_formula(formula, 0, symbols_t);
        printf("The result of formula is always %c\n", (char)res);
        char *result = (char *)malloc(2);
        if (result) {
            result[0] = (char)res;
            result[1] = '\0';
        }
        return result;
    }

    // Вывод заголовка таблицы истинности
    for (int i = 0; i < 26; ++i) {
        if (symbols_t[i] == '1') {
            printf(" %c |", 'A' + i);
        }
    }
    printf(" Result\n");

    // Вывод строк таблицы истинности
    int total_rows = 1 << (*symbols_cnt);
    char *table_of_truth = (char *)malloc(total_rows + 1);
    if (!table_of_truth) return NULL;

    for (int i = 0; i < total_rows; ++i) {
        for (int j = 1; j <= *symbols_cnt; ++j) {
            printf(" %d |", (i >> (*symbols_cnt - j)) & 1);
        }
        int res = compute_formula(formula, i, symbols_t);
        printf(" %c\n", (char)res);
        table_of_truth[i] = (char)res;
    }
    table_of_truth[total_rows] = '\0';

    return table_of_truth;
}
