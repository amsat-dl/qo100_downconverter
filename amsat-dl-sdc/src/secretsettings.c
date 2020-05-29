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
 * File:   secretsettings.h
 * Author: kurt
 *
 * Created on 3. November 2019, 16:06
 * 
 * Procedure to set an operating mode:
 * 
 * * switch OFF the board
 * * bridge pin 3+4 of the GPS connector (like in ext10MHz mode)
 * * bridge both LNB freq. selectors (both jumpers set)
 * * select an OP mode with the switch 0..9
 * * power up the board
 * * wait until the 4 LEDs are flashing
 * * power down, remove the jumpers
 * 
 * now the OP mode is set and the board can be used normally
 * 
 * the switch selects the config.
 * currently defined:
 * 0 ... restore factory defaults
 * 1 ... switch OFF GPS frequency shift
 * 2 ... set OCXO mode even if a GPS module is detected, don't use GPS-Sync but display GPS data
 * 3 ... TCXO version without GPS module
 * 
 */

#include <config.h>
#include <pinmap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <eeprom.h>
#include <driver/si5328.h>
#include <gps.h>

u8 read_switch(void);
void lcd_settingsOK(u8 mode);
void secret_settings_write(void);

u8 special_cfg = 0;
u8 tcxo_no_GPS = 0;

void secret_settings()
{
	secret_settings_write();	// returns if nothing to write
	
	// read the config
	special_cfg = eeprom_getmode();
	switch(special_cfg)
	{
	case 1:
		// switch OFF the GPS frequency shift
		clear_GPS_shift();
		printf("GPS: shift OFF\n");
		break;
		
	case 2:
		// do not use GPS timepulse (stay in OCXO 40 MHz mode), but read GPS data into the display
		use_GPS = 0;
		break;
		
	case 3:
		// use the TCXO version without GPS module
		tcxo_no_GPS = 1;
		break;
	}
}

void secret_settings_write()
{
	// TX/RX are bridged, this has two functions:
	// if both LNB selectors are also bridged, then go to OP mode selection
	// otherwise continue in ext10MHz mode
	
	if(ext10MHzReference == 0)
		return;

	if(gpio_get_input(PIN_LNB_QRG2) != 0 || gpio_get_input(PIN_LNB_QRG1) != 0)
		return;
	
	// read the switch 0..9 to get the configuration
	u8 sw = read_switch();
	
	if(sw <= 3)
	{
		eeprom_setmode(sw);
	
		lcd_settingsOK(sw);
		// blinking shows that we are ready and the board must be powered off/on
		while(1)
		{
			gpio_set_lvl_low(PIN_LED_SI_LOCK);
			gpio_set_lvl_low(PIN_LED_CPU_OK);
			gpio_set_lvl_low(PIN_LED_ADF_LOCK);
			gpio_set_lvl_low(PIN_LED_LNB_GOOD);	

			__delaywdt_ms(100);

			gpio_set_lvl_high(PIN_LED_SI_LOCK);
			gpio_set_lvl_high(PIN_LED_CPU_OK);
			gpio_set_lvl_high(PIN_LED_ADF_LOCK);
			gpio_set_lvl_high(PIN_LED_LNB_GOOD);	

			__delaywdt_ms(100);
		}
	}
}
