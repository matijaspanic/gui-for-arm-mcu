#include <math.h>

#include "main.h"
#include "heap.h"
#include "gui.h"
#include "lcd.h"
#include "font.h"
#include "str.h"
#include "delay.h"
#include "timer.h"

//******************************************************************************
// COLORS
//******************************************************************************
const unsigned int const_color[16] = {
  COLOR_WHITE, // 0
  COLOR_BLACK, // 1
  COLOR_RED, // 2
  COLOR_GREEN, // 3
  COLOR_BLUE, // 4
  COLOR_YELLOW, // 5
  COLOR_CYAN, // 6
  COLOR_MAGENTA, // 7
  COLOR_GRAY25, // 5
  COLOR_GRAY50, // 9
  COLOR_GRAY75 // 10
};

//******************************************************************************
// PALETTE
//******************************************************************************

const unsigned int const_palette[2][16] = {
  {
    // 0 - 255
    0x0000,
    0x1082,
    0x2104,
    0x31A6,
    0x4228,
    0x52AA,
    0x632C,
    0x73AE,
    0x8C51,
    0x9CD3,
    0xAD55,
    0xBDD7,
    0xCE59,
    0xDEFB, 
    0xEF7D,
    0xFFFF,
  },
  {
    // 0 - 180
    0x0000,
    0x0861,
    0x18C3,
    0x2124,
    0x3186,
    0x39E7,
    0x4A49,
    0x52AA,
    0x630C,
    0x6B6D,
    0x7BCF,
    0x8430,
    0x9492,
    0x9CF3,
    0xA554,
    0xB596  
  }
};

void gui_draw_object_text (unsigned char pos_x, unsigned char pos_y, s_gui_object_text *text) {
  unsigned char row;
  int color;
  int y, x;
  
  //LOG("text: %d %d\n", pos_x, pos_y);
  
  unsigned char *c1;
  unsigned char *c2;
  
  s_font *font = get_font(text->font_indeks);
  unsigned int *palette = const_palette[text->palette_index];
  
  // broj znakova stringa
  unsigned char string_length_ch = strlen(text->string);
  
  // dužina stringa u pikelima
  int string_length_x_px = string_length_ch * font->width;
    
  // velièina objekta u pikselima
  unsigned int object_size_x_px = text->size_x*8;
  unsigned int object_size_y_px = text->size_y*8;
  
  // pozicija stringa u objektu u piskelima
  int start_position_of_string_x_px = 0;
  int start_position_of_string_y_px = 0;
  if (text->align==1)
    start_position_of_string_x_px = (object_size_x_px - string_length_x_px) / 2;
  else if (text->align==2)
    start_position_of_string_x_px = object_size_x_px - string_length_x_px;
  
  // pozicija u stringu od koje pocinjemo crtati u pikselima 
  int block_pos_in_string_x_px = ((pos_x - text->pos_x) * 8) - start_position_of_string_x_px;
  int block_pos_in_string_y_px = ((pos_y - text->pos_y) * 8) - start_position_of_string_y_px;
  
  if (block_pos_in_string_x_px + 8 < 0 || block_pos_in_string_x_px >= string_length_x_px) {
    lcd_data_set(palette[15]);
    for (x = 0; x < 64; x++) {
      LCD_WRITE;
    }
  }
  else
  {
    /*if (block_pos_in_string_x_px + font->width >= 0) {
      c1 = &font->data[(text->string[char_indeks_1]-32) * font->char_byte_length];
      LOG("a\n");
    }
    else {
      c1 = font->data;
      //c1 = &font->data[(text->string['B']-32) * font->char_byte_length];
      LOG("b\n");
    }*/
    int char_indeks_1 = 0;
    if (block_pos_in_string_x_px < 0) {
      c1 = font->data;
    }
    else {
      // prvi znak kojeg crtamo u ovom blocku
      char_indeks_1 = block_pos_in_string_x_px / font->width;
      c1 = &font->data[(text->string[char_indeks_1]-32) * font->char_byte_length];
    }
    
    // prvi piksel u slovu kojeg crtamo u gornjem desnom kutu blocka
    char char_start_pos_x_px = (block_pos_in_string_x_px + font->width) % font->width;
    char char_start_pos_y_px = block_pos_in_string_y_px;    
    
    // dali prvo slovo svojom širinom doseže kraj bloka, ako ne, crtamo i slijedeæe slovo u ovom bloku
    if (font->width - char_start_pos_x_px < 8) {
      int char_indeks_2;
      if (block_pos_in_string_x_px < 0)
        char_indeks_2 = 0;
      else
        char_indeks_2 = char_indeks_1 + 1;
        
      c2 = &font->data[(text->string[char_indeks_2]-32) * font->char_byte_length];
      
      // ako je to zadnji znak u stringu, crtamo "space" znak  
      if (*c2 == '\0') {
        c2 = font->data;
      }
    }
    else {
      c2 = font->data;
    }
    
    unsigned int block_data[8][8];
    
    if (font->alpha) {
      // pomicemo se na redak u znaku od kojeg pocinjemo crtati
      c1 = c1 + font->row_byte_length * char_start_pos_y_px;
      c2 = c2 + font->row_byte_length * char_start_pos_y_px;
                    
      unsigned char *bp[8];
      unsigned char bs[8];
      
      for (int i = 0; i < 8; i++ ) {
        unsigned char x_pos_piksela = char_start_pos_x_px + i; 
        if (x_pos_piksela < font->width) {
          bp[i] = c1 + x_pos_piksela/2;
          bs[i] = ((x_pos_piksela+1)%2) * 4;
        }
        else {
          x_pos_piksela -= font->width;
          bp[i] = c2 + x_pos_piksela/2;
          bs[i] = ((x_pos_piksela+1)%2) * 4;
        }
      }
      
      for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
          color = palette[((*bp[x])>>bs[x])&0xf];
          
          lcd_data_set(color);
          LCD_WRITE;
          
          bp[x] = bp[x] + font->row_byte_length;
        }
      }    
    }
    else {  
      for (y = 0; y < 8; y++) {
        row = c1[y] << char_start_pos_x_px;
        row += c2[y] >> (font->width-char_start_pos_x_px);
        
        for (x = 0; x < 8; x++) {
          if (row & 0x80)
            color = palette[15];
          else
            color = palette[0];
          row = row<<1;
          
          lcd_data_set(color);
          LCD_WRITE;
        }
      }
    }
  }
}

