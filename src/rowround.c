
#include "salsa20.h"

// Member 2: Row Round
// Bien doi 16 phan tu state bang 4 lan Quarter Round
void rowround(uint32_t state[16]) {

    // Thu tu cac phan tu trong 4 lan Quarter Round
    const int indices[4][4] = {
        {0, 1, 2, 3},
        {5, 6, 7, 4},
        {10, 11, 8, 9},
        {15, 12, 13, 14}
    };

    // Xu ly lan luot 4 hang
    for (int row = 0; row < 4; row++) {

        uint32_t temp[4];

        // Lay 4 phan tu theo thu tu quy dinh
        for (int col = 0; col < 4; col++) {
            temp[col] = state[indices[row][col]];
        }

        // Goi ham Quarter Round cua Member 1
        quarterround(temp);

        // Ghi ket qua vao vi tri ban dau cua state
        for (int col = 0; col < 4; col++) {
            state[indices[row][col]] = temp[col];
        }
    }
}
