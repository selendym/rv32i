#pragma once

// memory



// INCLUDES

#include "isa.hpp"

#include <string>



namespace cpu_emu::memory {

// CLASS DEFINITIONS

class Memory final
{
// TYPE, CONSTEXPR MEMBERS
public:
    static constexpr auto addr_length = isa::addr_length;
    using addr_t   = isa::addr_t;
    using uint8_t  = isa::uint8_t;
    using uint16_t = isa::uint16_t;
    using uint32_t = isa::uint32_t;
    using uint64_t = isa::uint64_t;


// DATA MEMBERS
private:
    uint8_t *mem_carr;  // memory array; memory-mapped c-style array
    addr_t   mem_size;  // memory array size


// CONSTRUCTOR, DESTRUCTOR DECLARATIONS
public:
    explicit Memory( const std::string &file_path );
    ~Memory();


// PUBLIC MEMBER-FUNCTION DECLARATIONS
public:
    const uint8_t *get_mem_carr( addr_t addr = 0 ) const;
    uint8_t       *get_mem_carr_nc( addr_t addr = 0 );  // non-const
    const addr_t  &get_mem_size() const;
    uint8_t  lb( addr_t addr ) const;
    uint16_t lh( addr_t addr ) const;
    uint32_t lw( addr_t addr ) const;
    uint64_t ld( addr_t addr ) const;  // rv64i
    void sb( addr_t addr, uint8_t  word8 );
    void sh( addr_t addr, uint16_t word16 );
    void sw( addr_t addr, uint32_t word32 );
    void sd( addr_t addr, uint64_t word64 );  // rv64i
};  // END class Memory

}  // END namespace cpu_emu::memory
