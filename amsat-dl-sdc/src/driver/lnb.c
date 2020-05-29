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
#include <pinmap.h>
#include "lnb.h"
#include "gpio.h"

#define LNB_REG_DATA1 0x02
#define LNB_REG_DATA2 0x03
#define LNB_REG_DATA3 0x04
#define LNB_REG_DATA4 0x05

static int lnb_do_configure(i2c_dev i2c, u8 addr);

void lnb_configure(i2c_dev i2c, u8 addr)
{
    int res = i2c_probe_addr(i2c, addr);
    if(res == 0) 
	{
		printf("LNB:LNB Chip found\n");
		lnb_do_configure(i2c, addr);
		__delaywdt_ms(250);
		if(!check_LNB_status(i2c, addr))
		{
			// if FLT then configure again
			lnb_do_configure(i2c, addr);
		}
    } 
	else 
	{
        printf("LNB:LNB Chip not found!\n");
    }
    
    return;
}

static int lnb_do_configure(i2c_dev i2c, u8 addr)
{
    u8 value;
    int res;

    //14V 0x04
    //18V 0x08
    res = smbus8_write_byte(i2c, addr, LNB_REG_DATA1, 0x84);
    if(res == 0) 
	{
		res = smbus8_read_byte(i2c, addr, LNB_REG_DATA4, &value);
		if(res == 0) 
		{
			value |= (1<<6) | (1<<3);
			res = smbus8_write_byte(i2c, addr, LNB_REG_DATA4, value);
			if(res == 0) 
			{
				res = smbus8_read_byte(i2c, addr, LNB_REG_DATA3, &value);
				if(res == 0) 
				{
					value |= (1<<2) | (1<<6);
					res = smbus8_write_byte(i2c, addr, LNB_REG_DATA3, value);
				}
			}
		}
	}
	
	if(res == 0) 
		printf("LNB:LNB Chip configured\n");
	else
		printf("LNB:Failed to configure LNB Chip (%i)\n", res);

    return res;
}

u8 LNB_laststat = 0xff;
u8 LNB_error = 0;

u8 check_LNB_status(i2c_dev i2c, u8 addr)
{
	u8 stat;
	
	stat = gpio_get_input(PIN_LNB_FAULT);
	if(stat != LNB_laststat)
	{
		if(stat) 
		{
			printf("LNB:OK\n");
			gpio_set_lvl_low(PIN_LED_LNB_GOOD);
		} 
		else 
		{
			// read status register to clear error
			smbus8_read_byte(i2c, addr, 0, &LNB_error);
			printf("LNB: error: %02X\n",LNB_error);
			gpio_set_lvl_high(PIN_LED_LNB_GOOD);
		}
		LNB_laststat = stat;
	}
	
	return stat;
}
