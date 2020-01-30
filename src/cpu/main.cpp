// test main



// INCLUDES

#include "cpu.hpp"

// #include <cstdint>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <string>



namespace {

// USED NAMESPACES

using namespace cpu_emu::cpu;
// using namespace cpu_emu::isa;

}  // END namespace



// STATIC VARIABLES

static std::size_t current_step_count;
static std::size_t current_pc;
static std::size_t current_sp;



// EXIT HANDLER DEFINITIONS

static
void
at_exit()
{
    std::cout << std::dec;
    std::cout << "at_exit: current_step_count: " << current_step_count
            << std::endl;
    std::cout << std::hex << std::showbase;
    std::cout << "at_exit: current_pc: " << current_pc << std::endl;
    std::cout << "at_exit: current_sp: " << current_sp << std::endl;
}



// MAIN FUNCTION

int
main(
    int argc,
    char *argv[]
)
{
    if ( argc > 4 ) {
        std::cout << "Usage: " << argv[ 0 ] << " [<step_count>] [<pc>] [<sp>]"
                << std::endl;

        return EXIT_FAILURE;
    }

    std::size_t step_count = argc > 1
                           ? std::stoull( argv[ 1 ], nullptr, 0 )
                           : -1;
    Cpu::word_t pc = argc > 2
                   ? std::stoull( argv[ 2 ], nullptr, 0 )
                   : 0;
    Cpu::word_t sp = argc > 3
                   ? std::stoull( argv[ 3 ], nullptr, 0 )
                   : 0;

    Cpu cpu;
    if ( argc > 2 ) { cpu.set_pc_reg( pc ); }
    if ( argc > 3 ) { cpu.set_sp_reg( sp ); }

    if ( std::atexit( at_exit ) ) { return EXIT_FAILURE; }

    for (
        std::size_t i_ = 0;
        i_ < step_count;
        ++i_
    ) {
        current_step_count = i_ + 1;
        current_pc = cpu.get_pc_reg();
        current_sp = cpu.get_sp_reg();

        cpu.step();
    }

    return EXIT_SUCCESS;
}
