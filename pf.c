#include "pf.h"
#include <stdio.h>
#include "formula.h"

// Вспомогательная функция для получения буквы j-й переменной (1-based)
static char get_var_letter(int var_num, const char *symbols) {
    int count = 0;
    for (int k = 0; k < 26; ++k) {
        if (symbols[k] == '1') {
            count++;
            if (count == var_num) {
                return (char)('A' + k);
            }
        }
    }
    return '?';
}

// Построение СДНФ (PDNF)
void pdnf(stack_t *formula, int symbols_cnt, const char *symbols) {
    printf("PDNF: ");
    if (symbols_cnt == 0) {
        int val = compute_formula(formula, 0, symbols);
        if (val == '1') {
            printf("$1\n");
        } else {
            printf("No opportunity to construct a PDNF because of function is always 0\n");
        }
        return;
    }

    int first_flag = 1;
    int total_rows = 1 << symbols_cnt;

    for (int i = 0; i < total_rows; ++i) {
        if (compute_formula(formula, i, symbols) == '1') {
            if (first_flag) {
                first_flag = 0;
            } else {
                printf(" [+] ");
            }

            for (int j = 1; j <= symbols_cnt; ++j) {
                int bit = (i >> (symbols_cnt - j)) & 1;
                if (!bit) {
                    printf("[!]");
                }
                printf("$%c", get_var_letter(j, symbols));
                if (j != symbols_cnt) {
                    printf(" [*] ");
                }
            }
        }
    }

    if (first_flag) {
        printf("No opportunity to construct a PDNF because of function is always 0");
    }
    printf("\n");
}

// Построение СКНФ (PCNF)
void pcnf(stack_t *formula, int symbols_cnt, const char *symbols) {
    printf("PCNF: ");
    if (symbols_cnt == 0) {
        int val = compute_formula(formula, 0, symbols);
        if (val == '0') {
            printf("$0)\n");
        } else {
            printf("No opportunity to construct a PCNF because of function is always 1\n");
        }
        return;
    }

    int first_flag = 1;
    int total_rows = 1 << symbols_cnt;

    for (int i = 0; i < total_rows; ++i) {
        if (compute_formula(formula, i, symbols) == '0') {
            if (first_flag) {
                first_flag = 0;
            } else {
                printf(" [*] ");
            }

            printf("(");
            for (int j = 1; j <= symbols_cnt; ++j) {
                int bit = (i >> (symbols_cnt - j)) & 1;
                if (bit) {
                    printf("[!]");
                }
                printf("$%c", get_var_letter(j, symbols));
                if (j != symbols_cnt) {
                    printf(" [+] ");
                } else {
                    printf(")");
                }
            }
        }
    }

    if (first_flag) {
        printf("No opportunity to construct a PCNF because of function is always 1");
    }
    printf("\n");
}
