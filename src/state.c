#include <stdint.h>
#include <string.h>
#include "salsa20.h"

static uint32_t load_littleendian(const uint8_t *p) {
    return ((uint32_t)p[0]) |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

void salsa20_init_state(uint32_t state[16], const uint8_t key[32],
                        const uint8_t nonce[8], uint64_t counter) {
    state[0]  = 0x61707865;
    state[1]  = load_littleendian(key + 0);
    state[2]  = load_littleendian(key + 4);
    state[3]  = load_littleendian(key + 8);
    state[4]  = load_littleendian(key + 12);
    state[5]  = 0x3320646e;
    state[6]  = load_littleendian(key + 16);
    state[7]  = load_littleendian(key + 20);
    state[8]  = load_littleendian(key + 24);
    state[9]  = load_littleendian(key + 28);
    state[10] = 0x79622d32;
    state[11] = (uint32_t)(counter & 0xffffffff);
    state[12] = (uint32_t)(counter >> 32);
    state[13] = load_littleendian(nonce + 0);
    state[14] = load_littleendian(nonce + 4);
    state[15] = 0x6b206574;
}