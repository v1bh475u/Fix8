.label:
    MOVI R0, 2
    MOVI R1, 2
    ADD R0, R1

.loop:
    ; This is a comment
    ADD R0, R1
    CMP R0, R1
    JUMP .loop
    HALT
