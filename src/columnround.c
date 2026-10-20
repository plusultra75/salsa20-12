
#include <stdint.h>
#include "salsa20.h"

// Member 3: Column Round
// Input: state gồm 16 phần tử 32-bit
// Output: state được biến đổi trực tiếp

void columnround(uint32_t state[16])
{
    // Thứ tự 4 nhóm theo đặc tả Salsa20
    static const unsigned int indices[4][4] = {
        {0, 4, 8, 12},
        {5, 9, 13, 1},
        {10, 14, 2, 6},
        {15, 3, 7, 11}
    };

    for (unsigned int column = 0; column < 4; ++column) {
        uint32_t words[4];

        // Lấy 4 phần tử của nhóm
        for (unsigned int j = 0; j < 4; ++j) {
            words[j] = state[indices[column][j]];
        }

        // Gọi hàm của Member 1
        quarterround(words);

        // Ghi kết quả về đúng vị trí state
        for (unsigned int j = 0; j < 4; ++j) {
            state[indices[column][j]] = words[j];
        }
    }
}
