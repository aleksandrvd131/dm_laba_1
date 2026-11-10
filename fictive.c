#include "fictive.h"
#include <stdio.h>
#include "formula.h"

// Переменная является фиктивной, если при любых значениях остальных переменных
// результат функции не зависит от значения этой переменной (f(x=0) == f(x=1)).
void print_fictive(stack_t *formula, int symbols_cnt, const char *symbols) {
    printf("Fictive variables: ");
    if (symbols_cnt == 0) {
        printf("None\n");
        return;
    }

    int found_fictive = 0;
    int var_letter_idx = 0;

    for (int i = 0; i < symbols_cnt; ++i) {
        int is_fictive = 1;
        int bit_pos = symbols_cnt - 1 - i;
        int combinations = 1 << (symbols_cnt - 1);

        for (int j = 0; j < combinations; ++j) {
            int low = j & ((1 << bit_pos) - 1);
            int high = (j >> bit_pos) << (bit_pos + 1);
            int val0 = high | (0 << bit_pos) | low;
            int val1 = high | (1 << bit_pos) | low;

            int res0 = compute_formula(formula, val0, symbols);
            int res1 = compute_formula(formula, val1, symbols);

            if (res0 != res1) {
                is_fictive = 0;
                break;
            }
        }

        // Находим соответствующую букву переменной
        for (int k = var_letter_idx; k < 26; ++k) {
            if (symbols[k] == '1') {
                var_letter_idx = k + 1;
                break;
            }
        }

        if (is_fictive) {
            printf("%c ", 'A' + var_letter_idx - 1);
            found_fictive = 1;
        }
    }

    if (!found_fictive) {
        printf("None");
    }
    printf("\n");
}
