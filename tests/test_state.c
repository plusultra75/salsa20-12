#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "salsa20.h"

int main() {
    uint8_t key[32];
    uint8_t nonce[8];
    for(int i = 0; i < 32; i++) key[i] = 0x01;
    for(int i = 0; i < 8; i++) nonce[i] = 0x02;
    uint64_t counter = 0;

    uint32_t state[16];
    salsa20_init_state(state, key, nonce, counter);

    assert(state[0] == 0x61707865);
    assert(state[5] == 0x3320646e);
    assert(state[10] == 0x79622d32);
    assert(state[15] == 0x6b206574);

    printf("=== KIEM TRA STATE KHOI TAO (PASS) ===\n");
    for(int i = 0; i < 16; i++) {
        printf("state[%2d] = 0x%08x\n", i, state[i]);
    }

    printf("\nTat cả các assertion da vuot qua thanh cong!\n");
    return 0;
}