#pragma once

// shared defines



// DEFINES

#ifndef MEM_IMG_PATH
#define MEM_IMG_PATH "mem.img"
#endif

#ifndef ENTRY_POINT_ADDR
#define ENTRY_POINT_ADDR 0x0  // must match the address of `_start`
#endif

#ifndef STACK_START_ADDR
#define STACK_START_ADDR 0x100000  // 1 MiB; may be overridden by `_start`
#endif

#ifndef HEAP_START_ADDR
#define HEAP_START_ADDR 0x80000  // 512 KiB; must be greater than `_end`
#endif

#ifndef TEST_INPUT_BUFFER_SIZE
#define TEST_INPUT_BUFFER_SIZE 0x100  // 256 bytes
#endif

// ecall function indices
// sbrk
#define ECALL_SBRK     0x0d6  // 214
// std::cin
#define ECALL_IN_WORD  0x100
#define ECALL_IN_STR   0x101
// std::cout
#define ECALL_OUT_WORD 0x110
#define ECALL_OUT_STR  0x111
// std::cerr
#define ECALL_ERR_WORD 0x120
#define ECALL_ERR_STR  0x121
// std::exit
#define ECALL_EXIT     0x1F0
