// asm utils



// INCLUDES

#include "asm_utils.h"

#include <stdint.h>



// FUNCTION DEFINITIONS

void
ecall(
    uint32_t index,
    uint32_t arg,
    uint32_t *output
)
{
    __asm__ volatile (
    // asm
        "mv a0, %[arg];"
        "mv a7, %[index];"
        "ecall;"
        "mv %[output], a0;"
    // output
        : [output] "=r" (*output)
    // input
        : [index] "r" (index),
          [arg]   "r" (arg)
    // clobber
        : "a0", "a7", "memory"
    );
}
