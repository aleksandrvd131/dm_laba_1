#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "formula.h"
#include "table.h"
#include "pf.h"
#include "fictive.h"
#include "basis.h"
#include "zhegalkin.h"

int main(int argc, char *argv[]) {
    int pdnf_flag = 0;
    int pcnf_flag = 0;
    int fictive_flag = 0;
    int diff_basis_flag = 0;

    if (argc < 2) {
        printf("Usage: truth_table [--flags] <path_to_file>\n");
        return 0;
    }

    // Обработка ключей командной строки
    for (int i = 1; i < argc - 1; ++i) {
        if (strlen(argv[i]) < 2 || argv[i][0] != '-' || argv[i][1] != '-') {
            printf("Unknown parameter: %s\n", argv[i]);
            return 1;
        }
        if (strcmp(argv[i], "--pdnf") == 0) {
            pdnf_flag = 1;
        } else if (strcmp(argv[i], "--pcnf") == 0) {
            pcnf_flag = 1;
        } else if (strcmp(argv[i], "--fictive") == 0) {
            fictive_flag = 1;
        } else if (strcmp(argv[i], "--different_basis") == 0) {
            diff_basis_flag = 1;
        } else {
            printf("Unknown flag: %s\n", argv[i]);
        }
    }

    // Открытие файла с формулой
    FILE *file = fopen(argv[argc - 1], "r");
    if (file == NULL) {
        printf("File could not be opened\n");
        return 1;
    }

    // Преобразование формулы в постфиксную запись (ОПЗ)
    stack_t *formula = formula_to_postfix(file);

    int symbols_cnt = 0;
    char symbols[27] = {0};

    // Построение и вывод таблицы истинности
    char *table_of_truth = print_table(file, formula, &symbols_cnt, symbols);

    // Вывод СДНФ при наличии флага --pdnf
    if (pdnf_flag) {
        pdnf(formula, symbols_cnt, symbols);
    }

    // Вывод СКНФ при наличии флага --pcnf
    if (pcnf_flag) {
        pcnf(formula, symbols_cnt, symbols);
    }

    // Вывод фиктивных переменных при наличии флага --fictive
    if (fictive_flag) {
        print_fictive(formula, symbols_cnt, symbols);
    }

    // Вывод в различных базисах и полинома Жегалкина при флаге --different_basis
    if (diff_basis_flag) {
        for (int i = 0; i < 3; ++i) {
            basis_change(formula, i);
        }
        zhegalkin_polynomial(table_of_truth, symbols_cnt, symbols);
    }

    // Освобождение ресурсов
    fclose(file);
    free(table_of_truth);

    while (!stack_is_empty(formula)) {
        char *to_free = stack_pop(&formula);
        free(to_free);
    }

    return 0;
}
