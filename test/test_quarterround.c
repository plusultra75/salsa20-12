#include <stdio.h>
#include <assert.h>
#include "salsa20.h"

int main(void) {
//P1: kiem tra ham xoay trai rotl32 

// Kiem tra rotl32: xoay du 32 bit phai ve gia tri ban dau
assert(rotl32(13, 32) == 13);

// Kiểm tra trường hợp không xoay bit
assert(rotl32(13, 0) == 13);

// Kiểm tra xoay trái 2 bit
assert(rotl32(13, 2) == 52);

// Kiểm tra xoay quá 32 bit
assert(rotl32(13, 33) == 26);
assert(rotl32(13, 64) == 13);

// Kiểm tra bit ngoài cùng bên trái quay về bên phải
assert(rotl32(0x80000000u, 1) == 1u);

// Kiem tra xoay voi gia tri 32 bit
assert(rotl32(0x12345678u, 8) == 0x34567812u);

printf("Test rotl32 thanh cong\n");

// PHAN 2: KIEM TRA HAM QUARTERROUND

  //Tat ca phan tu bang 0
    uint32_t a[4] = {0, 0, 0, 0};
    quarterround(a);

    for (int i = 0; i < 4; i++) {
        assert(a[i] == 0u);
    }

    //Chi x[0] bang 1
    uint32_t b[4] = {1, 0, 0, 0};
    quarterround(b);

    assert(b[0] == 0x08008145u);
    assert(b[1] == 0x00000080u);
    assert(b[2] == 0x00010200u);
    assert(b[3] == 0x20500000u);

    //Chi x[1] bang 1
    uint32_t c[4] = {0, 1, 0, 0};
    quarterround(c);

    assert(c[0] == 0x88000100u);
    assert(c[1] == 0x00000001u);
    assert(c[2] == 0x00000200u);
    assert(c[3] == 0x00402000u);

    //Chi x[2] bang 1
    uint32_t d[4] = {0, 0, 1, 0};
    quarterround(d);

    assert(d[0] == 0x80040000u);
    assert(d[1] == 0x00000000u);
    assert(d[2] == 0x00000001u);
    assert(d[3] == 0x00002000u);

    //Chi x[3] bang 1
    uint32_t e[4] = {0, 0, 0, 1};
    quarterround(e);

    assert(e[0] == 0x00048044u);
    assert(e[1] == 0x00000080u);
    assert(e[2] == 0x00010000u);
    assert(e[3] == 0x20100001u);

    // Mang so thong thuong
    uint32_t f[4] = {1, 2, 3, 4};
    quarterround(f);

    assert(f[0] == 2552136791u);
    assert(f[1] == 642u);
    assert(f[2] == 329219u);
    assert(f[3] == 2702221316u);

    // Gia tri lon nhat cua uint32_t
    uint32_t g[4] = {
        0xFFFFFFFFu, 0xFFFFFFFFu,
        0xFFFFFFFFu, 0xFFFFFFFFu
    };
    quarterround(g);

    assert(g[0] == 0xF8038143u);
    assert(g[1] == 0x00000080u);
    assert(g[2] == 0xFFFF01FFu);
    assert(g[3] == 0x1FB00000u);

    printf("Tat ca Unit Test thanh cong");

    return 0;
}