void gui_draw_object_image (unsigned char pos_x, unsigned char pos_y, s_gui_object_image *image) {
  unsigned int sy = (pos_y - image->pos_y) * 8;
  unsigned int sx = (pos_x - image->pos_x) * 8;
  unsigned int ey = sy + 8;
  unsigned int ex = sx + 8;  
  
  for (int y = sy; y < ey; y++) {
    for (int x = sx; x < ex; x++) {
      unsigned char *d = &image->image[(y*image->image_size_x+x)*2];
      lcd_data_set_2(*d, *(d+1));
      LCD_WRITE;
    }
  }
}

#include "images/button1.h"
#include "pagegen/page1.h"
#include "pagegen/page2.h"
#include "pagegen/page3.h"

static const s_gui_object_plain bg = { GUI_OBJECT_TYPE_PLAIN, COLOR_WHITE };
static const s_gui_object_bitmap b0 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*0 };
static const s_gui_object_bitmap b1 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*1 };
static const s_gui_object_bitmap b2 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*2 };
static const s_gui_object_bitmap b3 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*3 };
static const s_gui_object_bitmap b4 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*4 };
static const s_gui_object_bitmap b5 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*5 };
static const s_gui_object_bitmap b6 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*6 };
static const s_gui_object_bitmap b7 = { GUI_OBJECT_TYPE_BITMAP, button1_image+128*7 };

const unsigned char *const_object[9] = {&bg, &b0, &b1, &b2, &b3, &b4, &b5, &b6, &b7};

s_gui_page *current_page;
s_gui_page *page[3];

void change_page(int param1, int param2) {
  LOG("change_page: %d\n", param1);
    
  gui_draw(page[param1], 0, 0, 40, 30);
}

#define DRAW_SIZE 1

unsigned int draw_color = COLOR_BLACK; 

