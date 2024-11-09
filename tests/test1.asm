; This test shows the utility of NAND to compute all possible bitwise operations.
    MOVI $4
    MOV R1, R0
    MOVI $F
    NAND R1, R0 ; This is and
    NAND R1, R1
    MOVI $9
    NAND R0, R0 ; This is or
    NAND R1, R1
    NAND R1, R0
    MOVI $8
    SHL R1, R0
    MOVI $D
    MOV R3, R0
    MOV R4, R1
    NAND R0, R0
    NAND R4, R0
    NAND R1, R1
    NAND R3, R1
    NAND R3, R4 ; This is xor
    MOV R0, R3
    HLT