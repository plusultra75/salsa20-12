#include <assert.h>
#include <stdint.h>
#include "salsa20.h"

int main(void)
{
    uint8_t key[32];
    uint8_t nonce[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t zkey[32] = {0};
    uint8_t znonce[8] = {0};
    uint32_t s[16];

    for (int i = 0; i < 32; i++)
        key[i] = (uint8_t)i;

    salsa20_init_state(s, key, nonce, 0x1112131415161718ULL);

    assert(s[0] == 0x61707865 && s[5] == 0x3320646e);
    assert(s[10] == 0x79622d32 && s[15] == 0x6b206574);
    assert(s[1] == 0x03020100 && s[2] == 0x07060504);
    assert(s[3] == 0x0b0a0908 && s[4] == 0x0f0e0d0c);
    assert(s[11] == 0x13121110 && s[12] == 0x17161514);
    assert(s[13] == 0x1b1a1918 && s[14] == 0x1f1e1d1c);
    assert(s[6] == 0x04030201 && s[7] == 0x08070605);
    assert(s[8] == 0x15161718 && s[9] == 0x11121314);

    salsa20_init_state(s, zkey, znonce, 0);

    for (int i = 0; i < 16; i++) {
        if (i == 0 || i == 5 || i == 10 || i == 15)
            continue;
        assert(s[i] == 0);
    }

    return 0;
}