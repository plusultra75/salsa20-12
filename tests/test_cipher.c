#include <assert.h>
#include <stdio.h>
#include "salsa20.h"

int main(void)
{
    const uint8_t key[32] = {0x80};
    const uint8_t nonce[8] = {0};

    // Test 1: length = 0 
    const uint8_t empty_input[1] = {0};
    uint8_t empty_output[1] = {0xA5};
    salsa20_12_crypt(empty_input, empty_output, 0, key, nonce);
    assert(empty_output[0] == 0xA5);
    puts("PASS 1: output unchanged");

    // Test 2: 1 byte 'A'. 
    const uint8_t one_byte[1] = {'A'};
    uint8_t one_output[1] = {0};
    salsa20_12_crypt(one_byte, one_output, 1, key, nonce);
    assert(one_output[0] == 0xEE);
    puts("PASS 2: A -> EE");

    // Test 3: 3 byte 'A', 'B', 'C'. 
    const uint8_t input[3] = {'A', 'B', 'C'};
    uint8_t ciphertext[3] = {0};
    salsa20_12_crypt(input, ciphertext, 3, key, nonce);
    assert(ciphertext[0] == 0xEE);
    assert(ciphertext[1] == 0xA6);
    assert(ciphertext[2] == 0x52);
    puts("PASS 3: ABC -> EE A6 52");

    // Test 4: decrypt 
    uint8_t recovered[3] = {0};
    salsa20_12_crypt(ciphertext, recovered, 3, key, nonce);
    assert(recovered[0] == input[0]);
    assert(recovered[1] == input[1]);
    assert(recovered[2] == input[2]);
    puts("PASS 4: decrypt recovers ABC");

    // Keystream counter 0 kiem tra 64 byte dau.
    // dung cho test 56
    uint32_t state[16];
    uint8_t block0[64];
    salsa20_init_state(state, key, nonce, 0);
    salsa20_12_block(state, block0);

    // Test 5: 63 byte
    uint8_t input63[63];
    uint8_t output63[64] = {0};
    for (int i = 0; i < 63; i++) {
        input63[i] = (uint8_t) i;
    }
    output63[63] = 0xA5;

    salsa20_12_crypt(input63, output63, 63, key, nonce);
    for (int i = 0; i < 63; i++) {
        assert(output63[i] == (input63[i] ^ block0[i]));
    }
    assert(output63[63] == 0xA5);
    puts("PASS 5: 63 bytes");

    // Test 6: 64 byte, vua du mot khoi.
    uint8_t input64[64];
    uint8_t output64[65] = {0};
    for (int i = 0; i < 64; i++) {
        input64[i] = (uint8_t) i;
    }
    output64[64] = 0xA5;

    salsa20_12_crypt(input64, output64, 64, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output64[i] == (input64[i] ^ block0[i]));
    }
    assert(output64[64] == 0xA5);
    puts("PASS 6: 64 bytes");

    // Test 7: 65 byte
    uint8_t block1[64];
    salsa20_init_state(state, key, nonce, 1);
    salsa20_12_block(state, block1);

    uint8_t input65[65];
    uint8_t output65[66] = {0};
    for (int i = 0; i < 65; i++) {
        input65[i] = (uint8_t) i;
    }
    output65[65] = 0xA5;

    salsa20_12_crypt(input65, output65, 65, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output65[i] == (input65[i] ^ block0[i]));
    }
    assert(output65[64] == (input65[64] ^ block1[0]));
    assert(output65[65] == 0xA5);
    puts("PASS 7: 65 bytes");

    
    return 0;
}
