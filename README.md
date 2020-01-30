# rv32i

## Summary

**rv32i** is a simple [RISC-V] RV32I emulator.


## Overview

Below are some excerpts from the documentation; for the details, please consult `doc/doc.pdf`.

The subject of this project is the emulation of the [RISC-V instruction-set architecture (ISA)], more precisely, the "RV32I base integer instruction set" variant, which is a 32-bit ISA.


## Instructions for building and usage

### Main

The CLI for the emulator, `main`, is built by invoking `make [all]` (the brackets signify optionality) in the `src/cpu` directory.
After a successful compilation, the `main` executable is located at `src/test/main`.
Note that a GCC version supporting `-std=c++17` is required.

Usage: `./main [<step_count>] [<pc>] [<sp>]`

If `step_count` is not given, the largest possible value, `-1`, is used (with unsigned arithmetic, this will wrap around).
Note that unless required, `pc` and `sp` should not be set explicitly; these correspond to the initial program counter (pc), which should point to the address of `_start`, and the initial stack pointer (sp), which by default points to just past the end of the memory image.


### Test

The test program for the emulator, `test`, is built by invoking `make [all]` in the `src/test` directory.
After a successful compilation, two memory images, `mem.img.clean` and `mem.img` are created (the latter is simply a copy of the former).
Note that this step is optional, as a precompiled `mem.img.clean` is provided; otherwise, a GCC version supporting RISC-V bare-metal cross-compilation is required.

After an emulation run, the memory image file, `mem.img`, has most likely been modified by the run.
If desired, the image can be restored to a clean state by invoking `make reset-mem` in the `src/test` directory; this will also create the image if it does not exist.


[RISC-V]: https://en.wikipedia.org/wiki/RISC-V
[RISC-V instruction-set architecture (ISA)]: https://riscv.org/specifications/
