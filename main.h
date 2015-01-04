// frequency of crystal oscillator
#define FOSC 12000000

// Max CC Osc Freq.
#define FCCO_MAX 320000000

// PLL Multiplier and divider
#define PLL_MUL 5
#define PLL_DIV 2 //(FCCO_MAX / (2 * CCLK))
// 1 8
// 2 4
// 3 4
// 4 2
// 5 2

// CCLK = Fosc * PLL multiplier               
#define CCLK (PLL_MUL*FOSC)

#define VPBDIV_VALUE 1
#define VBP_DIVIDER 1 // postaviti na 1, 2 ili 4 ovisno o VPBDIV_VALUE vrijednosti
// 0 - APB bus clock is one fourth of the processor clock.
// 1 - APB bus clock is the same as the processor clock.
// 2 - APB bus clock is one half of the processor clock.


#define PCLK (CCLK / VBP_DIVIDER)

void lprintf(char *fmt, ...);
#define LOG lprintf
