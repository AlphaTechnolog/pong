#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

int int_bytes(int n) {
    return floor(log10(abs(n == 0 ? 1 : n))) + 1;
}

char *int2str(int n) {
    char *str = malloc(int_bytes(n));
    sprintf(str, "%d", n);
    return str;
}

