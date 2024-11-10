;This test checks for utility of if..else statements
MOVL $1
MOV R1, R0
MOVL $0
CMP R0, R1
MOV R3, R0
MOVH $0
MOVL $0
LD R0, R3
MOVL $1
LD R0, R1
MOVH $0
MOVL $C
MOV R1, R0
MOVH $0
MOVL $0
JE R0, R1 ; Equality check
MOVH $0
MOVL $2
MOV R1, R0
MOVL $4
LD R1, R0
MOVH $0
MOVL $6
MOV R1, R0
MOVL $0
MOVH $0
JMP R0, R1
MOVL $2
MOVH $0
MOV R1, R0
MOVL $5
LD R1, R0

MOVL $1
MOV R1, R0
MOVL $0
CMP R0, R1
MOV R3, R0
MOVH $0
MOVL $0
LD R0, R3
MOVL $1
LD R0, R1
MOVH $0
MOVL $C
MOV R1, R0
MOVH $0
MOVL $0
JGT R0, R1 ; Greater than check
MOVH $0
MOVL $2
MOV R1, R0
MOVL $4
LD R1, R0
MOVH $0
MOVL $6
MOV R1, R0
MOVL $0
MOVH $0
JMP R0, R1
MOVL $2
MOVH $0
MOV R1, R0
MOVL $5
LD R1, R0

MOVL $1
MOV R1, R0
MOVL $0
CMP R0, R1
MOV R3, R0
MOVH $0
MOVL $0
LD R0, R3
MOVL $1
LD R0, R1
MOVH $0
MOVL $C
MOV R1, R0
MOVH $0
MOVL $0
JE R0, R1 
JGT R0, R1 ; Greater than equal to check
MOVH $0 ; Less than case
MOVL $2
MOV R1, R0
MOVL $4
LD R1, R0
MOVH $0
MOVL $6
MOV R1, R0
MOVL $0
MOVH $0
JMP R0, R1
MOVL $2
MOVH $0
MOV R1, R0
MOVL $5
LD R1, R0
HLT