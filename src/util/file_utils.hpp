#pragma once

// file utils



// INCLUDES

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>



namespace cpu_emu::util {

// FUNCTION DECLARATIONS

std::pair<
    std::uint8_t *,  // memory-map start address
    std::size_t      // memory-map size
>
mmap_file(
    const std::string &file_path
);

void
munmap_file(
    std::uint8_t *start_addr,  // memory-map start address
    std::size_t size           // memory-map size
);

}  // END cpu_emu::util
