; for reading the string = "Namaste" from the terminal
MOVL $7
MOV R2, R0
MOVL $0
MOV R1, R0
MOVL $1
SYSCALL

; for writing the string = "Namaste" to the terminal
MOVL $0
SYSCALL