typedef struct {
  unsigned char width;
  unsigned char height;
  unsigned char alpha;
  unsigned char row_byte_length;
  unsigned char char_byte_length;
  unsigned char *data;
} s_font;

void init_fonts ();
s_font *get_font(int font_index);

void print_t8 (unsigned short x, unsigned short y, char *str, int fore_color, int back_color);
