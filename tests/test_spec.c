#include <assert.h>
#include <stdint.h>
#include "salsa20.h"

int main(void)
{
    uint8_t key[32], nonce[8];
    uint32_t s[16];

    for (int i = 0; i < 16; i++) {
        key[i] = (uint8_t)(1 + i);
        key[16 + i] = (uint8_t)(201 + i);
    }
    for (int i = 0; i < 8; i++)
        nonce[i] = (uint8_t)(101 + i);

    salsa20_init_state(s, key, nonce, 0x74737271706f6e6dULL);

    assert(s[0] == 0x61707865 && s[5] == 0x3320646e);
    assert(s[10] == 0x79622d32 && s[15] == 0x6b206574);
    assert(s[1] == 0x04030201 && s[4] == 0x100f0e0d);
    assert(s[6] == 0x68676665 && s[7] == 0x6c6b6a69);
    assert(s[8] == 0x706f6e6d && s[9] == 0x74737271);
    assert(s[11] == 0xcccbcac9 && s[14] == 0xd8d7d6d5);

    return 0;
}