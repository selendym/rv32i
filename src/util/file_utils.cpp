// file utils



// INCLUDES

#include "file_utils.hpp"

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <string>
#include <system_error>
#include <utility>

#include <fcntl.h>      // open
#include <sys/mman.h>   // mmap, munmap
#include <sys/stat.h>   // open, fstat
#include <sys/types.h>  // open, fstat
#include <unistd.h>     // fstat, close



namespace cpu_emu::util {

// FUNCTION DEFINITIONS

std::pair<
    std::uint8_t *,  // memory-map start address
    std::size_t      // memory-map size
>
mmap_file(
    const std::string &file_path
)
{
    auto fd = open( file_path.c_str(), O_RDWR );
    if ( fd == -1 ) {
        throw std::system_error( errno, std::system_category() );
    }

    struct stat stat_;
    if ( fstat( fd, &stat_ ) == -1 ) {
        throw std::system_error( errno, std::system_category() );
    }

    auto start_addr = mmap(
        NULL,  // addr
        stat_.st_size,  // length
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0  // offset
    );
    if ( start_addr == MAP_FAILED ) {
        throw std::system_error( errno, std::system_category() );
    }

    if ( close( fd ) == -1 ) {
        throw std::system_error( errno, std::system_category() );
    }

    return {
        reinterpret_cast< std::uint8_t * >( start_addr ),
        stat_.st_size
    };
}

void
munmap_file(
    std::uint8_t *start_addr,  // memory-map start address
    std::size_t size           // memory-map size
)
{
    if ( munmap( start_addr, size ) == -1 ) {
        throw std::system_error( errno, std::system_category() );
    }
}

}  // END cpu_emu::util
