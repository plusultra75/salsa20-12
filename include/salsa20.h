
#ifndef SALSA20_H
#define SALSA20_H

#include <stdint.h>
#include <stddef.h>

// Member 1: Quarter Round
uint32_t rotl32(uint32_t x, unsigned int n);
void quarterround(uint32_t x[4]);

// Member 2: Row Round
void rowround(uint32_t state[16]);

// Member 3: Column Round
void columnround(uint32_t state[16]);

// Member 4: Double Round + Core
void doubleround(uint32_t state[16]);

void salsa20_12_block(
    const uint32_t state[16],
    uint8_t keystream[64]
);

// Member 5: State Initialization
void salsa20_init_state(
    uint32_t state[16],
    const uint8_t key[32],
    const uint8_t nonce[8],
    uint64_t counter
);

// Member 6: Encryption / Decryption
void salsa20_12_crypt(
    const uint8_t *input,
    uint8_t *output,
    size_t length,
    const uint8_t key[32],
    const uint8_t nonce[8]
);

#endif
