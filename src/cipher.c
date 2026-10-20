#include "salsa20.h"

void salsa20_12_crypt(
    const uint8_t *input,
    uint8_t *output,
    size_t length,
    const uint8_t key[32],
    const uint8_t nonce[8]
)
{
    uint32_t state[16];
    uint8_t keystream[64];
    uint64_t counter = 0;
    size_t offset = 0;

    if (length == 0) {
        return;
    }
    
    salsa20_init_state(state, key, nonce, counter);
    salsa20_12_block(state, keystream);

    while (length > 64) {
        for (size_t i = 0; i < 64; i++) {
            output[offset + i] = input[offset + i] ^ keystream[i];
        }
        offset += 64;
        length -= 64;
        counter++;
        salsa20_init_state(state, key, nonce, counter);
        salsa20_12_block(state, keystream);
    }

    for (size_t i = 0; i < length; i++) {
        output[offset + i] = input[offset + i] ^ keystream[i];
    }
}
