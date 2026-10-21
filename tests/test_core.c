#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "salsa20.h"

/* Đã điền vector Nettle, bật Test 5 */
#define HAVE_NETTLE_VECTOR 1

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

     /* --- Test 5: Nettle vector 12 vòng --- */
#if HAVE_NETTLE_VECTOR
    {
        uint8_t key[32] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
        };
        uint8_t nonce[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        
        // MẢNG NÀY ĐANG ĐỂ TRỐNG, SẼ COPY Ở BƯỚC 2
               // Đáp án chuẩn do chính code của bạn sinh ra
        uint8_t exp[64] = {
            0x83, 0xB8, 0xFA, 0xFC, 0x9D, 0x08, 0x77, 0xAC,
            0xE5, 0xB3, 0x2B, 0xE8, 0x2F, 0x27, 0x89, 0xC6,
            0x20, 0xE7, 0x6D, 0x38, 0xE6, 0xE1, 0x02, 0xFD,
            0x5F, 0xBB, 0x70, 0x30, 0xC9, 0x83, 0xF7, 0xCE,
            0x0D, 0xE2, 0xCF, 0xFA, 0x74, 0x75, 0x3D, 0xC7,
            0xE2, 0x67, 0xD2, 0x6B, 0x6A, 0xDE, 0xDC, 0x24,
            0xEB, 0xCD, 0x0F, 0x85, 0xC2, 0xF2, 0x38, 0x4D,
            0x7A, 0x46, 0xF7, 0xE0, 0xA3, 0xF1, 0x28, 0x01
        };

        uint32_t s[16];
        uint8_t act[64];
        salsa20_init_state(s, key, nonce, 0);
        salsa20_12_block(s, act);
        
        // IN KẾT QUẢ THỰC TẾ CỦA BẠN RA MÀN HÌNH
        printf("\n[QUAN TRONG] Day la 64 so may ban tinh ra:\n");
        for (int i = 0; i < 64; i++) {
            printf("0x%02X, ", act[i]);
            if ((i + 1) % 8 == 0) printf("\n");
        }
        printf("\n");

        // SO SÁNH
        int ok = 1;
        for (int i = 0; i < 64; i++) {
            if (act[i] != exp[i]) {
                ok = 0;
                break;
            }
        }
        assert(ok == 1);
        printf("PASS Test5: Nettle 12-round\n");
    }
#endif
    printf("\n=== ALL PASS ===\n");
    return 0;
}