#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "salsa20.h"

/* Đổi thành 1 khi đã điền vector Nettle ở Test 5 */
#define HAVE_NETTLE_VECTOR 0

int main(void) {
    printf("=== Salsa20/12 CORE TESTS ===\n\n");

    /* --- Test 1: doubleround với vector spec.pdf (độc lập) --- */
    {
        uint32_t s[16] = {
            0xde501066, 0x6f9eb8f7, 0xe4fbbd9b, 0x454e3f57,
            0xb75540d3, 0x43e93a4c, 0x3a6f2aa0, 0x726d6b36,
            0x9243f484, 0x9145d1e8, 0x4fa9d247, 0xdc8dee11,
            0x054bf545, 0x254dd653, 0xd9421b6d, 0x67b276c1
        };
        const uint32_t e[16] = {
            0xccaaf672, 0x23d960f7, 0x9153e63a, 0xcd9a60d0,
            0x50440492, 0xf07cad19, 0xae344aa0, 0xdf4cfdfc,
            0xca531c29, 0x8e7943db, 0xac1680cd, 0xd503ca00,
            0xa74b2ad6, 0xbc331c5c, 0x1dda24c7, 0xee928277
        };
        doubleround(s);
        for (int i = 0; i < 16; i++) assert(s[i] == e[i]);
        printf("PASS Test1: doubleround (spec.pdf)\n");
    }

    /* --- Test 2: block structural (bắt 6 vòng + feed-forward + LE) --- */
    {
        uint32_t s[16] = {
            0x11111111, 0x22222222, 0x33333333, 0x44444444,
            0x55555555, 0x66666666, 0x77777777, 0x88888888,
            0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC,
            0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x00000001
        };
        uint32_t m[16];
        memcpy(m, s, sizeof(m));
        for (int r = 0; r < 6; r++) doubleround(m);
        for (int i = 0; i < 16; i++) m[i] += s[i];

        uint8_t exp[64], act[64];
        for (int i = 0; i < 16; i++) {
            exp[4*i+0] = (uint8_t)( m[i]        & 0xFF);
            exp[4*i+1] = (uint8_t)((m[i] >>  8) & 0xFF);
            exp[4*i+2] = (uint8_t)((m[i] >> 16) & 0xFF);
            exp[4*i+3] = (uint8_t)((m[i] >> 24) & 0xFF);
        }
        salsa20_12_block(s, act);
        assert(memcmp(act, exp, 64) == 0);
        printf("PASS Test2: block structural\n");
    }

    /* --- Test 3: input state không bị sửa (const) --- */
    {
        uint32_t s[16], backup[16];
        for (int i = 0; i < 16; i++) s[i] = i + 1;
        memcpy(backup, s, sizeof(s));
        uint8_t ks[64];
        salsa20_12_block(s, ks);
        assert(memcmp(s, backup, sizeof(s)) == 0);
        printf("PASS Test3: input unchanged\n");
    }

    /* --- Test 4: deterministic --- */
    {
        uint32_t s[16] = { 0xDEADBEEF };
        uint8_t a[64], b[64];
        salsa20_12_block(s, a);
        salsa20_12_block(s, b);
        assert(memcmp(a, b, 64) == 0);
        printf("PASS Test4: deterministic\n");
    }

    /* --- Test 5: Nettle vector 12 vòng (điền khi có) --- */
#if HAVE_NETTLE_VECTOR
    {
        /* TODO: điền từ Nettle test_salsa20r12 */
        uint8_t key[32]   = { 0x00 };
        uint8_t nonce[8]  = { 0x00 };
        uint8_t exp[64]   = { 0x00 };

        uint32_t s[16];
        uint8_t act[64];
        salsa20_init_state(s, key, nonce, 0);
        salsa20_12_block(s, act);
        assert(memcmp(act, exp, 64) == 0);
        printf("PASS Test5: Nettle 12-round\n");
    }
#else
    printf("SKIP Test5: chưa điền vector Nettle\n");
#endif

    printf("\n=== ALL PASS ===\n");
    return 0;
}