#include <stdint.h>
#include "salsa20.h"

static uint32_t load32_le(const uint8_t *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 |
           (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

void salsa20_init_state(uint32_t state[16], const uint8_t key[32],
                        const uint8_t nonce[8], uint64_t counter)
{
    state[0]  = 0x61707865;
    state[1]  = load32_le(key);
    state[2]  = load32_le(key + 4);
    state[3]  = load32_le(key + 8);
    state[4]  = load32_le(key + 12);
    state[5]  = 0x3320646e;
    state[6]  = load32_le(nonce);
    state[7]  = load32_le(nonce + 4);
    state[8]  = (uint32_t)(counter & 0xffffffffu);
    state[9]  = (uint32_t)(counter >> 32);
    state[10] = 0x79622d32;
    state[11] = load32_le(key + 16);
    state[12] = load32_le(key + 20);
    state[13] = load32_le(key + 24);
    state[14] = load32_le(key + 28);
    state[15] = 0x6b206574;
}