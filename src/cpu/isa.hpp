#pragma once

// instruction-set architecture



// INCLUDES

#include "../util/bit_utils.hpp"

#include <array>
#include <cstdint>



namespace cpu_emu::isa {

inline namespace rv32i {

// TYPE ALIASES, CONSTEXPR DEFINITIONS

using std::uint8_t,  std::int8_t;
using std::uint16_t, std::int16_t;
using std::uint32_t, std::int32_t;
using std::uint64_t, std::int64_t;

constexpr unsigned iword_length = 32;  // instruction bit width
using iword_t = uint32_t;  // instruction word type
using siword_t = int32_t;  // instruction word type; signed

constexpr unsigned word_length = 32;  // register bit width
using word_t = uint32_t;  // register word type
using sword_t = int32_t;  // register word type; signed

constexpr unsigned addr_length = word_length;  // memory address bit width
using addr_t = word_t;  // memory address type

constexpr unsigned reg_count = 32;  // register count
constexpr word_t reg_0_value = 0;  // register-0 fixed value

using reg_arr_t = std::array< word_t, reg_count >;
using reg_idx_t = reg_arr_t::size_type;



// FUNCTION DECLARATIONS

constexpr auto iword_mask = util::bit_mask< iword_t, iword_length >;
constexpr auto iword_extract = util::bit_extract< iword_t, iword_length >;

constexpr auto word_mask = util::bit_mask< word_t, word_length >;
constexpr auto word_extract = util::bit_extract< word_t, word_length >;

constexpr auto addr_mask = word_mask;
constexpr auto addr_extract = word_extract;



// NAMESPACE WRAPPERS

// These names are conventional and have no inherent meaning to the ISA.
// The values range from 0 to 31 and correspond to the register indices.
// Note that the unscoped enum is to allow implicit conversions.
// Note that `s0` and `fp` point to the same register.
namespace reg_idx_ns {
    enum : reg_idx_t
    {
        zero = 0,  // constant 0
        ra,  // return address
        sp,  // stack pointer
        gp,  // global pointer
        tp,  // thread pointer
        t0, t1, t2,  // temp
        s0 = 8, fp = 8,  // saved / frame pointer
        s1,  // saved
        a0, a1,  // function args / return values
        a2, a3, a4, a5, a6, a7,  // function args
        s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,  // saved
        t3, t4, t5, t6,  // temp
    };  // END enum
}  // END namespace reg_idx_ns



// ENUM CLASS DEFINITIONS

enum class opcode_type_e
{
// opcode format types
    reg,
    imm,
    store,
    branch,
    upper,
    jump,

// other
    _illegal,
};  // END enum class opcode_type_e

enum class opcode_e : iword_t
{
// r-type
    arith_r = 0b0110011,  // 'op'
// i-type
    load    = 0b0000011,
    fence   = 0b0001111,  // 'misc_mem'
    arith_i = 0b0010011,  // 'op_imm'
    jalr    = 0b1100111,
    system  = 0b1110011,
// s-type
    store   = 0b0100011,
// b-type
    branch  = 0b1100011,
// u-type
    auipc   = 0b0010111,
    lui     = 0b0110111,
// j-type
    jal     = 0b1101111,
};  // END enum class opcode_e

// Note that grouped mnems without space in between them share
// `funct3` but differ in `funct7` or otherwise; e.g., 'ADD,SUB'.
enum class mnem_e
{
// r-type
    // arith_r
    ADD,SUB,  SLL,  SLT,  SLTU,  XOR,  SRL,SRA,  OR,  AND,
// i-type
    // load
    LB,  LH,  LW,  LBU,  LHU,
    // fence
    FENCE,  FENCE_I,
    // arith_i
    ADDI,  SLLI,  SLTI,  SLTIU,  XORI,  SRLI,SRAI,  ORI,  ANDI,
    // jalr
    JALR,
    // system
    ECALL,EBREAK,  CSRRW,  CSRRS,  CSRRC,  CSRRWI,  CSRRSI,  CSRRCI,
// s-type
    // store
    SB,  SH,  SW,
// b-type
    // branch
    BEQ,  BNE,  BLT,  BGE,  BLTU,  BGEU,
// u-type
    // auipc
    AUIPC,
    // lui
    LUI,
// j-type
    // jal
    JAL,

// other
    _ILLEGAL,
};  // END enum class mnem_e



// STRUCT DEFINITIONS

struct instr_t
{
// raw instruction word
    iword_t instr_word;
// instruction-word bit fields
    opcode_e opcode;  // instr_word[ 6: 0]
    iword_t funct3;   // instr_word[14:12]
    iword_t funct7;   // instr_word[31:25]
    iword_t rd;       // instr_word[11: 7]
    iword_t rs1;      // instr_word[19:15]
    iword_t rs2;      // instr_word[24:20]
    word_t imm;  // variant; depends on opcode format type; sign-extended
// decoded instruction and type
    opcode_type_e opcode_type;  // opcode format type
    mnem_e mnem;  // mnemonic; 'fully resolved opcode'; e.g., 'XOR'
};  // END struct instr_t

}  // END inline namespace rv32i

}  // END namespace cpu_emu::isa
