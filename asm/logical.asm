; This test shows the utility of NAND to compute all possible bitwise operations.

MOVL $5
NAND R0, R0 ; R0 contains NOT($5) finally

MOVL $4
MOV R1, R0
MOVL $F
NAND R1, R0 
NAND R1, R1 ; R1 contains $4 AND $F finally

MOVL $3
MOV R1, R0
MOVL $9
NAND R0, R0
NAND R1, R1
NAND R1, R0 ; R1 contains $3 OR $9 finally

MOVL $8
MOV R1, R0
MOVL $D
MOV R2, R0
MOV R3, R1
NAND R2, R2
NAND R3, R3
NAND R3, R0
NAND R2, R1
NAND R3, R2 ; R3 contains $8 XOR $D finally

HLT