void page2_draw (int param1, int param2) {
  int i;
  
  if (param1 <= 1)
    param1 = 2;
  //if (y == 0)
  //  y = 1;
    
  lcd_set_address(param1-DRAW_SIZE,param2-DRAW_SIZE,param1+DRAW_SIZE,param2+DRAW_SIZE);
  delay_nop(10);
  LCD_SET_DATA_MODE;
  delay_nop(10);
  
  lcd_data_set(draw_color);
  for(i = 0; i < ((DRAW_SIZE*2)+1)*((DRAW_SIZE*2)+1); i++) {
    LCD_WRITE;
  }
}
void page2_color (unsigned int param1, unsigned int param2) {
  draw_color = param1;
}

void page2_clear (unsigned int param1, unsigned int param2) {
  LOG("color=%x", param1);
  gui_draw(page[1], 0, 0, 40, 30);
}

void page3_image (unsigned int param1, unsigned int param2) {
  s_gui_object_image *image = (s_gui_object_image *)page[2]->object[0];
  if (image->image == STATIC_PORSCHE) {
    image->image = STATIC_ROAD;
  }
  else if (image->image == STATIC_ROAD) {
    image->image = STATIC_CITY;
  }
  else if (image->image == STATIC_CITY) {
    image->image = STATIC_PORSCHE;
  }
  
  gui_draw(page[2], 0, 0, 40, 30);
}

