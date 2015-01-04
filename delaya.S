.global delay_nop
delay_nop:

loop:
add r0, r0, #-1
cmp r0, #0
bgt loop

bx lr
