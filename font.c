#include <arch/philips/LPC214X.h>
#include "main.h"
#include "lcd.h"
#include "font.h"
//#include "font_terminal12.h"
//#include "font_consolas24.h"
//#include "font_1.h"

#define STATIC_FONT_TERMINAL8   ((unsigned char *)0x8000)   
#define STATIC_FONT_CONSOLAS24  ((unsigned char *)0x8300) 

s_font font[2];

void init_fonts () {
  font[0].width = 6;
  font[0].height = 8;
  font[0].alpha = 0;
  font[0].row_byte_length = 1;
  font[0].char_byte_length = 8;
  font[0].data = STATIC_FONT_TERMINAL8;
  
  font[1].width = 13;
  font[1].height = 24;
  font[1].alpha = 1;
  font[1].row_byte_length = 7;
  font[1].char_byte_length = 168;
  font[1].data = STATIC_FONT_CONSOLAS24;
}

s_font *get_font(int font_index) {
  return &font[font_index];
}

void draw_t8_char(unsigned short x, unsigned short y, char chr, int fore_color, int back_color) {
  int i,j;
  
  lcd_set_address(x,y,x+4,y+7);
    
  LCD_SET_DATA_MODE;
  
  for(i=0;i<8;i++) {
    unsigned char row = STATIC_FONT_TERMINAL8[(chr-32)*8+i];
    for (j=0;j<5;j++) {
      if (row & 0x80)
        lcd_data_set(back_color);
      else
        lcd_data_set(fore_color);
      row = row<<1;
      
      LCD_WRITE;
    }
  }
  
  /*for(i=0;i<8;i++) {
    unsigned char row = terminal8[(chr-32)*8+i];
    for (j=0;j<5;j++) {
      if (!(row & 0x80)) {
          lcd_set_address(x+j,y+i,x+j,y+i);
          LCD_SET_DATA_MODE;
          lcd_data_set(BLACK);
          LCD_WRITE;
      }
      row = row<<1;
    }
  }*/
}

void print_t8 (unsigned short x, unsigned short y, char *str, int fore_color, int back_color) {
  int i = 0;
  while (str[i] != '\0') {
    draw_t8_char(x + i*6, y, str[i], fore_color, back_color);
    i++;
  }
}

/*void draw_t12_char(unsigned short x, unsigned short y, char chr, unsigned int color) {
  int i,j;
  
  lcd_set_address(x,y,x+7,y+11);
    
  LCD_SET_DATA_MODE;
  
  for(i=0;i<12;i++) {
    unsigned char row = terminal12[(chr-32)*12+i];
    for (j=0;j<8;j++) {
      if (row & 0x80)
        lcd_data_set(color);
      else
        lcd_data_set(0);
      row = row<<1;
      
      LCD_WRITE;
    }
  }
}

void print_t12 (unsigned short x, unsigned short y, char *str, unsigned int color) {
  int i = 0;
  while (str[i] != '\0') {
    draw_t12_char(x + i*8, y, str[i], color);
    i++;
  }
}*/



  /*for (i = 0; i < 5; i++) {
    lcd_set_address(i*13+16, 16, i*13+28, 16+24);
    LCD_SET_DATA_MODE;
    
    s_font *f = get_font(1);
    
    unsigned char *l = &f->data[(test_string2[i]-32)*f->char_byte_length];
    
    for (y = 0; y < 24; y++) {
      
      unsigned char *row = &l[y*7];
      for (x = 0; x < 13; x++) {
        unsigned char ind = x/2;
        unsigned char pixel = row[ind];
        unsigned char o = x-(ind*2);
        if (o==0) {
          pixel = (pixel&0xf0) >> 4;
        }
        else {
          pixel = pixel & 0x0f;
        }
        
        lcd_data_set(palette[palette_indeks].color[pixel]);
        LCD_WRITE;
      }
    }
  }*/
