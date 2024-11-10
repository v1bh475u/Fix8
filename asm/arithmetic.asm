; This test shows the use of simple arithmetic operations.

MOVL $2
MOV R1, R0
MOVL $3
ADD R1, R0 ; Addition

MOVL $D
SUB R1, R0 ; Subtraction

MOVL $5
MUL R1, R0 ; Multiplication

MOV R3, R0
MOVL $4
MOV R1, R0
DIV R3, R1 ; Division

HLT