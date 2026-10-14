#include <stdio.h>
#include <assert.h>
#include "salsa20.h"

int main(void) {
assert(rotl32(13, 32) == 13);// Kiem tra rotl32: xoay du 32 bit phai ve gia tri ban dau
// Kiểm tra trường hợp không xoay bit
assert(rotl32(13, 0) == 13);

// Kiểm tra xoay trái 2 bit
assert(rotl32(13, 2) == 52);

// Kiểm tra xoay quá 32 bit
assert(rotl32(13, 33) == 26);

// Kiểm tra bit ngoài cùng bên trái quay về bên phải
assert(rotl32(0x80000000u, 1) == 1u);
  // Tao mang du lieu dau vao cho Quarter Round
uint32_t x[4] = {1, 0, 0, 0};
// Goi ham Quarter Round de bien doi 4 phan tu
quarterround(x);
   // So sanh ket qua voi bo gia tri kiem thu 
assert(x[0] == 134250821u);
assert(x[1] == 128u);
assert(x[2] == 66048u);
assert(x[3] == 542113792u);
// Kiểm tra Quarter Round khi cả 4 phần tử bằng 0
uint32_t y[4] = {0, 0, 0, 0};

quarterround(y);

// Sau khi biến đổi vẫn phải bằng 0
for (int i = 0; i < 4; i++) {
    assert(y[i] == 0);
}

printf("True\n");
return 0;
}