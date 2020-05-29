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
#include <config.h>
#include "irq.h"

static struct {
    irq_handler_func handler;
    void            *udata;
} g_irq_handler_info[IRQ_HANDLER_ID_CNT] = {};

void irq_set_handler(IRQ_HANDLER_ID id, irq_handler_func handler, void *udata)
{
    if(id < IRQ_HANDLER_ID_CNT) {
        g_irq_handler_info[id].udata = udata;
        g_irq_handler_info[id].handler = handler;
    }
}

static void __interrupt() global_irq_handler(void)
{
    if(PIR1bits.RCIF) 
	{
        if(g_irq_handler_info[IRQ_HANDLER_ID_USART].handler) 
            g_irq_handler_info[IRQ_HANDLER_ID_USART].handler(g_irq_handler_info[IRQ_HANDLER_ID_USART].udata);
		
		if(INTCONbits.RBIF) {
			volatile u8 dummy = PORTB;
			INTCONbits.RBIF = 0; 
		}
    }

    if(INTCONbits.TMR0IF) 
	{
        if(g_irq_handler_info[IRQ_HANDLER_ID_TIMER0].handler) 
            g_irq_handler_info[IRQ_HANDLER_ID_TIMER0].handler(g_irq_handler_info[IRQ_HANDLER_ID_TIMER0].udata);
		
		if(INTCONbits.TMR0IF) {
			volatile u8 dummy = PORTB;
			INTCONbits.TMR0IF = 0; 
		}
    }
}
