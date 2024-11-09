; This test shows the use of simple arithmetic operations.
    MOVI $2
    MOV R1, R0
    MOVI $3
    ADD R1, R0 ; Addition
    MOVI $D
    SUB R1, R0 ; Subtraction
    MOVI $8
    SHL R1, R0
    MOVI $C
    MUL R1, R0 ; Multiplication
    MOV R3, R0
    MOVI $4
    MOV R1, R0
    DIV R3, R1 ; Division
    HLT