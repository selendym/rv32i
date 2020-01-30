// startfiles



// INCLUDES

#include "../cpu/defines.h"



// DIRECTIVES

.global _start;
.global _exit;



// SECTIONS

.section .rodata;
.align 2;

start_enter_str:  .string "_start: Entered.\n";
start_main_str:   .string "_start: Calling main.\n";
start_exit_str:   .string "_start: Exiting.\n";
exit_enter_str:   .string "_exit: Entered.\n";
exit_loop_str:    .string "_exit: Terminal loop reached; ECALL_EXIT failed.\n";

sp_str:  .string "SP: ";
gp_str:  .string "GP: ";

// END .section .rodata

.section .text;
.align 2;

_start:
.option push;
.option norelax;
    la gp, __global_pointer$;  // init global pointer
.option pop;

    // li sp, STACK_START_ADDR;  // init stack pointer

    // print info
    la a0, start_enter_str;
    li a7, ECALL_OUT_STR;
    ecall;

    // print sp
    la a0, sp_str;
    li a7, ECALL_OUT_STR;
    ecall;
    mv a0, sp;
    li a7, ECALL_OUT_WORD;
    ecall;
    // print gp
    la a0, gp_str;
    li a7, ECALL_OUT_STR;
    ecall;
    mv a0, gp;
    li a7, ECALL_OUT_WORD;
    ecall;

    // print info
    la a0, start_main_str;
    li a7, ECALL_OUT_STR;
    ecall;

    jal main;
    mv s0, a0;  // save return value

    // print info
    la a0, start_exit_str;
    li a7, ECALL_OUT_STR;
    ecall;

    mv a0, s0;  // set arg
    j _exit;

_exit:
    mv s0, a0;  // save arg

    // print info
    la a0, exit_enter_str;
    li a7, ECALL_OUT_STR;
    ecall;

    mv a0, s0;  // set exit value
    li a7, ECALL_EXIT;
    ecall;

    // print info
    la a0, exit_loop_str;
    li a7, ECALL_OUT_STR;
    ecall;

1:
    j 1b;  // terminal loop; in case ECALL_EXIT fails

// END .section .text
