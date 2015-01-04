#include <arch/philips/LPC214X.h>

#include "tp.h"

void tp_write_bit (int bit) {
  FIO0CLR = TP_SCK | TP_SI;
  if (bit)
    FIO0SET = TP_SI;
  delay_us(1);
  FIO0SET = TP_SCK;
  delay_us(1);
}

void tp_write (int data) {
  int mask = 0x80;
  while (mask > 0) {
    tp_write_bit (data & mask);
    
    mask >>= 1;
  }
  FIO0CLR = TP_SI;
}

int tp_read () {
  int i;
  int data = 0;
  
  FIO0SET = TP_SCK;
  delay_us(1);
  
  for (i = 0; i < TP_PRECISION; i++) {
    data <<= 1;
    FIO0CLR = TP_SCK;
    delay_us(1);
    if (FIO0PIN & TP_SO) {
      data |= 0x01;
      //printChar(i*8, 64, '1', get_RGB_color(0xff,0xff,0xff));
    }
    else {
      //printChar(i*8, 64, '0', get_RGB_color(0xff,0xff,0xff));
    }
    FIO0SET = TP_SCK;
  }
  return data;
}

int tp_get(int axis) {
  if (axis == TP_AXIS_Z) {
    return (FIO0PIN & TP_IRQ) ? 0 : 1;
  }

  int i = 0;
  int value = 0;
  int control = 
    0x80 // S
    | axis 
    | (TP_PRECISION == 8 ? 0x08 : 0x00) // 8-bit mode
    //| 0x04 // SER/DFR
    //| 0x03 // power-down mode
  ;
  
  //for (;i < 4; i++) {
    tp_write(control);
    tp_write_bit(0);
    value += tp_read();
  //}
  
  //value /= 4;
    
  return value;
}

int tp_cal (int axis, int value) {
  if (axis == TP_AXIS_X) {
    value -= 320;
    value /= 11.18;
    if (value < 0) value = 0;
    if (value > 320) value = 320;
  }
  else if (axis == TP_AXIS_Y) {
    value -= 240;
    value /= 14.8;
    if (value < 0) value = 0;
    if (value > 240) value = 240;
  }
  
  return value;
}