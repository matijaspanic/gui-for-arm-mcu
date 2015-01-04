#include <arch/philips/LPC214X.h>
#include "lcd.h"
#include "ssd1289.h"
#include "delay.h"

void lcd_data_set (unsigned int data) {
  FIO0PIN = (FIO0PIN & ~0x40ffff00) | (data << 8)/* & 0xffff00)*/;
}

void lcd_data_write (unsigned int data) {
  lcd_data_set(data);
  
 	LCD_WRITE;
}

#define FIO0PIN1        (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x15))
#define FIO0PIN2        (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x16))

void lcd_data_set_2 (unsigned int data_high, unsigned int data_low) {
  FIO0PIN1 = data_low;
  FIO0PIN2 = data_high;
}

void lcd_data_write_2 (unsigned int data_high, unsigned int data_low) {
  FIO0PIN1 = data_low;
  FIO0PIN2 = data_high;
  
  LCD_WRITE;
}

void lcd_command_mode_write(unsigned int data)
{
  LCD_SET_COMMAND_MODE;
  lcd_data_write(data);
}

void lcd_data_mode_write(unsigned int data)
{
  LCD_SET_DATA_MODE;
  lcd_data_write(data);
}

void lcd_command_data_write(unsigned int command, unsigned int data)
{
  lcd_command_mode_write(command);
  //delay_nop(10000);
  lcd_data_mode_write(data);
  //delay_nop(10000);
}

void lcd_set_address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	lcd_command_data_write(0x0044,(y2<<8)+y1);
	lcd_command_data_write(0x0045,x1);
	lcd_command_data_write(0x0046,x2);
	lcd_command_data_write(0x004e,y1);
	lcd_command_data_write(0x004f,x1);
  lcd_command_mode_write(0x0022);
}

void lcd_init()
{
//  LCD_RD_SET;
  
//  LCD_RST_SET;
//  LCD_RST_CLR;
  
  delay_ms(10);
 
  // display control
  lcd_command_data_write(DISPLAY_CONTROL,
    DC_D0
    | DC_GON
  );
  
  // turn on oscillator
  lcd_command_data_write(OSCILATOR, OSC_OSCEN);
  
  // set R07h at 0023h
  lcd_command_data_write(DISPLAY_CONTROL,
    DC_D0
    | DC_D1
    | DC_GON
  );
  
  // set R10h at 0000h = exit sleep mode
  lcd_command_data_write(SLEEP_MODE, 0x0000);
  
  // wait 30ms
  delay_ms(30);
  
  /*lcd_write_command(0x0022);
  lcd_set_data(get_RGB_color(0x00,0x00,0x00));
  LCD_RS_SET; // RS=1 - data
  for(int i = 0; i < 76800; i++)
	{
    LCD_WR_CLR;
	  LCD_WR_SET;
		//lcd_write_data(color);
	}*/
  
  
  lcd_command_data_write(DISPLAY_CONTROL,
    DC_D0
    | DC_D1
    | DC_DTE
    | DC_GON
  );
  
  // entry mode setting
  // set R11h at
  lcd_command_data_write(ENTRY_MODE,
    EM_AM 
    | EM_ID0
    | EM_ID1
    | EM_OED
    //| EM_TRANS
    | EM_DFM0
    | EM_DFM1
  );
  
  // LCD drive AC setting
  lcd_command_data_write(DRIVING_WAVEFORM_CONTROL, DWC_FLD);
  
  //Driver Output Control (R01h) (POR = [0XXXX0X1]3Fh)
  lcd_command_data_write(DRIVER_OUTPUT_CONTROL, 
    DOC_MUX0
    | DOC_MUX1
    | DOC_MUX2
    | DOC_MUX3
    | DOC_MUX4
    | DOC_MUX5
    | DOC_MUX8
    //| DOC_TB
    //| DOC_SM
    | DOC_BGR
    | DOC_REV 
    //| DOC_RL
  );
  
  // Frame Cycle Control (R0Bh) (POR = 5308h)
  lcd_command_data_write(FRAME_CYCLE_CONTROL, 
    FCC_RTN3
    | FCC_EQ0
    | FCC_EQ1
    | FCC_SDT0
    | FCC_NO0
  );
    
  //Setting R28h as 0x0006 is required before setting R25h and R29h registers.
  lcd_command_data_write(0x0028, 0x0006);
  
  //Frame Frequency Control (R25h) (POR = 8000h)
  lcd_command_data_write(FRAME_FREQUENCY_CONTROL, 0x8000); // 80Hz
  
  //set the RAM register for writing
  lcd_command_mode_write(0x0022);
}

void paint(unsigned int color)
{
	int i;
	lcd_set_address(0,0,319,239);
  
  LCD_SET_DATA_MODE;
  lcd_data_set(color);
  for(i = 0; i < 76800; i++)
	{
	  LCD_WRITE;
	}
}

