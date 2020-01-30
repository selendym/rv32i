// cpu



// INCLUDES

#include "cpu.hpp"

#include "decoder.hpp"
#include "defines.h"
#include "isa.hpp"

#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <stdexcept>



namespace {

// USED NAMESPACES

using namespace cpu_emu::isa;

}  // END namespace



namespace cpu_emu::cpu {

// CONSTRUCTOR, DESTRUCTOR DEFINITIONS

Cpu::Cpu(
    word_t pc_reg,
    word_t sp_reg,
    const std::string &mem_img_path
):
    pc_reg( pc_reg ),
    mem_img_path( mem_img_path )
{
    this->set_sp_reg( sp_reg );
}

Cpu::~Cpu()
{}



// PUBLIC MEMBER-FUNCTION DEFINITIONS

const reg_arr_t &
Cpu::get_reg_arr() const
{
    return this->reg_arr;
}

const word_t &
Cpu::get_reg(
    reg_idx_t index
) const
{
    return this->reg_arr.at( index );
}

const word_t &
Cpu::get_sp_reg() const
{
    return this->sp_reg;
}

const word_t &
Cpu::get_pc_reg() const
{
    return this->pc_reg;
}

void
Cpu::set_reg_arr(
    const reg_arr_t &reg_arr
)
{
    this->reg_arr = reg_arr;
    this->reg_arr[ 0 ] = this->reg_0_value;
}

void
Cpu::set_reg(
    reg_idx_t index,
    word_t value
)
{
    if ( index == 0 ) { return; }  // register 0 has a fixed value

    this->reg_arr.at( index ) = value;
}

void
Cpu::set_sp_reg(
    word_t value
)
{
    this->sp_reg = value;
}

void
Cpu::set_pc_reg(
    word_t value
)
{
    this->pc_reg = value;
}

void
Cpu::step()  // FIX; handle exceptions
{
    this->execute( decoder::decode( this->fetch() ) );
}



// PRIVATE MEMBER-FUNCTION DEFINITIONS

iword_t
Cpu::fetch() const
{
    return this->load_iword( this->pc_reg );
}

iword_t
Cpu::load_iword(
    addr_t addr
) const
{
    return this->mem.lw( addr );
}

void
Cpu::execute(
    instr_t instr
)
{
    switch ( instr.opcode ) {
        case opcode_e::arith_r:  this->arith_r( instr );  break;
        case opcode_e::load:     this->load( instr );     break;
        case opcode_e::fence:    this->fence( instr );    break;
        case opcode_e::arith_i:  this->arith_i( instr );  break;
        case opcode_e::jalr:     this->jalr( instr );     break;
        case opcode_e::system:   this->system( instr );   break;
        case opcode_e::store:    this->store( instr );    break;
        case opcode_e::branch:   this->branch( instr );   break;
        case opcode_e::auipc:    this->auipc( instr );    break;
        case opcode_e::lui:      this->lui( instr );      break;
        case opcode_e::jal:      this->jal( instr );      break;

        default:  this->_illegal( instr );  break;
    }
    this->reg_arr[ 0 ] = this->reg_0_value;  // restore reg_0 value

    // increment pc if no jump/branch
    switch ( instr.mnem ) {
    // jal
        case mnem_e::JAL:
    // jalr
        case mnem_e::JALR:
    // branch
        case mnem_e::BEQ:
        case mnem_e::BNE:
        case mnem_e::BLT:
        case mnem_e::BGE:
        case mnem_e::BLTU:
        case mnem_e::BGEU:
            return;  // jump/branch

        default:  break;
    }
    // no jump/branch
    this->pc_reg += this->iword_length >> 3;
}

void
Cpu::arith_r(
    instr_t instr
)
{
    auto      &rd  = this->reg_arr[ instr.rd ];
    const auto rs1 = this->reg_arr[ instr.rs1 ];
    const auto rs2 = this->reg_arr[ instr.rs2 ];

    switch ( instr.mnem ) {
        case mnem_e::ADD:   rd = rs1 + rs2;                              break;
        case mnem_e::SUB:   rd = rs1 - rs2;                              break;
        case mnem_e::SLL:   rd = rs1 << word_extract( rs2, 4, 0 );       break;
        case mnem_e::SLT:   rd = static_cast< sword_t >( rs1 )
                               < static_cast< sword_t >( rs2 ) ? 1 : 0;  break;
        case mnem_e::SLTU:  rd = rs1 < rs2 ? 1 : 0;                      break;
        case mnem_e::XOR:   rd = rs1 ^ rs2;                              break;
        case mnem_e::SRL:   rd = rs1 >> word_extract( rs2, 4, 0 );       break;
        case mnem_e::SRA:   rd = static_cast< sword_t >( rs1 )
                                     >> word_extract( rs2, 4, 0 );       break;
        case mnem_e::OR:    rd = rs1 | rs2;                              break;
        case mnem_e::AND:   rd = rs1 & rs2;                              break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal arith_r." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

void
Cpu::load(
    instr_t instr
)
{
    auto       &rd  = this->reg_arr[ instr.rd ];
    const auto  rs1 = this->reg_arr[ instr.rs1 ];
    const auto &mem = this->mem;

    addr_t addr = rs1 + instr.imm;

    switch ( instr.mnem ) {
        // static casts needed for automatic sign-extension
        case mnem_e::LB:   rd = static_cast< int8_t  >( mem.lb( addr ) ); break;
        case mnem_e::LH:   rd = static_cast< int16_t >( mem.lh( addr ) ); break;
        case mnem_e::LW:   rd = mem.lw( addr );                           break;
        case mnem_e::LBU:  rd = mem.lb( addr );                           break;
        case mnem_e::LHU:  rd = mem.lh( addr );                           break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal load." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

void
Cpu::fence(
    instr_t instr
) const  // FIX; currently no-op
{
    switch ( instr.mnem ) {
        case mnem_e::FENCE:    break;
        case mnem_e::FENCE_I:  break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal fence." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

void
Cpu::arith_i(
    instr_t instr
)
{
    auto      &rd  = this->reg_arr[ instr.rd ];
    const auto rs1 = this->reg_arr[ instr.rs1 ];
    const auto imm = instr.imm;

    switch ( instr.mnem ) {
        case mnem_e::ADDI:   rd = rs1 + imm;                              break;
        case mnem_e::SLLI:   rd = rs1 << word_extract( imm, 4, 0 );       break;
        case mnem_e::SLTI:   rd = static_cast< sword_t >( rs1 )
                                < static_cast< sword_t >( imm ) ? 1 : 0;  break;
        case mnem_e::SLTIU:  rd = rs1 < imm ? 1 : 0;                      break;
        case mnem_e::XORI:   rd = rs1 ^ imm;                              break;
        case mnem_e::SRLI:   rd = rs1 >> word_extract( imm, 4, 0 );       break;
        case mnem_e::SRAI:   rd = static_cast< sword_t >( rs1 )
                                      >> word_extract( imm, 4, 0 );       break;
        case mnem_e::ORI:    rd = rs1 | imm;                              break;
        case mnem_e::ANDI:   rd = rs1 & imm;                              break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal arith_i." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

void
Cpu::jalr(
    instr_t instr
)
{
    auto      &pc  = this->pc_reg;
    auto      &rd  = this->reg_arr[ instr.rd ];
    const auto rs1 = this->reg_arr[ instr.rs1 ];

    rd = pc + (this->iword_length >> 3);
    pc = (rs1 + instr.imm) & ~word_mask( 0, 0 );
}

void
Cpu::system(
    instr_t instr
)
{
    // auto      &rd  = this->reg_arr[ instr.rd ];
    // const auto rs1 = this->reg_arr[ instr.rs1 ];
    // const auto imm = instr.imm;

    switch ( instr.mnem ) {
        case mnem_e::ECALL:   this->ecall();  break;
        case mnem_e::EBREAK:                  break;
        case mnem_e::CSRRW:                   break;
        case mnem_e::CSRRS:                   break;
        case mnem_e::CSRRC:                   break;
        case mnem_e::CSRRWI:                  break;
        case mnem_e::CSRRSI:                  break;
        case mnem_e::CSRRCI:                  break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal system." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

void
Cpu::ecall()
{
    // arguments
    auto &a0 = this->reg_arr[ reg_idx_ns::a0 ];
    auto &a1 = this->reg_arr[ reg_idx_ns::a1 ];
    auto &a2 = this->reg_arr[ reg_idx_ns::a2 ];
    auto &a3 = this->reg_arr[ reg_idx_ns::a3 ];
    auto &a4 = this->reg_arr[ reg_idx_ns::a4 ];
    auto &a5 = this->reg_arr[ reg_idx_ns::a5 ];
    auto &a6 = this->reg_arr[ reg_idx_ns::a6 ];
    auto &a7 = this->reg_arr[ reg_idx_ns::a7 ];

    switch ( a7 ) {
        case ECALL_SBRK:
            std::cout << std::hex << std::showbase;
            std::cout << "ECALL_SBRK:" << std::endl;
            std::cout << "  a0: " << a0 << std::endl;
            std::cout << "  a1: " << a1 << std::endl;
            std::cout << "  a2: " << a2 << std::endl;
            std::cout << "  a3: " << a3 << std::endl;
            std::cout << "  a4: " << a4 << std::endl;
            std::cout << "  a5: " << a5 << std::endl;
            std::cout << "  a6: " << a6 << std::endl;
            std::cout << "  a7: " << a7 << std::endl;

            if ( !a0 ) { a0 = HEAP_START_ADDR; }  // init heap start address
            a0 = a0;  // new program break; grant all requests

            break;
        case ECALL_IN_WORD:
            std::cin >> a0;
            if ( !std::cin ) { throw std::runtime_error( "Input error." ); }

            std::cin.ignore(
                std::numeric_limits< std::streamsize >::max(),
                '\n'
            );  // skip rest of the line

            break;
        case ECALL_IN_STR:
            std::cin.getline(
                reinterpret_cast< char * >( this->mem.get_mem_carr_nc( a0 ) ),
                TEST_INPUT_BUFFER_SIZE
            );
            if ( !std::cin ) { throw std::runtime_error( "Input error." ); }

            break;
        case ECALL_OUT_WORD:
            std::cout << std::hex << std::showbase
                      << std::setw( (this->word_length >> 2) + 2 )
                      << std::setfill( ' ' ) << std::internal;
            std::cout << a0 << std::endl;

            break;
        case ECALL_OUT_STR:
            std::cout << std::setw( 0 );
            std::cout << reinterpret_cast< const char * >(
                    this->mem.get_mem_carr( a0 ) ) << std::flush;

            break;
        case ECALL_ERR_WORD:
            std::cerr << std::hex << std::showbase
                      << std::setw( (this->word_length >> 2) + 2 )
                      << std::setfill( ' ' ) << std::internal;
            std::cerr << a0 << std::endl;

            break;
        case ECALL_ERR_STR:
            std::cerr << std::setw( 0 );
            std::cerr << reinterpret_cast< const char * >(
                    this->mem.get_mem_carr( a0 ) ) << std::flush;

            break;
        case ECALL_EXIT:
            std::exit( a0 );

            break;

        default:  throw std::runtime_error( "No matching ecall." );  break;
    }
}

void
Cpu::store(
    instr_t instr
)
{
    const auto rs1 = this->reg_arr[ instr.rs1 ];
    const auto rs2 = this->reg_arr[ instr.rs2 ];

    addr_t addr = rs1 + instr.imm;

    switch ( instr.mnem ) {
        case mnem_e::SB:  mem.sb( addr, static_cast< uint8_t  >( rs2 ) ); break;
        case mnem_e::SH:  mem.sh( addr, static_cast< uint16_t >( rs2 ) ); break;
        case mnem_e::SW:  mem.sw( addr, rs2 );                            break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal store." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
}

void
Cpu::branch(
    instr_t instr
)
{
    auto      &pc  = this->pc_reg;
    const auto rs1 = this->reg_arr[ instr.rs1 ];
    const auto rs2 = this->reg_arr[ instr.rs2 ];
    const auto imm = instr.imm;

    switch ( instr.mnem ) {
        case mnem_e::BEQ:   if ( rs1 == rs2 ) { pc += imm;  return; }  break;
        case mnem_e::BNE:   if ( rs1 != rs2 ) { pc += imm;  return; }  break;
        case mnem_e::BLT:   if ( static_cast< sword_t >( rs1 )
                               < static_cast< sword_t >( rs2 ) )
                                              { pc += imm;  return; }  break;
        case mnem_e::BGE:   if ( static_cast< sword_t >( rs1 )
                              >= static_cast< sword_t >( rs2 ) )
                                              { pc += imm;  return; }  break;
        case mnem_e::BLTU:  if ( rs1  < rs2 ) { pc += imm;  return; }  break;
        case mnem_e::BGEU:  if ( rs1 >= rs2 ) { pc += imm;  return; }  break;

        case mnem_e::_ILLEGAL:
            throw std::runtime_error( "Illegal branch." );  break;

        default:  throw std::logic_error( "Should not occur." );  break;
    }
    // branch not taken
    pc += this->iword_length >> 3;
}

void
Cpu::auipc(
    instr_t instr
)
{
    const auto pc = this->pc_reg;
    auto      &rd = this->reg_arr[ instr.rd ];

    rd = pc + instr.imm;
}

void
Cpu::lui(
    instr_t instr
)
{
    auto &rd = this->reg_arr[ instr.rd ];

    rd = instr.imm;
}

void
Cpu::jal(
    instr_t instr
)
{
    auto &pc = this->pc_reg;
    auto &rd = this->reg_arr[ instr.rd ];

    rd = pc + (this->iword_length >> 3);
    pc += instr.imm;
}

void
Cpu::_illegal(
    [[maybe_unused]] instr_t instr
) const
{
    throw std::runtime_error( "Illegal opcode." );
}

}  // END namespace cpu_emu::cpu