void gui_init () {
  s_gui_object_text *text;
  
  // PAGE 0
  page[0] = malloc(sizeof(s_gui_page));
  page[0]->map = page1_map;
  
  page[0]->object = malloc(sizeof(unsigned char *) * 3);
  
  page[0]->object[0] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[0]->object[0];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 1;
  text->pos_y = 1;
  text->size_x = 38;
  text->size_y = 3;
  text->string = "http://www.mspanic.com";
  text->font_indeks = 1;
  text->palette_index = 0;
  text->align = 1;

  page[0]->object[1] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[0]->object[1];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 2;
  text->pos_y = 6;
  text->size_x = 36;
  text->size_y = 3;
  text->string = "Drawing";
  text->font_indeks = 1;
  text->palette_index = 1;
  text->align = 1;
  
  page[0]->object[2] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[0]->object[2];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 2;
  text->pos_y = 12;
  text->size_x = 36;
  text->size_y = 3;
  text->string = "Gallery";
  text->font_indeks = 1;
  text->palette_index = 1;
  text->align = 1;
  
  page[0]->touch_area_count = 2;
  page[0]->touch_area = malloc(sizeof(s_gui_touch_area) * page[0]->touch_area_count);
  
  s_gui_object_plain *plain;
  s_gui_touch_area *touch_area;
  
  page[0]->touch_area[0] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[0]->touch_area[0];
  touch_area->start_x = 1;
  touch_area->start_y = 5;
  touch_area->end_x = 39;
  touch_area->end_y = 10;
  touch_area->func_ptr = &change_page;
  touch_area->param1 = 1;
  
  page[0]->touch_area[1] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[0]->touch_area[1];
  touch_area->start_x = 1;
  touch_area->start_y = 11;
  touch_area->end_x = 39;
  touch_area->end_y = 16;
  touch_area->func_ptr = &change_page;
  touch_area->param1 = 2;
  
  // PAGE 1
  page[1] = malloc(sizeof(s_gui_page));
  page[1]->map = page2_map;
  
  page[1]->object = malloc(sizeof(unsigned char *) * 6);
  
  page[1]->object[0] = malloc(sizeof(s_gui_object_plain));
  plain = (s_gui_object_plain *)page[1]->object[0];
  plain->type = GUI_OBJECT_TYPE_PLAIN;
  plain->color = COLOR_BLACK;

  page[1]->object[1] = malloc(sizeof(s_gui_object_plain));
  plain = (s_gui_object_plain *)page[1]->object[1];
  plain->type = GUI_OBJECT_TYPE_PLAIN;
  plain->color = COLOR_RED;

  page[1]->object[2] = malloc(sizeof(s_gui_object_plain));
  plain = (s_gui_object_plain *)page[1]->object[2];
  plain->type = GUI_OBJECT_TYPE_PLAIN;
  plain->color = COLOR_GREEN;

  page[1]->object[3] = malloc(sizeof(s_gui_object_plain));
  plain = (s_gui_object_plain *)page[1]->object[3];
  plain->type = GUI_OBJECT_TYPE_PLAIN;
  plain->color = COLOR_BLUE;
  
  page[1]->object[4] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[1]->object[4];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 27;
  text->pos_y = 27;
  text->size_x = 3;
  text->size_y = 1;
  text->string = "Clear";
  text->font_indeks = 0;
  text->palette_index = 1;
  text->align = 0;
  
  page[1]->object[5] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[1]->object[5];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 34;
  text->pos_y = 27;
  text->size_x = 3;
  text->size_y = 1;
  text->string = "Back";
  text->font_indeks = 0;
  text->palette_index = 1;
  text->align = 1;
  
  page[1]->touch_area_count = 7;
  page[1]->touch_area = malloc(sizeof(s_gui_touch_area) * page[1]->touch_area_count);
  
  page[1]->touch_area[0] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[0];
  touch_area->start_x = 0;
  touch_area->start_y = 0;
  touch_area->end_x = 40;
  touch_area->end_y = 24;
  touch_area->func_ptr = &page2_draw;
  
  page[1]->touch_area[1] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[1];
  touch_area->start_x = 1;
  touch_area->start_y = 25;
  touch_area->end_x = 5;
  touch_area->end_y = 29;
  touch_area->func_ptr = &page2_color;
  touch_area->param1 = COLOR_BLACK;
  
  page[1]->touch_area[2] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[2];
  touch_area->start_x = 6;
  touch_area->start_y = 25;
  touch_area->end_x = 10;
  touch_area->end_y = 29;
  touch_area->func_ptr = &page2_color;
  touch_area->param1 = COLOR_RED;
  
  page[1]->touch_area[3] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[3];
  touch_area->start_x = 11;
  touch_area->start_y = 25;
  touch_area->end_x = 15;
  touch_area->end_y = 29;
  touch_area->func_ptr = &page2_color;
  touch_area->param1 = COLOR_GREEN;
  
  page[1]->touch_area[4] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[4];
  touch_area->start_x = 16;
  touch_area->start_y = 25;
  touch_area->end_x = 20;
  touch_area->end_y = 29;
  touch_area->func_ptr = &page2_color;
  touch_area->param1 = COLOR_BLUE;
  
  page[1]->touch_area[5] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[5];
  touch_area->start_x = 26;
  touch_area->start_y = 26;
  touch_area->end_x = 33;
  touch_area->end_y = 29;
  touch_area->func_ptr = &page2_clear;
  touch_area->param1 = 0;
  
  page[1]->touch_area[6] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[1]->touch_area[6];
  touch_area->start_x = 33;
  touch_area->start_y = 26;
  touch_area->end_x = 39;
  touch_area->end_y = 29;
  touch_area->func_ptr = &change_page;
  touch_area->param1 = 0;
   
  // PAGE 2
  page[2] = malloc(sizeof(s_gui_page));
  page[2]->map = page3_map;
  
  page[2]->object = malloc(sizeof(unsigned char *) * 4);
  
  page[2]->object[0] = malloc(sizeof(s_gui_object_image));
  s_gui_object_image *image = (s_gui_object_image *)page[2]->object[0];
  image->type = GUI_OBJECT_TYPE_IMAGE;
  image->pos_x = 0;
  image->pos_y = 0;
  image->size_x = 40;
  image->size_y = 30;
  image->image = STATIC_ROAD;
  image->image_size_x = 320;
  image->image_size_y = 240;
     
  page[2]->object[1] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[2]->object[1];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 20;
  text->pos_y = 27;
  text->size_x = 4;
  text->size_y = 1;
  text->string = "Prev.";
  text->font_indeks = 0;
  text->palette_index = 1;
  text->align = 1;
  
  page[2]->object[2] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[2]->object[2];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 27;
  text->pos_y = 27;
  text->size_x = 4;
  text->size_y = 1;
  text->string = "Next";
  text->font_indeks = 0;
  text->palette_index = 1;
  text->align = 1;   
   
  page[2]->object[3] = malloc(sizeof(s_gui_object_text));
  text = (s_gui_object_text *)page[2]->object[3];
  text->type = GUI_OBJECT_TYPE_TEXT;
  text->pos_x = 34;
  text->pos_y = 27;
  text->size_x = 4;
  text->size_y = 1;
  text->string = "Back";
  text->font_indeks = 0;
  text->palette_index = 1;
  text->align = 1;   
   
  page[2]->touch_area_count = 3;
  page[2]->touch_area = malloc(sizeof(s_gui_touch_area) * page[2]->touch_area_count);
  
  page[2]->touch_area[0] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[2]->touch_area[0];
  touch_area->start_x = 19;
  touch_area->start_y = 26;
  touch_area->end_x = 25;
  touch_area->end_y = 30;
  touch_area->func_ptr = &page3_image;
  touch_area->param1 = 0;
  
  page[2]->touch_area[1] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[2]->touch_area[1];
  touch_area->start_x = 26;
  touch_area->start_y = 26;
  touch_area->end_x = 30;
  touch_area->end_y = 30;
  touch_area->func_ptr = &page3_image;
  touch_area->param1 = 1;
  
  page[2]->touch_area[2] = malloc(sizeof(s_gui_touch_area));
  touch_area = (s_gui_object_text *)page[2]->touch_area[2];
  touch_area->start_x = 33;
  touch_area->start_y = 26;
  touch_area->end_x = 39;
  touch_area->end_y = 30;
  touch_area->func_ptr = &change_page;
  touch_area->param1 = 0;
   
   
  gui_draw(page[0], 0, 0, 40, 30);
}


