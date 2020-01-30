// io utils



// INCLUDES

#include "../../cpu/defines.h"
#include "asm_utils.h"

#include <stdint.h>



// FUNCTION DEFINITIONS

void
print(
    const char *arg
)
{
    uint32_t _not_used;
    ecall( ECALL_OUT_STR, (uint32_t) arg, &_not_used );
}

void
println(
    const char *arg
)
{
    print( arg );  print( "\n" );
}

void
print_word(
    uint32_t arg
)
{
    uint32_t _not_used;
    ecall( ECALL_OUT_WORD, arg, &_not_used );
}

void
scan(
    char *arg
)
{
    uint32_t _not_used;
    ecall( ECALL_IN_STR, (uint32_t) arg, &_not_used );
}

void
scan_word(
    uint32_t *arg
)
{
    ecall( ECALL_IN_WORD, 0, arg );
}
