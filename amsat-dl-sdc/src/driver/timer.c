/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * timer interrupt
 * 
 * The timer clock is CPUclk/4 = 2 MHz
 * no prescaler is used
 * counting 0x4e20  times ( = b1e0 to ffff)
 * results in one IRQ every 10ms
 * 
 */

#include <config.h>
#include <pinmap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "timer.h"
#include "irq.h"

u8 gps_init_timer = 0;
u8 cpu_blink_timer = 0;
u8 adf4351_set_qrg = 0;

// interrupt service routine
void t0_irq() 
{
	static u8 second2 = 0;
	static u8 ms500 = 0;
	
	if(++ms500 > 50)
	{
		ms500 = 0;
		// here ever 500ms
		if(cpu_blink_timer) cpu_blink_timer = 0;
		else cpu_blink_timer = 1;
		
		if(adf4351_set_qrg != 0 && adf4351_set_qrg < 3)
			adf4351_set_qrg++;
	}
	
	if(++second2 >= 200)
	{
		second2 = 0;
		// here every two seconds
		// trigger GPS init
		gps_init_timer = 1;
	}
	
	TMR0L = 0xe0;
	TMR0H = 0xb1;
}

void init_timer(void)
{
    T0CON = 0x88;	// no prescaler
	INTCONbits.TMR0IE = 1; // enable TMR0 int
	// must be cleared after every interrupt
	INTCONbits.TMR0IF = 0;  // Clear TMR0 IRQ flag
	TMR0L = 0xe0;
	TMR0H = 0xb1;
	
	irq_set_handler(IRQ_HANDLER_ID_TIMER0, t0_irq, NULL);
}

