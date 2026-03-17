# assembler-project
 Assembler for Custom CPU

## Overview
This project implements a full assembler in C for a custom CPU architecture.

The assembler translates assembly language into binary machine code using a two-pass approach.

## Features
- Macro expansion (pre-assembler stage)
- Two-pass assembler:
  - First pass: symbol table construction
  - Second pass: machine code generation
- Multiple addressing modes
- Instruction encoding
- Error detection and validation

## Technologies
- C
- Linux
- GCC
- Makefile

## How to Run
```bash
make
./assembler input_file.as
