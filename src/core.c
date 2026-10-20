#include "salsa20.h"

void doubleround(uint32_t state[16]) {
    columnround(state);
    rowround(state);
}

void salsa20_12_block(const uint32_t state[16], uint8_t keystream[64]) {
    uint32_t working_state[16];
    int i;

    /* Bước 1: sao chép state ban đầu để dùng cho feed-forward cuối */
    for (i = 0; i < 16; i++) working_state[i] = state[i];

    /* Bước 2: 6 double rounds = 12 rounds (Salsa20/12) */
    for (i = 0; i < 6; i++) doubleround(working_state);

    /* Bước 3: feed-forward — cộng state ban đầu, modulo 2^32 */
    for (i = 0; i < 16; i++) working_state[i] += state[i];

    /* Bước 4: xuất 64 byte little-endian */
    for (i = 0; i < 16; i++) {
        keystream[4*i + 0] = (uint8_t)( working_state[i]        & 0xFF);
        keystream[4*i + 1] = (uint8_t)((working_state[i] >>  8) & 0xFF);
        keystream[4*i + 2] = (uint8_t)((working_state[i] >> 16) & 0xFF);
        keystream[4*i + 3] = (uint8_t)((working_state[i] >> 24) & 0xFF);
    }
}