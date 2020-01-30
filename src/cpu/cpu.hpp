#pragma once

// cpu



// INCLUDES

#include "defines.h"
#include "isa.hpp"
#include "memory.hpp"



namespace cpu_emu::cpu {

// CLASS DEFINITIONS

class Cpu final
{
// TYPE, CONSTEXPR MEMBERS
public:
    static constexpr auto iword_length = isa::iword_length;
    static constexpr auto word_length  = isa::word_length;
    static constexpr auto reg_count    = isa::reg_count;
    static constexpr auto reg_0_value  = isa::reg_0_value;
    using iword_t   = isa::iword_t;
    using word_t    = isa::word_t;
    using addr_t    = isa::addr_t;
    using instr_t   = isa::instr_t;
    using reg_arr_t = isa::reg_arr_t;
    using reg_idx_t = isa::reg_idx_t;


// DATA MEMBERS
private:
    reg_arr_t         reg_arr      = { reg_0_value, 0, STACK_START_ADDR };
    word_t           &sp_reg       = reg_arr[ isa::reg_idx_ns::sp ];
    word_t            pc_reg       = ENTRY_POINT_ADDR;
    const std::string mem_img_path = MEM_IMG_PATH;
    memory::Memory    mem          = memory::Memory( mem_img_path );


// CONSTRUCTOR, DESTRUCTOR DECLARATIONS
public:
    explicit Cpu( word_t pc_reg = ENTRY_POINT_ADDR,
                  word_t sp_reg = STACK_START_ADDR,
                  const std::string &mem_img_path = MEM_IMG_PATH );
    ~Cpu();


// PUBLIC MEMBER-FUNCTION DECLARATIONS
public:
    const reg_arr_t &get_reg_arr() const;
    const word_t &get_reg( reg_idx_t index ) const;
    const word_t &get_sp_reg() const;
    const word_t &get_pc_reg() const;
    void set_reg_arr( const reg_arr_t &reg_arr );
    void set_reg( reg_idx_t index, word_t value );
    void set_sp_reg( word_t value );
    void set_pc_reg( word_t value );
    void step();


// PRIVATE MEMBER-FUNCTION DECLARATIONS
private:
    iword_t fetch() const;
    iword_t load_iword( addr_t addr ) const;
    void execute( instr_t instr );
    void arith_r( instr_t instr );
    void load( instr_t instr );
    void fence( instr_t instr ) const;  // FIX; currently no-op
    void arith_i( instr_t instr );
    void jalr( instr_t instr );
    void system( instr_t instr );
    void ecall();
    void store( instr_t instr );
    void branch( instr_t instr );
    void auipc( instr_t instr );
    void lui( instr_t instr );
    void jal( instr_t instr );
    void _illegal( instr_t instr ) const;
};  // END class Cpu

}  // END namespace cpu_emu::cpu
