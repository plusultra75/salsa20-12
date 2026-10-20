#include <assert.h>
#include <stdint.h>
#include "salsa20.h"

int main(void)
{
    uint8_t key[32], nonce[8];
    uint32_t s[16];

    for (int i = 0; i < 32; i++)
        key[i] = 0xff;
    for (int i = 0; i < 8; i++)
        nonce[i] = 0x80;

    salsa20_init_state(s, key, nonce, 0xffffffffffffffffULL);
    for (int i = 1; i < 5; i++)
        assert(s[i] == 0xffffffffu);
    for (int i = 11; i < 15; i++)
        assert(s[i] == 0xffffffffu);
    assert(s[6] == 0x80808080u && s[7] == 0x80808080u);
    assert(s[8] == 0xffffffffu && s[9] == 0xffffffffu);

    salsa20_init_state(s, key, nonce, 0x100000000ULL);
    assert(s[8] == 0 && s[9] == 1);

    salsa20_init_state(s, key, nonce, 0xffffffffULL);
    assert(s[8] == 0xffffffffu && s[9] == 0);

    return 0;
}