.global str_copy
str_copy:                                     
LDRB R2, [R1],#1  
STRB R2, [R0],#1  
CMP  R2, #0       
BNE  str_copy      
BX   lr           

    