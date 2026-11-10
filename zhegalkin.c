#include "zhegalkin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void zhegalkin_polynomial(const char* table_of_truth, int symbols_cnt, const char *symbols) {
    printf("Zhegalkin polynomial: ");
    if (!table_of_truth) {
        printf("0\n");
        return;
    }

    size_t len = strlen(table_of_truth);
    if (len == 0) {
        printf("0\n");
        return;
    }

    // Случай константной функции (нет переменных)
    if (symbols_cnt == 0) {
        if (table_of_truth[0] == '1') {
            printf("1\n");
        } else {
            printf("0\n");
        }
        return;
    }

    // Создаем локальную копию таблицы для преобразования
    char *table = (char *)malloc(len + 1);
    if (!table) return;
    strcpy(table, table_of_truth);

    int flag_none = 1;
    int flag_1 = 0;

    // Свободный член (константа 1)
    if (table[0] == '1') {
        printf("1");
        flag_1 = 1;
        flag_none = 0;
    }

    // Метод треугольника Паскаля (преобразование Мёбиуса)
    for (size_t j = 0; j < len - 1; ++j) {
        int prev = 0;
        for (int i = (int)(len - 1 - j); i >= 0; --i) {
            int tmp = table[i] - '0';
            table[i] = (char)(((tmp ^ prev) + '0'));
            prev = tmp;
        }

        if (table[0] == '1') {
            if (flag_1) {
                printf(" [^] ");
                flag_1 = 0;
            }

            int flag_first = 1;
            int cnt = 0;

            for (int k = 0; k < symbols_cnt; ++k) {
                // Находим следующую переменную
                for (int c = cnt; c < 26; ++c) {
                    if (symbols[c] == '1') {
                        cnt = c + 1;
                        break;
                    }
                }

                // Проверяем бит в двоичном представлении индекса монома (j + 1)
                if (((j + 1) >> (symbols_cnt - 1 - k)) & 1) {
                    if (!flag_first) {
                        printf(" [*] ");
                    }
                    printf("$%c", 'A' + cnt - 1);
                    flag_none = 0;
                    flag_first = 0;
                }
            }
            flag_1 = 1;
        }
    }

    if (flag_none) {
        printf("0");
    }
    printf("\n");

    free(table);
}
