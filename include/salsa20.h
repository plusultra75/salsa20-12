#ifndef SALSA20_H
#define SALSA20_H

#include <stdint.h>
#include <stddef.h>

void salsa20_init_state(uint32_t state[16], const uint8_t key[32],
                        const uint8_t nonce[8], uint64_t counter);

#endif
