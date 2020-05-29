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
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"
#include "gps.h"
#include <driver/irq.h>

//USART RX-fifo
static volatile char g_usart_buf[128];
static volatile int g_usart_rpos = 0;
static volatile int g_usart_wpos = 0;

static void usart_rx_irq_handler(void *udata);

void usart_init(u32 baud)
{
    irq_set_handler(IRQ_HANDLER_ID_USART, usart_rx_irq_handler, NULL);
    TXSTAbits.TX9 = 0;   //8-Bit transmit
    TXSTAbits.SENDB = 0; //don't send sync break
    TXSTAbits.BRGH = 1;  //high speed
    TXSTAbits.TXEN = 1;  //enable transmiter
    
    
    RCSTAbits.RX9 = 0;   //8-Bit receive
    RCSTAbits.CREN = 1;  //enable receiver
    RCSTAbits.ADDEN = 0; //disable address detection
    
    BAUDCONbits.BRG16 = 1; //16-bit baud rate generator
    BAUDCONbits.ABDEN = 0; //auto baudrate disable
    
    u16 spbrg_value = ((_XTAL_FREQ/4) / baud) - 1;
    
    SPBRGH = (spbrg_value>>8)&0xFF; //set baudrate
    SPBRG  = (spbrg_value>>0)&0xFF; //set baudrate
    
    TXSTAbits.SYNC = 0; //async
    RCSTAbits.SPEN = 1; //enable usart port

    PIE1bits.RCIE = 1;  //enable rx interrupt
}

char usart_putc(char c)
{
    //wait for tx register to be empty
	CLRWDT();
    while(TXSTAbits.TRMT == 0) CLRWDT();
    //send data
    TXREG = c;
    
    return c;
}

//read char from usart-port
int usart_getc(void)
{
	int res;
	
	CLRWDT();

	PIE1bits.RCIE = 0;
	
	if(g_usart_wpos == g_usart_rpos) 
	{
		PIE1bits.RCIE = 1;
		return -1;	// no char available
	}

	//get char
	res = g_usart_buf[g_usart_rpos];
	g_usart_rpos = (g_usart_rpos+1) % sizeof(g_usart_buf);

	PIE1bits.RCIE = 1;
	return res;
}

static void usart_rx_irq_handler(void *udata)
{
    volatile char c;
    volatile int wposn;

    //check if operation is active
    if(BAUDCONbits.RCIDL == 0) return;
    
	//read char (clears irq flag)
	c = RCREG;

    //check for errors
    if(RCSTAbits.FERR) {
        return;
    }
    if(RCSTAbits.OERR) {
        //reset bus
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        return;
    }

	//write char into fifo
	wposn = (g_usart_wpos+1) % sizeof(g_usart_buf);
	if(wposn != g_usart_rpos) {
		g_usart_buf[g_usart_wpos] = c;
		g_usart_wpos = wposn;
	}
}

// called by user program to get data from the rx buffer and handle it
void rx_usart(void)
{
	int c;
	
	while(1)
	{
		c = usart_getc();
		if(c == -1) return;
		rx_gps(c);
	}
}

//retarget for printf
int fputc(int c, FILE *fp)
{
#ifdef GPS
	return 0;	// do not send debug messages if GPS is active
#endif
	
    if(c == '\n') {
        usart_putc('\r');
    }
    usart_putc((char)c);

    return (unsigned char)c;
}

void putgps(u8 c)
{
	usart_putc(c);
}

