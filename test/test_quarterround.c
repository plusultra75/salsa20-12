#include <stdio.h>
#include <assert.h>
#include "salsa20.h"

int main(void) {
    assert(rotl32(13, 32) == 13);

uint32_t x[4] = {1, 0, 0, 0};

quarterround(x);

assert(x[0] == 134250821u);
assert(x[1] == 128u);
assert(x[2] == 66048u);
assert(x[3] == 542113792u);

printf("True\n");
return 0;
}