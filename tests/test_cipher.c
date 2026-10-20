#include <assert.h>
#include <stdio.h>
#include "salsa20.h"

/*
 * Dap an doc lap:
 * - Nettle, testsuite/salsa20-test.c: hai vector Salsa20/12 khoa 256 bit.
 *   https://github.com/gnutls/nettle/blob/master/testsuite/salsa20-test.c
 * - Cac mang zero_stream, mixed_stream va long_tail duoc sinh tu ma tham
 *   chieu libsodium 1.0.20, KHONG lay tu cipher/core/state cua bai lam.
 *   https://github.com/jedisct1/libsodium/tree/1.0.20-RELEASE/src/libsodium/crypto_stream/salsa2012/ref
 *   https://github.com/jedisct1/libsodium/tree/1.0.20-RELEASE/src/libsodium/crypto_core/salsa/ref
 *   Bo tham chieu da duoc doi chieu voi hai vector Nettle va SHA-256
 *   cua stream 4096 byte trong test/default/stream5.exp cua libsodium.
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

    // Keystream counter 0 kiem tra 64 byte dau.
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

    // Test 8: 128 byte
    uint8_t input128[128];
    uint8_t output128[129] = {0};
    for (int i = 0; i < 128; i++) {
        input128[i] = (uint8_t) i;
    }
    output128[128] = 0xA5;

    salsa20_12_crypt(input128, output128, 128, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output128[i] == (input128[i] ^ block0[i]));
    }
    for (int i = 0; i < 64; i++) {
        assert(output128[64 + i] == (input128[64 + i] ^ block1[i]));
    }
    assert(output128[128] == 0xA5);
    puts("PASS 8: 128 bytes");

    // Test 9: 130 byte
    uint8_t block2[64];
    salsa20_init_state(state, key, nonce, 2);
    salsa20_12_block(state, block2);

    uint8_t input130[130];
    uint8_t output130[131] = {0};
    for (int i = 0; i < 130; i++) {
        input130[i] = (uint8_t) i;
    }
    output130[130] = 0xA5;

    salsa20_12_crypt(input130, output130, 130, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output130[i] == (input130[i] ^ block0[i]));
    }
    for (int i = 0; i < 64; i++) {
        assert(output130[64 + i] == (input130[64 + i] ^ block1[i]));
    }
    for (int i = 0; i < 2; i++) {
        assert(output130[128 + i] == (input130[128 + i] ^ block2[i]));
    }
    assert(output130[130] == 0xA5);
    puts("PASS 9: 130 bytes, counters 0, 1, 2");

    // Test 10: giai ma 130 byte o test 9.
    uint8_t recovered130[131] = {0};
    recovered130[130] = 0xA5;

    salsa20_12_crypt(output130, recovered130, 130, key, nonce);
    for (int i = 0; i < 130; i++) {
        assert(recovered130[i] == input130[i]);
    }
    assert(recovered130[130] == 0xA5);
    puts("PASS 10: decrypt recovers 130 bytes");

    // Test 11: 127 byte
    uint8_t input127[127];
    uint8_t output127[128] = {0};
    for (int i = 0; i < 127; i++) {
        input127[i] = (uint8_t) i;
    }
    output127[127] = 0xA5;
    salsa20_12_crypt(input127, output127, 127, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output127[i] == (input127[i] ^ block0[i]));
    }
    for (int i = 0; i < 63; i++) {
        assert(output127[64 + i] == (input127[64 + i] ^ block1[i]));
    }
    assert(output127[127] == 0xA5);
    puts("PASS 11: 127 bytes");

    // Test 12: 129 byte
    uint8_t input129[129];
    uint8_t output129[130] = {0};
    for (int i = 0; i < 129; i++) {
        input129[i] = (uint8_t) i;
    }
    output129[129] = 0xA5;
    salsa20_12_crypt(input129, output129, 129, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output129[i] == (input129[i] ^ block0[i]));
        assert(output129[64 + i] == (input129[64 + i] ^ block1[i]));
    }
    assert(output129[128] == (input129[128] ^ block2[0]));
    assert(output129[129] == 0xA5);
    puts("PASS 12: 129 bytes");

    // Test 13: 192 byte
    uint8_t input192[192];
    uint8_t output192[193] = {0};
    for (int i = 0; i < 192; i++) {
        input192[i] = (uint8_t) i;
    }
    output192[192] = 0xA5;
    salsa20_12_crypt(input192, output192, 192, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(output192[i] == (input192[i] ^ block0[i]));
        assert(output192[64 + i] == (input192[64 + i] ^ block1[i]));
        assert(output192[128 + i] == (input192[128 + i] ^ block2[i]));
    }
    assert(output192[192] == 0xA5);
    puts("PASS 13: 192 bytes");

    // Test 14: 8 byte vector Nettle voi key 80 00 ... 00.
    const uint8_t nettle_expected1[8] = {
        0xAF, 0xE4, 0x11, 0xED, 0x1C, 0x4E, 0x07, 0xE4
    };
    const uint8_t zero_input8[8] = {0};
    uint8_t nettle_output1[9] = {0};
    nettle_output1[8] = 0xA5;
    salsa20_12_crypt(zero_input8, nettle_output1, 8, key, nonce);
    for (int i = 0; i < 8; i++) {
        assert(nettle_output1[i] == nettle_expected1[i]);
    }
    assert(nettle_output1[8] == 0xA5);
    puts("PASS 14: Nettle vector, first key");

    // Test 15: vector Nettle khac, key va nonce co byte khac 0.
    const uint8_t nettle_key[32] = {
        0x00, 0x53, 0xA6, 0xF9, 0x4C, 0x9F, 0xF2, 0x45,
        0x98, 0xEB, 0x3E, 0x91, 0xE4, 0x37, 0x8A, 0xDD,
        0x30, 0x83, 0xD6, 0x29, 0x7C, 0xCF, 0x22, 0x75,
        0xC8, 0x1B, 0x6E, 0xC1, 0x14, 0x67, 0xBA, 0x0D
    };
    const uint8_t nettle_nonce[8] = {
        0x0D, 0x74, 0xDB, 0x42, 0xA9, 0x10, 0x77, 0xDE
    };
    const uint8_t nettle_expected2[8] = {
        0x52, 0xE2, 0x0C, 0xF8, 0x77, 0x5A, 0xE8, 0x82
    };
    uint8_t nettle_output2[9] = {0};
    nettle_output2[8] = 0xA5;
    salsa20_12_crypt(zero_input8, nettle_output2, 8, nettle_key, nettle_nonce);
    for (int i = 0; i < 8; i++) {
        assert(nettle_output2[i] == nettle_expected2[i]);
    }
    assert(nettle_output2[8] == 0xA5);
    puts("PASS 15: Nettle vector");

    // Test 16: key nonce plaintext = 0, dap an tu libsodium.
    const uint8_t zero_key[32] = {0};
    const uint8_t zero_nonce[8] = {0};
    const uint8_t zero_input64[64] = {0};
    const uint8_t zero_stream[64] = {
        0xBD, 0x78, 0xA2, 0xF8, 0x11, 0x8A, 0x56, 0x3C,
        0x76, 0x1D, 0xB4, 0xF2, 0xFB, 0xE0, 0x55, 0xDA,
        0x97, 0xF9, 0x09, 0x88, 0xD2, 0x75, 0x94, 0xD9,
        0xC5, 0xDF, 0xD1, 0x3A, 0x3E, 0xFE, 0xAA, 0x3F,
        0x68, 0xF0, 0xD2, 0x56, 0x48, 0x50, 0xAD, 0xF5,
        0x01, 0x74, 0x33, 0x96, 0x8E, 0x4B, 0x34, 0x05,
        0xAC, 0x49, 0xA3, 0x95, 0x32, 0x12, 0x4F, 0xCD,
        0x6F, 0x47, 0xE4, 0x15, 0xC7, 0x02, 0x8A, 0x83
    };
    uint8_t zero_output[65] = {0};
    zero_output[64] = 0xA5;
    salsa20_12_crypt(zero_input64, zero_output, 64, zero_key, zero_nonce);
    for (int i = 0; i < 64; i++) {
        assert(zero_output[i] == zero_stream[i]);
    }
    assert(zero_output[64] == 0xA5);
    puts("PASS 16: zero key/nonce/plaintext, libsodium reference");

    // Test 17: 257 byte, key 00..1F va nonce A0..A7.
    // Dap an la keystream cua libsodium, counter 0, 1, 2, 3, 4.
    uint8_t mixed_key[32];
    uint8_t mixed_nonce[8];
    uint8_t mixed_input[257];
    uint8_t mixed_output[258] = {0};
    for (int i = 0; i < 32; i++) {
        mixed_key[i] = (uint8_t) i;
    }
    for (int i = 0; i < 8; i++) {
        mixed_nonce[i] = (uint8_t) (0xA0 + i);
    }
    for (int i = 0; i < 257; i++) {
        mixed_input[i] = (uint8_t) i;
    }
    const uint8_t mixed_stream[257] = {
        0x30, 0x65, 0x83, 0x06, 0x9A, 0x31, 0xF9, 0x83,
        0x0E, 0xCF, 0xEE, 0xF0, 0xA7, 0xAB, 0xD0, 0xB0,
        0xEB, 0x90, 0x0A, 0x27, 0xED, 0x40, 0x59, 0xB9,
        0x85, 0xEA, 0x07, 0xBD, 0x2F, 0x86, 0x01, 0xE1,
        0x7B, 0x82, 0xC8, 0x98, 0xD7, 0x36, 0xBA, 0x72,
        0xDC, 0x31, 0x14, 0x3A, 0xA9, 0x0F, 0xCA, 0xF4,
        0x9D, 0x0C, 0x1B, 0x19, 0xF7, 0xC0, 0xBC, 0xC5,
        0xD3, 0xA0, 0x11, 0xF6, 0x4C, 0xA4, 0x58, 0x19,
        0x4E, 0x3B, 0xBC, 0x38, 0x4B, 0xC0, 0xA4, 0x6B,
        0x53, 0x9F, 0xC7, 0x5A, 0x26, 0x78, 0x38, 0xC8,
        0xEA, 0xAC, 0xEF, 0x41, 0xE1, 0xBC, 0x4C, 0x08,
        0x70, 0x06, 0xCA, 0xE7, 0x14, 0x6F, 0xD1, 0x32,
        0x6E, 0x68, 0xFB, 0xEB, 0x9A, 0x4A, 0x7B, 0x69,
        0x13, 0x4F, 0x0C, 0x0C, 0xB4, 0x9F, 0xE1, 0xAA,
        0x8A, 0x2C, 0xBF, 0xD9, 0xA7, 0xD2, 0x5C, 0x4B,
        0xD5, 0x60, 0x01, 0x77, 0x7E, 0x83, 0xA0, 0xFE,
        0xC5, 0xD3, 0x6C, 0xCE, 0xF2, 0xD0, 0xCF, 0x04,
        0x06, 0x0A, 0x28, 0x40, 0x5C, 0xA6, 0xB1, 0xA2,
        0xD4, 0x33, 0xBB, 0x58, 0xF8, 0x51, 0x49, 0x1F,
        0x49, 0x43, 0x3F, 0x38, 0xCE, 0x52, 0x34, 0x65,
        0x35, 0x1E, 0xC8, 0x67, 0xCD, 0x14, 0x47, 0xED,
        0xE6, 0xD8, 0x2F, 0x83, 0xEA, 0x44, 0x18, 0xAB,
        0x67, 0xE9, 0xC5, 0x16, 0xA8, 0x50, 0x26, 0x43,
        0xB0, 0x31, 0x9A, 0x99, 0x6D, 0x23, 0xC5, 0x09,
        0x98, 0xBB, 0x90, 0x2E, 0x5D, 0x41, 0x98, 0x6F,
        0xA6, 0x2E, 0x39, 0x3C, 0x47, 0x67, 0x35, 0x9D,
        0x7B, 0xF4, 0xBE, 0xCA, 0xAF, 0x31, 0xAC, 0xF3,
        0xF0, 0xAC, 0xEF, 0x16, 0x6B, 0xFD, 0xF6, 0x5A,
        0xAA, 0x81, 0x2C, 0x45, 0xB0, 0x81, 0x31, 0x17,
        0x52, 0x4F, 0xFB, 0x59, 0xF6, 0x6C, 0x97, 0x96,
        0xC6, 0xAB, 0xF6, 0x77, 0x84, 0x66, 0x30, 0x1C,
        0x93, 0xD7, 0x8D, 0xE2, 0x91, 0xEB, 0x86, 0x1D,
        0xC5
    };
    mixed_output[257] = 0xA5;
    salsa20_12_crypt(mixed_input, mixed_output, 257, mixed_key, mixed_nonce);
    for (int i = 0; i < 257; i++) {
        assert(mixed_output[i] == (mixed_input[i] ^ mixed_stream[i]));
    }
    assert(mixed_output[257] == 0xA5);
    puts("PASS 17: 257 binary bytes, libsodium reference");

    // Test 18: giu nguyen input, key va nonce sau test 17.
    for (int i = 0; i < 257; i++) {
        assert(mixed_input[i] == (uint8_t) i);
    }
    for (int i = 0; i < 32; i++) {
        assert(mixed_key[i] == (uint8_t) i);
    }
    for (int i = 0; i < 8; i++) {
        assert(mixed_nonce[i] == (uint8_t) (0xA0 + i));
    }
    puts("PASS 18: input, key and nonce unchanged");

    // Test 19: ghi de len mang 130 byte.
    uint8_t inplace[131];
    for (int i = 0; i < 130; i++) {
        inplace[i] = input130[i];
    }
    inplace[130] = 0xA5;
    salsa20_12_crypt(inplace, inplace, 130, key, nonce);
    for (int i = 0; i < 130; i++) {
        assert(inplace[i] == output130[i]);
    }
    assert(inplace[130] == 0xA5);
    puts("PASS 19: in-place encryption");

    // Test 20: giai ma tren cung 1 mang duoc ma hoa test 19.
    salsa20_12_crypt(inplace, inplace, 130, key, nonce);
    for (int i = 0; i < 130; i++) {
        assert(inplace[i] == input130[i]);
    }
    assert(inplace[130] == 0xA5);
    puts("PASS 20: in-place decryption");

    // Test 21: ma hoa 130 byte o giua mang
    uint8_t shifted_input[132];
    uint8_t shifted_output[132] = {0};
    shifted_input[0] = 0x3C;
    shifted_input[131] = 0xC3;
    shifted_output[0] = 0xA5;
    shifted_output[131] = 0x5A;
    for (int i = 0; i < 130; i++) {
        shifted_input[1 + i] = input130[i];
    }
    salsa20_12_crypt(&shifted_input[1], &shifted_output[1], 130, key, nonce);
    for (int i = 0; i < 130; i++) {
        assert(shifted_output[1 + i] == output130[i]);
        assert(shifted_input[1 + i] == input130[i]);
    }
    assert(shifted_input[0] == 0x3C);
    assert(shifted_input[131] == 0xC3);
    assert(shifted_output[0] == 0xA5);
    assert(shifted_output[131] == 0x5A);
    puts("PASS 21: shifted buffers, before/after markers intact");

    // Test 22: ban ro toan FF
    uint8_t ff_input[65];
    uint8_t ff_output[66] = {0};
    for (int i = 0; i < 65; i++) {
        ff_input[i] = 0xFF;
    }
    ff_output[65] = 0xA5;
    salsa20_12_crypt(ff_input, ff_output, 65, key, nonce);
    for (int i = 0; i < 64; i++) {
        assert(ff_output[i] == (0xFF ^ block0[i]));
    }
    assert(ff_output[64] == (0xFF ^ block1[0]));
    assert(ff_output[65] == 0xA5);
    puts("PASS 22: FF bytes across a block boundary");

    // Test 23: moi lan goi ham phai bat dau lai tu counter 0.
    // Khong phai API streaming: khong tu nho counter cua lan goi truoc.
    uint8_t repeated_output[131] = {0};
    repeated_output[130] = 0xA5;
    salsa20_12_crypt(input130, repeated_output, 130, key, nonce);
    for (int i = 0; i < 130; i++) {
        assert(repeated_output[i] == output130[i]);
    }
    assert(repeated_output[130] == 0xA5);
    puts("PASS 23: repeated call starts at counter zero");

    // Test 24: cac byte UTF-8 cua chu "Tieng Viet" co dau.
    // Du lieu la byte, khong dung strlen va khong ma hoa ky tu ket thuc.
    const uint8_t utf8_input[14] = {
        0x54, 0x69, 0xE1, 0xBA, 0xBF, 0x6E, 0x67,
        0x20, 0x56, 0x69, 0xE1, 0xBB, 0x87, 0x74
    };
    uint8_t utf8_output[15] = {0};
    uint8_t utf8_recovered[15] = {0};
    utf8_output[14] = 0xA5;
    utf8_recovered[14] = 0xA5;
    salsa20_12_crypt(utf8_input, utf8_output, 14, key, nonce);
    for (int i = 0; i < 14; i++) {
        assert(utf8_output[i] == (utf8_input[i] ^ block0[i]));
    }
    salsa20_12_crypt(utf8_output, utf8_recovered, 14, key, nonce);
    for (int i = 0; i < 14; i++) {
        assert(utf8_recovered[i] == utf8_input[i]);
    }
    assert(utf8_output[14] == 0xA5);
    assert(utf8_recovered[14] == 0xA5);
    puts("PASS 24: UTF-8 bytes");

    // Test 25: 16385 byte, counter di qua 255 sang 256.
    const uint8_t long_tail[65] = {
        0xFE, 0xAE, 0x01, 0x64, 0x74, 0x51, 0xC2, 0x7B,
        0x75, 0xDB, 0xFC, 0xE9, 0xD4, 0x8E, 0xF8, 0xEE,
        0x6A, 0xF4, 0x00, 0xB4, 0xA3, 0xD6, 0xD5, 0x70,
        0x0E, 0x98, 0xEF, 0xE6, 0xD0, 0x94, 0x4F, 0x2F,
        0x57, 0xA5, 0x42, 0x17, 0x21, 0x7A, 0x13, 0x4D,
        0x55, 0x30, 0x69, 0x72, 0x01, 0xA9, 0x3A, 0xD0,
        0xB1, 0xC3, 0x5F, 0x2C, 0x16, 0x14, 0xAC, 0xF8,
        0xBB, 0x26, 0x3A, 0xC7, 0xAE, 0xA7, 0xF3, 0x4C,
        0x59
    };
    const uint8_t long_input[16385] = {0};
    uint8_t long_output[16386] = {0};
    uint8_t long_recovered[16386] = {0};
    long_output[16385] = 0xA5;
    long_recovered[16385] = 0xA5;
    salsa20_12_crypt(long_input, long_output, 16385, key, nonce);
    for (int i = 0; i < 65; i++) {
        assert(long_output[16320 + i] == long_tail[i]);
    }
    salsa20_12_crypt(long_output, long_recovered, 16385, key, nonce);
    for (int i = 0; i < 16385; i++) {
        assert(long_recovered[i] == 0);
    }
    assert(long_output[16385] == 0xA5);
    assert(long_recovered[16385] == 0xA5);
    puts("PASS 25: 16385 bytes, counters 255/256, libsodium tail");

    // Test 26: length 0 khong duoc doc/ghi input va output.
    salsa20_12_crypt(NULL, NULL, 0, key, nonce);
    puts("PASS 26: zero length with NULL input/output");

    return 0;
}
