#include <arch/philips/LPC214X.h>

// fast port
//#define LCD_RST_CLR FIO0CLR |= (1<<25);
//#define LCD_RST_SET FIO0SET |= (1<<25);

//#define LCD_RD_CLR FIO0CLR |= (1<<25);
//#define LCD_RD_SET FIO0SET |= (1<<25);

#define LCD_RS_CLR FIO0CLR = (1<<7); // DC
#define LCD_RS_SET FIO0SET = (1<<7);

#define LCD_WR_CLR FIO0CLR = (1<<2);
#define LCD_WR_SET FIO0SET = (1<<2);

//#define LCD_CS_CLR FIO0CLR |= (1<<31);
//#define LCD_CS_SET FIO0SET |= (1<<31);


#define RGB3(R,G,B) (((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3))
#define RGB(RGB) (((RGB & 0xF80000) >> 8) | ((RGB & 0xFC00) >> 5) | ((RGB & 0xF8) >> 3))

#define COLOR_BLACK           0x0000      /*   0,   0,   0 */
#define COLOR_BLUE            0x001F      /*   0,   0, 255 */
#define COLOR_GREEN           0x07E0      /*   0, 255,   0 */
#define COLOR_CYAN            0x07FF      /*   0, 255, 255 */
#define COLOR_RED             0xF800      /* 255,   0,   0 */
#define COLOR_MAGENTA         0xF81F      /* 255,   0, 255 */
#define COLOR_YELLOW          0xFFE0      /* 255, 255,   0 */
#define COLOR_WHITE           0xFFFF      /* 255, 255, 255 */

#define COLOR_GRAY25 0x4208
#define COLOR_GRAY50 0x8410
#define COLOR_GRAY75 0xc618

#define COLOR_NAVY            0x000F      /*   0,   0, 128 */
#define COLOR_DARKGREEN       0x03E0      /*   0, 128,   0 */
#define COLOR_DARKCYAN        0x03EF      /*   0, 128, 128 */
#define COLOR_Maroon          0x7800      /* 128,   0,   0 */
#define COLOR_Purple          0x780F      /* 128,   0, 128 */
#define COLOR_Olive           0x7BE0      /* 128, 128,   0 */
#define COLOR_LightGrey       0xC618      /* 192, 192, 192 */
#define COLOR_DarkGrey        0x7BEF      /* 128, 128, 128 */

#define LCD_SET_DATA_MODE LCD_RS_SET; delay_nop(6)
#define LCD_SET_COMMAND_MODE LCD_RS_CLR; delay_nop(6)

#define LCD_WRITE LCD_WR_CLR; delay_nop(6); LCD_WR_SET; delay_nop(6)

// lcd.c
void lcd_data_set (unsigned int data);
void lcd_data_write (unsigned int data);

void lcd_data_set_2 (unsigned int data_high, unsigned int data_low);
void lcd_data_write_2 (unsigned int data_high, unsigned int data_low);

void lcd_set_address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

void lcd_init();

// lcda.S
extern void lcd_clear(unsigned int color);
extern void lcd_write(unsigned int pixels);
extern void lcd_write_block(unsigned char *block, unsigned char size);
extern void lcd_write_block2(unsigned char *block, unsigned char size);
