#include "strings.h"

#include <stdio.h>

bool strings_equal(char *a, char *b) {
    if (!a) {
        printf("nullptr passed to strings_equal_a\n");
    }
    if (!b) {
        printf("nullptr passed to strings_equal_b\n");
    }

    do {
        if (*a != *b) {
            return false;
        }
        b++;
    } while (*a++ != '\0');
    return true;
}

bool strings_is_dec_int(char *a) {
    if (!a) {
        printf("nullptr passed to strings_is_int\n");
    }

    do {
        char c = *a;
        if (c < '0' || c > '9') {
            return false;
        }
    } while (*++a != '\0');
    return true;
}

int strings_atoi(char *a) {
    if (!a) {
        printf("nullptr passed to strings_atoi\n");
    }

    int acc = 0;
    do {
        acc *= 10;
        acc += *a - '0';
    } while (*a++ != '\0');

    return acc;
}