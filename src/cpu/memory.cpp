// memory

// The current implementation uses `reinterpret_cast` with `memcpy` semantics.
// That is to say, when an integer is cast to another integer type,
// the underlying bit pattern and its address remain unaffected,
// while truncation or extension takes place if the type sizes differ.
// Also, unaligned accesses are permitted.
//
// That above is considered undefined behavior according to C++-standard.
// However, on x86_64, GCC supports this without problems; thus it is used.



// INCLUDES

#include "memory.hpp"

#include "../util/file_utils.hpp"
#include "isa.hpp"



namespace {

// USED NAMESPACES

using namespace cpu_emu::isa;

}  // END namespace



namespace cpu_emu::memory {

// CONSTRUCTOR, DESTRUCTOR DEFINITIONS

Memory::Memory(
    const std::string &file_path
)
{
    auto pair = util::mmap_file( file_path );

    this->mem_carr = pair.first;
    this->mem_size = pair.second;
}

Memory::~Memory()
{
    util::munmap_file( this->mem_carr, this->mem_size );
}



// PUBLIC MEMBER-FUNCTION DEFINITIONS

const uint8_t *
Memory::get_mem_carr(
    addr_t addr
) const
{
    if ( addr >= this->mem_size ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    return reinterpret_cast< const uint8_t * >( this->mem_carr + addr );
}

uint8_t *
Memory::get_mem_carr_nc(
    addr_t addr
)  // non-const
{
    if ( addr >= this->mem_size ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    return reinterpret_cast< uint8_t * >( this->mem_carr + addr );
}

const addr_t &
Memory::get_mem_size() const
{
    return this->mem_size;
}

uint8_t
Memory::lb(
    addr_t addr
) const
{
    if ( addr >= this->mem_size ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    return *reinterpret_cast< uint8_t * >( this->mem_carr + addr );
}

uint16_t
Memory::lh(
    addr_t addr
) const
{
    if ( addr >= this->mem_size - 1 ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    return *reinterpret_cast< uint16_t * >( this->mem_carr + addr );
}

uint32_t
Memory::lw(
    addr_t addr
) const
{
    if ( addr >= this->mem_size - 3 ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    return *reinterpret_cast< uint32_t * >( this->mem_carr + addr );
}

uint64_t
Memory::ld(
    addr_t addr
) const  // rv64i
{
    if ( addr >= this->mem_size - 7 ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    return *reinterpret_cast< uint64_t * >( this->mem_carr + addr );
}

void
Memory::sb(
    addr_t addr,
    uint8_t word8
)
{
    if ( addr >= this->mem_size ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    *reinterpret_cast< uint8_t * >( this->mem_carr + addr ) = word8;
}

void
Memory::sh(
    addr_t addr,
    uint16_t word16
)
{
    if ( addr >= this->mem_size - 1 ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    *reinterpret_cast< uint16_t * >( this->mem_carr + addr ) = word16;
}

void
Memory::sw(
    addr_t addr,
    uint32_t word32
)
{
    if ( addr >= this->mem_size - 3 ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    *reinterpret_cast< uint32_t * >( this->mem_carr + addr ) = word32;
}

void
Memory::sd(
    addr_t addr,
    uint64_t word64
)  // rv64i
{
    if ( addr >= this->mem_size - 7 ) {
        throw std::out_of_range( "Address out of bounds." );
    }

    *reinterpret_cast< uint64_t * >( this->mem_carr + addr ) = word64;
}

}  // END namespace cpu_emu::memory
