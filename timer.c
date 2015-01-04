#include <arch/philips/LPC214X.h>
#include "main.h"

/*
timer0_reset();
timer0_start();
  
timer0_stop();
unsigned int t1 = timer0_get_us();
*/


void timer0_init() {
  T0PR = (PCLK/1000000)-1; // Prescale - tick = 1ns
  //T0MR0 =  1000; // Match register1s
  //T0MCR = 0x00000002;    // Restart on MR0
}

void timer0_reset() {
  // Counter Reset
  T0TCR = 0x02; 
  T0TCR = 0x00;
}

void timer0_start() {
  T0TCR = 0x01; // Counter Enable
}

void timer0_stop() {
  T0TCR = 0x00;
}

unsigned int timer0_get_ms() {
  return T0TC/1000;
}

unsigned int timer0_get_us() {
  return T0TC;
}

void timer1_init() {
  T1PR  = (PCLK/1000000)-1; // Prescale - tick = 1ns
  //T0MR0 =  1000; // Match register1s
  //T0MCR = 0x00000002;    // Restart on MR0
}

void timer1_reset() {
  // Counter Reset
  T1TCR = 0x02; 
  T1TCR = 0x00;
}

void timer1_start() {
  T1TCR = 0x01; // Counter Enable
}

void timer1_stop() {
  T1TCR = 0x00;
}

unsigned int timer1_get_ms() {
  return T1TC/1000;
}

unsigned int timer1_get_us() {
  return T1TC;
}
