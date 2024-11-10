# Documentation

This document tells about the way we made the project. It gives a comprehensive overview of all the “how” and “why” questions related to the project.

## ISA-Fix8

This part explains the reasons and thought process of the specific decisions taken while making the ISA which we have names fix8.

Our target is to make a fixed length 8-bit ISA. The main problem which we have to solve while making the ISA is that we have a constraint of 8-bits which automatically limits the following things:

1. Number of instructions and their functionality
2. Number and size of registers, which limits the immediate size
3. Types of addressing modes.
4. Size of memory in all respects (data memory, instruction memory, length of word).

So our target is to optimize the size of memory, number & size of registers and the number of instructions under the constraint of its 8-bit size. In essence, we have to try make a nearly Turing complete ISA with this constraint.

### Instructions

Number of instructions ≤ 2^i , where i = number of bits used for opcode in the instruction encoding.

So with a limited number of bits, we will have to go for a limited number of instructions which too would be very necessary ones.

After some brainstorming process on deciding ‘i’ we came to the decision that i = 4 would be optimal. So number of instructions = 16.

Intuitively, it is clear that the following 8 instructions have to be present there for an ISA:

1. Arithmetic instructions : add, sub, mul, div
2. Memory related : mov, st, ld
3. Ending the program: hlt (halt)

We would have stopped here with 2^3 = 8 instructions, but it would have made a very trivial calculator and not an ISA

Now for other requirements like if-else statements and loops, we require these 4 instructions: cmp, jmp, jgt, je.

So we have decided 12 out of the 16 instructions.

Now we need something to perform bitwise operations i.e. AND, OR, NOT & XOR.

We would have made 4 instructions, 1 for each of these. But we came with a great idea of making an instruction for bitwise NAND, which is the universal gate. So all the above bitwise operations can be performed by using a combination of NAND operations. So we saved 3 instructions by this smart move.

The immediate size that we can have is 4 bits (after using 4 bits for opcode). This makes the register size 4. But this would be very small. So we made 2 more instructions ‘movl’ and ‘movh’ which load the immediate to `R0`'s lower and upper 4 bits respectively. This enables us to make the register size 8 bits.

Now we are remaining with 1 more instruction, which we decided to have ‘syscall’. This will enable us to take text inputs give text outputs. This makes our ISA more Turing complete.

So this becomes the complete set of instructions of Fix8.

### Registers

The number of registers = 2^r , r = number of bits used to encode the registers. we have to minimize r and hence the number of registers.

Also, we are remaining with 4 bits only after using 4 bits for the opcode.

We would need at least 4 registers even for doing basic tasks. So we would require 2 bits for encoding the register. This gives us another facility (which is a need) that we would be able to give 2 operands with an instruction. 

The smart move of making ‘movl’ and ‘movh’ instructions enabled us to make the register size = 8.

### Memory

The memory is linear. A nicely big memory is required to be able to execute complex programs. So we have to optimize the memory size.

The size of word at each address becomes 8 bit by the virtue of register size.

The memory would have had been 4-bit (4-bit address of each word), because of immediate size, but we changed the addressing mode of ‘ld’ and ‘st’ functions (can be seen in the ISA definition) which enabled us to have an 8-bit memory by the virtue of register size of 8-bits.

We have kept the data memory and instruction memory separate. The above lines were about the data memory.

We could have made the instruction memory 8-bits as the offsets in instruction memory will be used by the jump instructions. But we made the jump instructions - ‘jmp’, ‘jgt’ and ‘je’ take 2 operands, which allowed us to make the instruction memory 16-bit and size of each word 8 bits.

We also tried for a data memory of 16-bits, with some change in instructions by using logical shift left, but it failed in executing some basic programs like if-else. Hence we rejected that model.

In this way, we finally came up with an 8-bit 256 B data memory and a 16-bit 64 KB instruction memory.