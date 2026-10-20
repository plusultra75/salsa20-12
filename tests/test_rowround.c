
#include <stdio.h>
#include <assert.h>
#include "salsa20.h"

// So sanh state thuc te voi ket qua mong doi
void check_state(const uint32_t actual[16],
                 const uint32_t expected[16]) {
    for (int i = 0; i < 16; i++) {
        assert(actual[i] == expected[i]);
    }
}

int main(void) {

    // Test 1: Tat ca phan tu bang 0
    uint32_t state0[16] = {0};
    const uint32_t expected0[16] = {0};

    rowround(state0);
    check_state(state0, expected0);
    printf("Test 1 passed!\n");

    // Test 2: Vector chuan trong Salsa20 specification
    uint32_t state1[16] = {
        1, 0, 0, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
        1, 0, 0, 0
    };

    const uint32_t expected1[16] = {
        0x08008145u, 0x00000080u, 0x00010200u, 0x20500000u,
        0x20100001u, 0x00048044u, 0x00000080u, 0x00010000u,
        0x00000001u, 0x00002000u, 0x80040000u, 0x00000000u,
        0x00000001u, 0x00000200u, 0x00402000u, 0x88000100u
    };

    rowround(state1);
    check_state(state1, expected1);
    printf("Test 2 passed!\n");

    // Test 3: Vector chuan voi 16 gia tri khac nhau
    uint32_t state2[16] = {
        0x08521bd6u, 0x1fe88837u, 0xbb2aa576u, 0x3aa26365u,
        0xc54c6a5bu, 0x2fc74c2fu, 0x6dd39cc3u, 0xda0a64f6u,
        0x90a2f23du, 0x067f95a6u, 0x06b35f61u, 0x41e4732eu,
        0xe859c100u, 0xea4d84b7u, 0x0f619bffu, 0xbc6e965au
    };

    const uint32_t expected2[16] = {
        0xa890d39du, 0x65d71596u, 0xe9487daau, 0xc8ca6a86u,
        0x949d2192u, 0x764b7754u, 0xe408d9b9u, 0x7a41b4d1u,
        0x3402e183u, 0x3c3af432u, 0x50669f96u, 0xd89ef0a8u,
        0x0040ede5u, 0xb545fbceu, 0xd257ed4fu, 0x1818882du
    };

    rowround(state2);
    check_state(state2, expected2);
    printf("Test 3 passed!\n");

    printf("All Row Round tests passed!\n");
    return 0;
}
