// instruction decoder



// INCLUDES

#include "decoder.hpp"

#include "isa.hpp"

#include <stdexcept>



namespace {

// USED NAMESPACES

using namespace cpu_emu::isa;

}  // END namespace



namespace cpu_emu::cpu::decoder {

// FUNCTION DECLARATIONS

instr_t decode( iword_t instr_word );

static void decode_mnem( instr_t &instr );
static void decode_mnem_arith_r( instr_t &instr );
static void decode_mnem_load( instr_t &instr );
static void decode_mnem_fence( instr_t &instr );
static void decode_mnem_arith_i( instr_t &instr );
static void decode_mnem_jalr( instr_t &instr );
static void decode_mnem_system( instr_t &instr );
static void decode_mnem_store( instr_t &instr );
static void decode_mnem_branch( instr_t &instr );
static void decode_mnem_auipc( instr_t &instr );
static void decode_mnem_lui( instr_t &instr );
static void decode_mnem_jal( instr_t &instr );
static void decode_imm( instr_t &instr );



// FUNCTION DEFINITIONS

instr_t
decode(
    iword_t instr_word
)
{
    instr_t instr;

    instr.instr_word = instr_word;

    instr.opcode = opcode_e{ iword_extract( instr_word, 6, 0 ) };
    instr.funct3 = iword_extract( instr_word, 14, 12 );
    instr.funct7 = iword_extract( instr_word, 31, 25 );
    instr.rd     = iword_extract( instr_word, 11,  7 );
    instr.rs1    = iword_extract( instr_word, 19, 15 );
    instr.rs2    = iword_extract( instr_word, 24, 20 );

    // call order important
    decode_mnem( instr );  // sets `instr.{mnem,opcode_type}`
    decode_imm( instr );  // sets `instr.imm`

    return instr;
}



// STATIC FUNCTION DEFINITIONS

static
void
decode_mnem(
    instr_t &instr
)
{
    switch ( instr.opcode ) {
    // reg
        case opcode_e::arith_r:  decode_mnem_arith_r( instr );
            instr.opcode_type = opcode_type_e::reg;     break;
    // imm
        case opcode_e::load:     decode_mnem_load( instr );
            instr.opcode_type = opcode_type_e::imm;     break;
        case opcode_e::fence:    decode_mnem_fence( instr );
            instr.opcode_type = opcode_type_e::imm;     break;
        case opcode_e::arith_i:  decode_mnem_arith_i( instr );
            instr.opcode_type = opcode_type_e::imm;     break;
        case opcode_e::jalr:     decode_mnem_jalr( instr );
            instr.opcode_type = opcode_type_e::imm;     break;
        case opcode_e::system:   decode_mnem_system( instr );
            instr.opcode_type = opcode_type_e::imm;     break;
    // store
        case opcode_e::store:    decode_mnem_store( instr );
            instr.opcode_type = opcode_type_e::store;   break;
    // branch
        case opcode_e::branch:   decode_mnem_branch( instr );
            instr.opcode_type = opcode_type_e::branch;  break;
    // upper
        case opcode_e::auipc:    decode_mnem_auipc( instr );
            instr.opcode_type = opcode_type_e::upper;   break;
        case opcode_e::lui:      decode_mnem_lui( instr );
            instr.opcode_type = opcode_type_e::upper;   break;
    // jump
        case opcode_e::jal:      decode_mnem_jal( instr );
            instr.opcode_type = opcode_type_e::jump;    break;

        default:  instr.opcode_type = opcode_type_e::_illegal;  break;
    }
}

static
void
decode_mnem_arith_r(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  switch ( instr.funct7 ) {
            case 0b0000000:  instr.mnem = mnem_e::ADD;  break;
            case 0b0100000:  instr.mnem = mnem_e::SUB;  break;

            default:  instr.mnem = mnem_e::_ILLEGAL;  break;
        }  break;
        case 0b001:  instr.mnem = mnem_e::SLL;   break;
        case 0b010:  instr.mnem = mnem_e::SLT;   break;
        case 0b011:  instr.mnem = mnem_e::SLTU;  break;
        case 0b100:  instr.mnem = mnem_e::XOR;   break;
        case 0b101:  switch ( instr.funct7 ) {
            case 0b0000000:  instr.mnem = mnem_e::SRL;  break;
            case 0b0100000:  instr.mnem = mnem_e::SRA;  break;

            default:  instr.mnem = mnem_e::_ILLEGAL;  break;
        }  break;
        case 0b110:  instr.mnem = mnem_e::OR;    break;
        case 0b111:  instr.mnem = mnem_e::AND;   break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_load(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  instr.mnem = mnem_e::LB;   break;
        case 0b001:  instr.mnem = mnem_e::LH;   break;
        case 0b010:  instr.mnem = mnem_e::LW;   break;
        case 0b100:  instr.mnem = mnem_e::LBU;  break;
        case 0b101:  instr.mnem = mnem_e::LHU;  break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_fence(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  switch ( instr.instr_word & ~iword_mask( 27, 20 ) ) {
            case 0x0000000f:  instr.mnem = mnem_e::FENCE;    break;

            default:  instr.mnem = mnem_e::_ILLEGAL;  break;
        }  break;
        case 0b001:  switch ( instr.instr_word ) {
            case 0x0000100f:  instr.mnem = mnem_e::FENCE_I;  break;

            default:  instr.mnem = mnem_e::_ILLEGAL;  break;
        }  break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_arith_i(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  instr.mnem = mnem_e::ADDI;   break;
        case 0b001:  instr.mnem = mnem_e::SLLI;   break;
        case 0b010:  instr.mnem = mnem_e::SLTI;   break;
        case 0b011:  instr.mnem = mnem_e::SLTIU;  break;
        case 0b100:  instr.mnem = mnem_e::XORI;   break;
        case 0b101:  switch ( instr.funct7 ) {
            case 0b0000000:  instr.mnem = mnem_e::SRLI;  break;
            case 0b0100000:  instr.mnem = mnem_e::SRAI;  break;

            default:  instr.mnem = mnem_e::_ILLEGAL;  break;
        }  break;
        case 0b110:  instr.mnem = mnem_e::ORI;    break;
        case 0b111:  instr.mnem = mnem_e::ANDI;   break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_jalr(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  instr.mnem = mnem_e::JALR;  break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_system(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  switch ( instr.instr_word ) {
            case 0x00000073:  instr.mnem = mnem_e::ECALL;   break;
            case 0x00100073:  instr.mnem = mnem_e::EBREAK;  break;

            default:  instr.mnem = mnem_e::_ILLEGAL;  break;
        }  break;
        case 0b001:  instr.mnem = mnem_e::CSRRW;   break;
        case 0b010:  instr.mnem = mnem_e::CSRRS;   break;
        case 0b011:  instr.mnem = mnem_e::CSRRC;   break;
        case 0b101:  instr.mnem = mnem_e::CSRRWI;  break;
        case 0b110:  instr.mnem = mnem_e::CSRRSI;  break;
        case 0b111:  instr.mnem = mnem_e::CSRRCI;  break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_store(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  instr.mnem = mnem_e::SB;  break;
        case 0b001:  instr.mnem = mnem_e::SH;  break;
        case 0b010:  instr.mnem = mnem_e::SW;  break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_branch(
    instr_t &instr
)
{
    switch ( instr.funct3 ) {
        case 0b000:  instr.mnem = mnem_e::BEQ;   break;
        case 0b001:  instr.mnem = mnem_e::BNE;   break;
        case 0b100:  instr.mnem = mnem_e::BLT;   break;
        case 0b101:  instr.mnem = mnem_e::BGE;   break;
        case 0b110:  instr.mnem = mnem_e::BLTU;  break;
        case 0b111:  instr.mnem = mnem_e::BGEU;  break;

        default:  instr.mnem = mnem_e::_ILLEGAL;  break;
    }
}

static
void
decode_mnem_auipc(
    instr_t &instr
)
{
    instr.mnem = mnem_e::AUIPC;
}

static
void
decode_mnem_lui(
    instr_t &instr
)
{
    instr.mnem = mnem_e::LUI;
}

static
void
decode_mnem_jal(
    instr_t &instr
)
{
    instr.mnem = mnem_e::JAL;
}
// END decode_mnem*

static
void
decode_imm(
    instr_t &instr
)
{
    const auto &instr_word = instr.instr_word;
    auto &imm = instr.imm;

    switch ( instr.opcode_type ) {
        case opcode_type_e::imm:
            imm = iword_extract( instr_word, 30, 20 );
            // sign-extend; imm[word_length-1:11] = instr_word[31]
            switch ( iword_extract( instr_word, 31, 31 ) ) {
                case 0u:  imm &= ~word_mask( word_length - 1, 11 );  break;
                case 1u:  imm |=  word_mask( word_length - 1, 11 );  break;

                default:  throw std::logic_error( "Should not occur." );  break;
            }  break;
        case opcode_type_e::store:
            imm = iword_extract( instr_word, 11, 7 )
                | iword_extract( instr_word, 30, 25 ) << 5;
            // sign-extend; imm[word_length-1:11] = instr_word[31]
            switch ( iword_extract( instr_word, 31, 31 ) ) {
                case 0u:  imm &= ~word_mask( word_length - 1, 11 );  break;
                case 1u:  imm |=  word_mask( word_length - 1, 11 );  break;

                default:  throw std::logic_error( "Should not occur." );  break;
            }  break;
        case opcode_type_e::branch:
            imm = iword_extract( instr_word, 11, 8 ) << 1
                | iword_extract( instr_word, 30, 25 ) << 5
                | iword_extract( instr_word, 7, 7 ) << 11;
            // sign-extend; imm[word_length-1:12] = instr_word[31]
            switch ( iword_extract( instr_word, 31, 31 ) ) {
                case 0u:  imm &= ~word_mask( word_length - 1, 12 );  break;
                case 1u:  imm |=  word_mask( word_length - 1, 12 );  break;

                default:  throw std::logic_error( "Should not occur." );  break;
            }  break;
        case opcode_type_e::upper:
            imm = iword_extract( instr_word, 31, 12 ) << 12;
            // sign-extend; imm[word_length-1:31] = instr_word[31]
            imm = static_cast< siword_t >( imm );
            break;
        case opcode_type_e::jump:
            imm = iword_extract( instr_word, 30, 21 ) << 1
                | iword_extract( instr_word, 20, 20 ) << 11
                | iword_extract( instr_word, 19, 12 ) << 12;
            // sign-extend; imm[word_length-1:20] = instr_word[31]
            switch ( iword_extract( instr_word, 31, 31 ) ) {
                case 0u:  imm &= ~word_mask( word_length - 1, 20 );  break;
                case 1u:  imm |=  word_mask( word_length - 1, 20 );  break;

                default:  throw std::logic_error( "Should not occur." );  break;
            }  break;
        case opcode_type_e::reg:
        case opcode_type_e::_illegal:
            break;  // no immediate

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

}  // END namespace cpu_emu::cpu::decoder
