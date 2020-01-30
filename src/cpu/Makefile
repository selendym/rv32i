.DEFAULT_GOAL := all



# COMPILER, FLAGS

CXX := g++
CXXFLAGS := \
	-std=c++17 -Wall -Wextra -Wpedantic -pipe \
	-O3 -flto \
	-march=native -mtune=native \
	$(CXXFLAGS)



# TARGETS, HEADERS, SOURCES

TARGET := ../test/main
TARGET_HEADERS := \
	../util/bit_utils.hpp \
	../util/file_utils.hpp \
	isa.hpp \
	decoder.hpp \
	memory.hpp \
	cpu.hpp
TARGET_SOURCES := \
	../util/file_utils.cpp \
	decoder.cpp \
	memory.cpp \
	cpu.cpp \
	main.cpp



# RULES

.PHONY : all main

all : $(TARGET)

main : $(TARGET)
$(TARGET) : $(TARGET_HEADERS) $(TARGET_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET_SOURCES)
