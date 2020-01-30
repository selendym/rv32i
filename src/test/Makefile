.DEFAULT_GOAL := all



# COMPILER, FLAGS

CC := riscv64-unknown-elf-gcc
CFLAGS := \
	-std=c17 -Wall -Wextra -Wpedantic -pipe \
	-O3 -flto -ffreestanding -fno-reorder-functions \
	-march=rv32i -mabi=ilp32 -mno-strict-align \
	-static -nostartfiles -Wl,--Ttext=0x0 \
	$(CFLAGS)



# TARGETS, HEADERS, SOURCES

TARGET := test
TARGET_HEADERS := \
	util/asm_utils.h \
	util/io_utils.h
TARGET_SOURCES := \
	startfiles.S \
	util/asm_utils.c \
	util/io_utils.c \
	test.c  # 'startfiles.S' must come first

MEM := mem.img
MEM_CLEAN := $(MEM).clean



# RULES

.PHONY : all mem-clean reset-mem

all : $(TARGET) $(MEM_CLEAN) reset-mem

$(TARGET) : $(TARGET_HEADERS) $(TARGET_SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET_SOURCES)

mem-clean : $(MEM_CLEAN)
$(MEM_CLEAN) : $(TARGET)
	riscv64-unknown-elf-objcopy --output-target=binary $(TARGET) $(TARGET).bin
	dd bs=1M count=1 if=/dev/zero     of=$(MEM_CLEAN)
	dd bs=1M count=1 if=$(TARGET).bin of=$(MEM_CLEAN) conv=notrunc

reset-mem :
	cp --preserve=timestamps --force -- $(MEM_CLEAN) $(MEM)
