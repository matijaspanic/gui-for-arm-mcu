// STATIC IMAGES

#define STATIC_BUTTON1          ((unsigned char *)0x0004485C) 

#define STATIC_PORSCHE          ((unsigned char *)0xc200)
#define STATIC_ROAD             ((unsigned char *)0x31a00)
#define STATIC_CITY             ((unsigned char *)0x57200)

#define GUI_OBJECT_TYPE_PLAIN   0
#define GUI_OBJECT_TYPE_BITMAP  1
#define GUI_OBJECT_TYPE_TEXT    2
#define GUI_OBJECT_TYPE_IMAGE   3

typedef struct {
  unsigned char type;
  
  unsigned int color;
} s_gui_object_plain;

typedef struct {
  unsigned char type;
  
  unsigned char *image;
} s_gui_object_bitmap;

typedef struct {
  unsigned char type;

  unsigned char pos_x;
  unsigned char pos_y;
  unsigned char size_x;
  unsigned char size_y;

  char *string;
  unsigned char font_indeks;
  unsigned char palette_index;
  unsigned char align; //0-left, 1-center, 2-right

} s_gui_object_text;

typedef struct {
  unsigned char type;

  unsigned char pos_x;
  unsigned char pos_y;
  unsigned char size_x;
  unsigned char size_y;

  unsigned char *image;
  unsigned int image_size_x;
  unsigned int image_size_y;
} s_gui_object_image;

typedef struct {
  unsigned char start_x, start_y, end_x, end_y;
  void (*func_ptr)(int *param1, int *param2);
  int param1;
  int param2;
} s_gui_touch_area;

typedef struct {
  unsigned char *map;
    
  unsigned char **object;
  unsigned char touch_area_count;
  s_gui_touch_area **touch_area;
} s_gui_page;

void gui_touch (unsigned int x, unsigned int y);
void gui_draw(s_gui_page *draw_page, unsigned char start_x, unsigned char start_y, unsigned char size_x, unsigned char size_y);
void gui_init ();
