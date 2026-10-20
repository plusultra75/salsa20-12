
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "salsa20.h"

// Quarter Round tạm phục vụ kiểm thử độc lập.
// Khi dùng source Member 1, biên dịch với -DUSE_MEMBER1.

#ifndef USE_MEMBER1

static uint32_t rol32_for_test(uint32_t x, unsigned int n)
{
    return (x << n) | (x >> (32U - n));
}

void quarterround(uint32_t x[4])
{
    x[1] ^= rol32_for_test(x[0] + x[3], 7);
    x[2] ^= rol32_for_test(x[1] + x[0], 9);
    x[3] ^= rol32_for_test(x[2] + x[1], 13);
    x[0] ^= rol32_for_test(x[3] + x[2], 18);
}

#endif

// So sánh kết quả với test vector chuẩn
static int check(
    const char *name,
    const uint32_t input[16],
    const uint32_t expected[16]
)
{
    uint32_t actual[16];
    int ok = 1;

    for (int i = 0; i < 16; ++i) {
        actual[i] = input[i];
    }

    columnround(actual);

    for (int i = 0; i < 16; ++i) {
        if (actual[i] != expected[i]) {
            printf(
                "Index %d: expected %08" PRIx32
                ", got %08" PRIx32 "\n",
                i, expected[i], actual[i]
            );
            ok = 0;
        }
    }

    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    return ok;
}

int main(void)
{
    // Test 1: toàn bộ state bằng 0
    const uint32_t zeros[16] = {0};

    // Test 2: test vector chuẩn Salsa20
    const uint32_t input1[16] = {
        1, 0, 0, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
        1, 0, 0, 0
    };

    const uint32_t expected1[16] = {
        0x10090288, 0, 0, 0,
        0x00000101, 0, 0, 0,
        0x00020401, 0, 0, 0,
        0x40a04001, 0, 0, 0
    };

    // Test 3: test vector thứ hai
    const uint32_t input2[16] = {
        0x08521bd6, 0x1fe88837,
        0xbb2aa576, 0x3aa26365,
        0xc54c6a5b, 0x2fc74c2f,
        0x6dd39cc3, 0xda0a64f6,
        0x90a2f23d, 0x067f95a6,
        0x06b35f61, 0x41e4732e,
        0xe859c100, 0xea4d84b7,
        0x0f619bff, 0xbc6e965a
    };

    const uint32_t expected2[16] = {
        0x8c9d190a, 0xce8e4c90,
        0x1ef8e9d3, 0x1326a71a,
        0x90a20123, 0xead3c4f3,
        0x63a091a0, 0xf0708d69,
        0x789b010c, 0xd195a681,
        0xeb7d5504, 0xa774135c,
        0x481c2027, 0x53a8e4b5,
        0x4c1f89c5, 0x3f78c9c8
    };

    int passed = 0;

    passed += check("Zero state", zeros, zeros);
    passed += check("Reference vector 1", input1, expected1);
    passed += check("Reference vector 2", input2, expected2);

    printf("\nResult: %d/3 tests passed\n", passed);

    return (passed == 3) ? 0 : 1;
}
