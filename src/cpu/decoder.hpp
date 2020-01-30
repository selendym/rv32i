#pragma once

// instruction decoder



// INCLUDES

#include "isa.hpp"



namespace cpu_emu::cpu::decoder {

// FUNCTION DECLARATIONS

isa::instr_t decode( isa::iword_t instr_word );

}  // END namespace cpu_emu::cpu::decoder
