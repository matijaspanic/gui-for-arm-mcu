#include <arch/philips/LPC214X.h>

#include "main.h"

#define PLLCON_PLLE   (1 << 0)          // PLL Enable
#define PLLCON_PLLC   (1 << 1)          // PLL Connect
#define PLLSTAT_PLOCK  (1 << 10)         // PLL Lock Status Bit

void set_pll () {
	PLLCFG = ((PLL_DIV - 1) << 5) | (PLL_MUL - 1); // higher 2 bits - divider, lower 5 bits - multiplier
	
	PLLCON = PLLCON_PLLE;
	PLLFEED = 0xAA;                       // Make it happen.  These two updates
	PLLFEED = 0x55;                       // MUST occur in sequence.

	//while (!(PLLSTAT & PLLSTAT_PLOCK));  // wait until PLL is locked onto the requested frequency
	 int loop_ctr = 10000;
    while
    ( ((PLLSTAT&(1<<10))==0) && (loop_ctr>0) ) loop_ctr--;
	
	// enable & connect PLL
	PLLCON = PLLCON_PLLE | PLLCON_PLLC;
	PLLFEED = 0xAA;                       // Make it happen.  These two updates
	PLLFEED = 0x55;                       // MUST occur in sequence.
}
