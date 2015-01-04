#include <arch/philips/LPC214X.h>

#include "main.h"

#define PINSEL0_TXD (1<<0)
#define PINSEL0_RXD (1<<2)

#define U0_PINSEL       (0x00000005)    /* PINSEL0 Value for UART0 */
#define U0_PINMASK      (0x0000000F)    /* PINSEL0 Mask for UART0 */
#define ULCR_DLAB_ENABLE    (1 << 7)    // Enable Divisor Latch Access

#define UART_BAUD(baud) (unsigned short)((PCLK / ((baud) * 16.0)) + 0.5)

#define ULCR_CHAR_8         (3 << 0)    // 8-bit character length
#define ULCR_PAR_NO         (0 << 3)    // No Parity
#define ULCR_STOP_1         (0 << 2)    // 1 stop bit
#define UART_8N1      (unsigned char)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_1)

#define UFCR_FIFO_ENABLE    (1 << 0)    // FIFO Enable
#define UFCR_FIFO_TRIG8     (2 << 6)    // Trigger @ 8 characters in FIFO
#define UART_FIFO_8   (unsigned char)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG8)

#define ULSR_THRE           (1 << 5)    // Transmit Holding Register Empty

#define ULSR_RDR            (1 << 0)    // Receive Data Ready

void uart_init () {
	// set port pins for UART0
	PINSEL0 = (PINSEL0 & ~U0_PINMASK) | U0_PINSEL;
	//PINSEL0 = PINSEL0_RXD;

	U0IER = 0x00;                         // disable all interrupts
	U0IIR;                                // clear interrupt ID
	U0RBR;                                // clear receive register
	U0LSR;                                // clear line status register

	// set the baudrate
	U0LCR = ULCR_DLAB_ENABLE;             // select divisor latches 
	U0DLL = (unsigned char)UART_BAUD(57600);                // set for baud low byte
	U0DLM = (unsigned char)(UART_BAUD(57600) >> 8);         // set for baud high byte
  
    // set the number of characters and other
	// user specified operating parameters
	U0LCR = (UART_8N1 & ~ULCR_DLAB_ENABLE);
	U0FCR = UART_FIFO_8;
  
	
	
	// endless loop to toggle the two leds
	/*while (1) {
		while (1) {
			if (U0LSR & ULSR_RDR) {
				dummy = U0RBR;
				break;
			}
		}
	}*/
}

void uart_send_char(char c) {
  while (!(U0LSR & ULSR_THRE));
  U0THR = c;
}

void uart_send_string (char *string) {
  while (*string != '\0') {
    uart_send_char(*string);
    string++;
  }
}

void uart_send_string_nl (char *string) {
  uart_send_string(string);
  uart_send_char('\n');
}
