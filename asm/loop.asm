MOVL $5 ; a = 5
MOV R1, R0
MOVL $0
ST R1, R0 ; store 5 in memory location 0
MOVL $1 
MOV R2, R0 ; ans = 1 
MOVL $1 
MOV R3, R0 ; loop variable initialization 
MUL R2, R3 ; loop process started
MOV R2, R0
MOVL $1
ADD R3, R0
MOVL $0
LD R1, R0
CMP R3, R1
MOVL $6
MOV R1, R0
MOVL $0
JGT R0, R1 ; break the loop if loop variable > a
MOVL $0
MOVH $F
MOV R1, R0 
MOVL $F
MOVH $F
JMP R0, R1 ; jump to -16 offset
HLT ; end of program,  R2 contains the answer