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
#include <pinmap.h>
#include <stdio.h>
#include <string.h>
#include "driver/si5328.h"

void check_RXTX(void);

void init_gpios(void)
{
	
    ADCON1bits.PCFG = 0xF;	// all analog pins work as digital I/O pins
    INTCON2bits.nRBPU = 0;	//enable internal pull-up on Port B

	// set ADF4351 pins
    gpio_set_dir_in(PIN_ADF_LOCK);	//ADF4351 Lock Detect
    
    gpio_set_lvl_low(PIN_ADF_CE);	//ADF4351 Chip Enable
    gpio_set_dir_out(PIN_ADF_CE);
    
    gpio_set_lvl_low(PIN_ADF_LE);	//ADF4351 Latch Enable
    gpio_set_dir_out(PIN_ADF_LE);
    
    gpio_set_lvl_low(PIN_SIO_DATA);	//Serial Data
    gpio_set_dir_out(PIN_SIO_DATA);
    
    gpio_set_lvl_low(PIN_SIO_CLK);	//Serial Clock
    gpio_set_dir_out(PIN_SIO_CLK);
	
	// set si5328 pins
	// LOL: Loss Of Lock (PLL status))
	gpio_set_dir_in(PIN_SI5328_LOL);

    gpio_set_lvl_high(PIN_SI5328_CS_CA);	//si5328 Chip Select
    gpio_set_dir_in(PIN_SI5328_CS_CA);		// pullup
	
	// set LED ports
    gpio_set_lvl_high(PIN_LED_SI_LOCK);
    gpio_set_dir_out(PIN_LED_SI_LOCK);

    gpio_set_lvl_high(PIN_LED_CPU_OK);
    gpio_set_dir_out(PIN_LED_CPU_OK);

    gpio_set_lvl_high(PIN_LED_ADF_LOCK);
    gpio_set_dir_out(PIN_LED_ADF_LOCK);

    gpio_set_lvl_high(PIN_LED_LNB_GOOD);
    gpio_set_dir_out(PIN_LED_LNB_GOOD);
    
	// set I2C ports
    gpio_set_dir_in(PIN_I2C_SCL);
    gpio_set_dir_in(PIN_I2C_SDA);

	// UART for GPS Receiver
	check_RXTX();
 
	// channel switch
    gpio_set_dir_in(PIN_SJ3);
    gpio_set_lvl_high(PIN_SJ3); //pull-up
    gpio_set_dir_in(PIN_SJ4);
    gpio_set_lvl_high(PIN_SJ4); //pull-up
    gpio_set_dir_in(PIN_SJ5);
    gpio_set_lvl_high(PIN_SJ5); //pull-up
    gpio_set_dir_in(PIN_SJ6);
    gpio_set_lvl_high(PIN_SJ6); //pull-up

	// LNB selection jumpers
    gpio_set_dir_in(PIN_LNB_QRG2);
    gpio_set_lvl_high(PIN_LNB_QRG2); //pull-up
    gpio_set_dir_in(PIN_LNB_QRG1);
    gpio_set_lvl_high(PIN_LNB_QRG1); //pull-up

	// LNBH26PQR LNB-Voltage Controller
    gpio_set_dir_in(PIN_LNB_FAULT);
}

// if RX is bridged to GND by a jumper, then configure the TIMEPULSE pin as external 10 MHz input
void check_RXTX()
{
	gpio_set_dir_out(PIN_USART_TX);
    gpio_set_dir_in(PIN_USART_RX);
	
	// measure if RX is connected to TX
	for(int i=0; i<5; i++)
	{
		gpio_set_lvl_low(PIN_USART_TX);
		__delaywdt_ms(10);
		if(gpio_get_input(PIN_USART_RX))
		{
			ext10MHzReference = 0;
			return;
		}

		gpio_set_lvl_high(PIN_USART_TX);
		__delaywdt_ms(10);
		if(gpio_get_input(PIN_USART_RX) == 0)
		{
			ext10MHzReference = 0;
			return;
		}
	}
	
	ext10MHzReference = 1;
}
