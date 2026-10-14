#include <stdio.h>
#include <assert.h>
#include "salsa20.h"

int main(void) {
    assert(rotl32(13, 32) == 13);

    printf("True!\n");

    return 0;
}