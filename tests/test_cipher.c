#include <assert.h>
#include <stdio.h>
#include "salsa20.h"

/* Test co ban cho length = 0, 1, 3; chua kiem tra ranh gioi 64 byte.
 * Nettle Salsa20/12, khoa 256 bit 80 00 ... 00, nonce 00 ... 00:
 * keystream bat dau AF E4 11, nen ASCII "ABC" XOR cho EE A6 52.
 * Nguon: https://sources.debian.org/src/nettle/3.8.1-2/testsuite/salsa20-test.c/
 * Dap an khong lay tu output cua cipher.c.
 */
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

    puts("Cipher tests: passed");
    return 0;
}
