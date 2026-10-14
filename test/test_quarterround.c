#include <stdio.h>
#include <assert.h>
#include "salsa20.h"

int main(void) {
    assert(rotl32(13, 32) == 13);// Kiem tra rotl32: xoay du 32 bit phai ve gia tri ban dau
  // Tao mang du lieu dau vao cho Quarter Round
uint32_t x[4] = {1, 0, 0, 0};
// Goi ham Quarter Round de bien doi 4 phan tu
quarterround(x);
   // So sanh ket qua voi bo gia tri kiem thu 
assert(x[0] == 134250821u);
assert(x[1] == 128u);
assert(x[2] == 66048u);
assert(x[3] == 542113792u);

printf("True\n");
return 0;
}