//******************************************************************************
// PAGE
//******************************************************************************
void gui_draw(s_gui_page *draw_page, unsigned char start_x, unsigned char start_y, unsigned char size_x, unsigned char size_y) {
  int x, y;
  
  current_page = draw_page;
  
  LOG("draw: %x %d %d %d %d\n", current_page, start_x, start_y, size_x, size_y);
  
  for (y = start_y; y < start_y+size_y; y++) {
    for (x = start_x; x < start_x+size_x; x++) {
      //LOG("d %d %d\n", x, y);
      lcd_set_address(x*8,y*8,x*8+7,y*8+7);
      LCD_SET_DATA_MODE;
      
      //LOG("i=%d\n", y*40+x);
      unsigned char object_index = current_page->map[y*40+x];
      //LOG("ind=%d\n", object_index);
      
      unsigned char *object;
      if (object_index >= 128)
        object = current_page->object[object_index - 128];
      else
        object = const_object[object_index];
      
      unsigned char object_type = *object;
      //LOG("object_type=%d\n", x, y, object_type);
      
      if (object_type == GUI_OBJECT_TYPE_PLAIN) {
        s_gui_object_plain *plain = (s_gui_object_plain *)object;
        
        lcd_data_set(plain->color);
        //lcd_write(64);
        for (int i = 0; i < 64; i++) {
          LCD_WRITE;
        }
      }
      else if (object_type == GUI_OBJECT_TYPE_BITMAP) {
        s_gui_object_bitmap *bitmap = (s_gui_object_bitmap *)object;
        
        //lcd_write_block2(block_image, 128);
        for (int i = 0; i < 128; i=i+2) {
          //LOG("%x %x\n", bitmap->image[i], bitmap->image[i+1]);
          lcd_data_set_2(bitmap->image[i], bitmap->image[i+1]);
          LCD_WRITE;
        }
      }
      else if (object_type == GUI_OBJECT_TYPE_TEXT) {
        gui_draw_object_text(x, y, object);
      }
      else if (object_type == GUI_OBJECT_TYPE_IMAGE) {
        gui_draw_object_image(x, y, object);
      }
      
    }
  }
}

void gui_touch (unsigned int x, unsigned int y) {
  unsigned char cx = x/8;
  unsigned char cy = y/8;
     
  s_gui_touch_area *ta = 0;
  for(int i = 0; i < current_page->touch_area_count; i++) {
    if ( cx >= current_page->touch_area[i]->start_x
      && cy >= current_page->touch_area[i]->start_y 
      && cx <= current_page->touch_area[i]->end_x
      && cy <= current_page->touch_area[i]->end_y)
    {
      //LOG("touch=%d", i);
      ta = current_page->touch_area[i];
      break;
    }
  }
  
  if (ta > 0) {
    if (ta->func_ptr == &page2_draw)
      (*ta->func_ptr)(x, y);
    else
      (*ta->func_ptr)(ta->param1, ta->param2);
    
  }
}
