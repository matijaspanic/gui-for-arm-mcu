#include "timer.h"

void delay_us(int us) {
  timer0_reset();
  timer0_start();
  while (timer0_get_us() < us) {
    asm volatile ("NOP");
  }
	timer0_stop();
}

void delay_ms(int ms) {
  timer0_reset();
  timer0_start();
  while (timer0_get_ms() < ms) {
    asm volatile ("NOP");
  }
	timer0_stop();
}