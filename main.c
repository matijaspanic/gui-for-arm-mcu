#include <arch/philips/LPC214X.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <math.h>

#include "main.h"
#include "font.h"
#include "timer.h"
#include "str.h"
#include "delay.h"
#include "lcd.h"
#include "tp.h"
#include "spi.h"
#include "uart.h"
#include "pll.h"
#include "printf.h"
#include "heap.h"
#include "gui.h"

// STATIC WRITE
/*const unsigned char dummy[2000] = {0x00};
#include "images/font_terminal8.h"
//#include "images/font_terminal12.h"
#include "images/font_consolas24.h"
#include "images/porsche1.h"
#include "images/road.h"
#include "images/city.h"*/

/*LOG("terminal8 %x\n", terminal8);
  //LOG("terminal12 %x\n", terminal12);
  LOG("consolas24 %x\n", consolas24);
  LOG("image_porsche %x\n", image_porsche);
  LOG("image_road %x\n", image_road);
  LOG("image_city %x\n", image_city);*/


void lprintf(char *fmt, ...)
{
  char str[1024];
  char *s = str; 

	va_list va;
	va_start(va,fmt);
	tfp_format(&s,putcp,fmt,va);
	putcp(&s,0);
	va_end(va);
	
	uart_send_string(str);
}

void init () {
  set_pll();

  MAMCR = 0;
  MAMTIM = 4;
  MAMCR = 2;

  // APBDIV (aka VPBDIV) controls APB clock (aka PCLK) in relation to processor clock (aka CCLK) 
  VPBDIV = VPBDIV_VALUE;
  
  uart_init();
    
  SCS = 0x00000001; // enable high speed GPIO0
  
	// set fast port 0
	FIO0DIR = 0xffffffff & ~(TP_SO | TP_IRQ | 0x10);
	FIO0PIN = 0x00000000 | TP_IRQ;
	
	timer0_init();
	
	/*PINSEL0 = 
    0x100 // p0.4 = SCK0
    | 0x400 // p0.5 = MISO0
    | 0x1000 // p0.6 = MOSI0
  ;  */
	
	/*S0SPCR = SPI_MSTR 
    | SPI_CPOL
    // | SPI_BitEnable
    // | SPI_Bits
    | SPI_CPHA
    //| SPI_LSBF
      
  ; 
  
  S0SPCCR = 8;*/
}

void dot(int x, int y, int size, unsigned int color) {
  int i;
  
  if (x <= 1)
    x = 2;
  //if (y == 0)
  //  y = 1;
    
  lcd_set_address(x-size,y-size,x+size,y+size);
  delay_nop(50);
  LCD_SET_DATA_MODE;
  delay_nop(50);
  
  lcd_data_set(color);
  for(i = 0; i < ((size*2)+1)*((size*2)+1); i++) {
    LCD_WRITE;
  }
}

int	main (void)
{
  int i, x, y, z1, z2, cx, cy;
  
  init();
      
  lcd_init();
  
  FIO0SET |= 0x40;
  
  // clear screen
  lcd_set_address(0,0,319,239);
  lcd_data_set(COLOR_WHITE);
  LCD_SET_DATA_MODE;
  for (i = 0; i < 153600; i+=2) {
    LCD_WRITE;
  }
  //lcd_clear(COLOR_WHITE);
     
  init_fonts();
            
  /*unsigned char *data = STATIC_PORSCHE;
  lcd_set_address(0,0,319,239);
  LCD_SET_DATA_MODE; // RS=1 - data
  for (i = 0; i < 153600; i+=2) {
    lcd_data_set_2(data[i], data[i+1]);
          
    LCD_WRITE;
  }*/
  //lcd_write_block(data, 153600);
  
  gui_init();
  
  
  
  LOG("heap: used=%d, allocated=%d\n", heap_get_used_bytes(), heap_get_allocated_bytes());
  
  int p;
  while (1) {   
    z1 = tp_get(TP_AXIS_Z1);
    z2 = tp_get(TP_AXIS_Z1);
    x = tp_get(TP_AXIS_X);
    y = tp_get(TP_AXIS_Y);
    
    p = (4096.0f/(float)y) * (((float)z1+(float)z2)/2.0f);

    if (p > 200) {
      cx = tp_cal(TP_AXIS_X, x);
      cy = tp_cal(TP_AXIS_Y, y);
      
      gui_touch(cx, cy);
    }
        
    //delay_ms(1);
  }
}

void IRQ_Routine (void) {
	while (1) ;
}

void FIQ_Routine (void)  {
	while (1) ;
}

void SWI_Routine (void)  {
	while (1) ;
}

void UNDEF_Routine (void) {
	while (1) ;
}

