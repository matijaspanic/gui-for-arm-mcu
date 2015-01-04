.global lcd_clear
lcd_clear:

stmfd	sp!, {r4, r5, lr}

mov r4, r0, LSL#8   // shift input to data pins

ldr r0,=0x3fffc018  // r0 = &FIO0SET
ldr r1,=0x3fffc01c  // r1 = &FIO0CLR
        
ldr r2,=0x00000004  // WR pin
    
mov r3,#0x12C00     // r3 = 76800 

ldr r5,=0xffff0000

str r5, [r1]        // clear data
str r4, [r0]        // set data
  
loop_lcd_clear:
str r2,[r1]         // clear WR
str r2,[r0]         // set WR
add r3, r3, #-1     // r3 = r3 - 1
cmp r3, #0
bgt loop_lcd_clear  // if > goto loop

ldmfd	sp!, {r4, r5, pc}




.global lcd_write
lcd_write:

ldr r1,=0x3fffc018  // r0 = &FIO0SET
ldr r2,=0x3fffc01c  // r1 = &FIO0CLR
        
ldr r3,=0x00000004  // WR pin
  
loop_lcd_write:
str r3,[r2]         // clear WR
str r3,[r1]         // set WR
add r0, r0, #-1     // r3 = r3 - 1
cmp r0, #0
bgt loop_lcd_write  // if > goto loop

bx lr





.global lcd_write_block
lcd_write_block:
stmfd	sp!, {r4, r5, r6, r7, r8, lr}

add r1, r0, r1

ldr r2,=0x3fffc018  // r2 = &FIO0SET
ldr r3,=0x3fffc01c  // r3 = &FIO0CLR

ldr r4,=0x00000004  // WR pin
ldr r5,=0x00ffff04  // data pins and WR pin

ldr r8,=0x00ffff00

loop_lcd_write_block:

ldr	r6, [r0]        // load 32-bits from r0 address to r6 register

// write 1. pixel
mov r7, r6, LSL#8
and r7, r7, r8      // leave only data bits
str r5, [r3]        // clear data bits and WR
str r7, [r2]        // write data
str r4, [r2]        // set WR

// write 2. pixel
mov r7, r6, LSR#8
and r7, r7, r8      // leave only data bits
str r5, [r3]        // clear data bits and WR
str r7, [r2]        // write data
str r4, [r2]        // set WR

// go to next two pixels
add r0, r0, #4      // r0 += 4
cmp r0, r1          // cmp r0 to r1
blt loop_lcd_write_block           // if r0 < r1 goto loop2

ldmfd	sp!, {r4, r5, r6, r7, r8, pc} // vrati vrijednosti sa stacka; lr (return adress) se vraća u pc






.global lcd_write_block2
lcd_write_block2:
stmfd	sp!, {r4, r5, r6, r7, lr}

add r1, r0, r1

ldr r2,=0x3fffc018  // r2 = &FIO0SET
ldr r3,=0x3fffc01c  // r3 = &FIO0CLR

ldr r4,=0x00000004  // r4 = 0x04; WR pin
ldr r5,=0x00ffff04  // two higher bytes and WR pin

loop3:

// write 1. pixel
ldrb r6, [r0, #1]   // load 1. byte
mov r6, r6, LSL#8   // shift 8 bits to left

ldrb r7, [r0, #0]   // load 2. byte
orr r7, r6, r7      // join 1. and 2. byte
mov r7, r7, LSL#8   // shift all 8 bits to left
 
str r5, [r3]        // clear data bytes and WR
str r7, [r2]        // write data
str r4, [r2]        // set WR

// write 2. pixel
ldrb r6, [r0, #3]   // load 3. byte
mov r6, r6, LSL#8   // shift 8 bits to left

ldrb r7, [r0, #2]   // load 4. byte
orr r7, r6, r7      // clear lower two bytes
mov r7, r7, LSL#8   // shift all 8 bits to left

str r5, [r3]        // clear data bytes and WR
str r7, [r2]        // write data
str r4, [r2]        // set WR


// go to next two pixels
add r0, r0, #4      // r0 += 4
cmp r0, r1          // cmp r0 to r1
blt loop3           // if r0 < r1 goto loop2

ldmfd	sp!, {r4, r5, r6, r7, pc} // vrati vrijednosti sa stacka; lr (return adress) se vraća u pc
