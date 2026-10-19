#include "salsa20.h"

/* Ban mot khoi dang phat trien: nguoi goi chi truyen length <= 64.
 * Counter bat dau bang 0. Chua ho tro du lieu nhieu khoi.
 * length = 0 khong ghi output; voi length > 0 can cac vung nho hop le.
 */
void salsa20_12_crypt(
    const uint8_t *input,
    uint8_t *output,
    size_t length,
    const uint8_t key[32],
    const uint8_t nonce[8]
)
{
    uint32_t state[16];
    uint8_t keystream[64];
    uint64_t counter = 0;
    
    if (length == 0) {
        return;
    }
    
    salsa20_init_state(state, key, nonce, counter);
    salsa20_12_block(state, keystream);

    for (size_t i =0; i < length; i++) {
        output[i] = input[i] ^ keystream[i];
    }
}
