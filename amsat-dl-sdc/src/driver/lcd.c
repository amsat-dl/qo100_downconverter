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
#include <string.h>
#include <stdio.h>

#include "lcd.h"
#include "gps.h"
#include "si5328.h"
#include "adf4351.h"
#include "lnb.h"
#include "secretsettings.h"

#include <pinmap.h>

void ssd1306_command(u8 *cmd, int len);
void ssd1306_commandbyte(u8 cmd);
void ssd1306_data(u8 *data, int len);
void ssd1306_databyte(u8 data);
void lcd_draw8x8(u8 *buffer, u8 x, u8 y);
void lcd_clear_screen(u8 ystart);
void print_char(char c, u8 xpos, u8 ypos, u8 underline);
void print_string(char *s, u8 x, u8 y, u8 underline);
char *itoa(int value);
void ssd1306_setpos(u8 x, u8 y);

extern const u8 font8x8_basic[97][8];

#define SSD1306_LCDHEIGHT  64
#define SSD1306_LCDWIDTH 128

u8 ssd1306_init_commands[27] = {
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV,
    0xa0,//0x80,
    SSD1306_SETMULTIPLEX,
    0x3F,
    SSD1306_SETDISPLAYOFFSET,
    0x0,
    SSD1306_SETSTARTLINE,
	
    SSD1306_CHARGEPUMP,
    0x14,
    SSD1306_MEMORYMODE,
    0x00,                   // 0x00 horizontal addressing
	
    SSD1306_SEGREMAP | 0x1, // rotate screen 180
    SSD1306_COMSCANDEC,     // rotate screen 180
    SSD1306_SETCOMPINS,
    0x12,
    SSD1306_SETCONTRAST,
    0xdf,//0xCF,
    SSD1306_SETPRECHARGE,
    0x82,//0xF1,
    SSD1306_SETVCOMDETECT,
    0x34,//0x40,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
	
	SSD1306_COMSCANINC,		// flip display upside down and left right
	SSD1306_SEGREMAP,		// flip display upside down and left right

    SSD1306_DISPLAYON
};

i2c_dev lcd_i2c;
u8 lcd_addr;
u8 lcd_available = 0;

void lcd_init(i2c_dev i2c, u8 addr)
{
	int res;
	
	/*while(1)
	{
		res = i2c_probe_addr(i2c, addr);
		if(res == 0) break;
		__delaywdt_ms(2);
	}*/
	
	res = i2c_probe_addr(i2c, addr);
    if(res != 0) 
	{
        printf("LCD:OLED not found.\n");
		lcd_available = 0;
		return;
    } 
	
	lcd_i2c = i2c;
	lcd_addr = addr;

	printf("LCD:OLED found!\n");  
	lcd_available = 1;
	
    // Init sequence for 128x64 OLED module
	ssd1306_command(ssd1306_init_commands,sizeof(ssd1306_init_commands));
	
	ssd1306_commandbyte(SSD1306_SETSTARTLINE | 0x0); // line #0
	
	lcd_clear_screen(0);
	if(ext10MHzReference == 0)
		print_string("AMSAT-DL  intClk",0,0,0);
	else
		print_string("AMSAT-DL  ext10M",0,0,0);

}

void ssd1306_command(u8 *cmd, int len)
{
	if(!lcd_available) return;
	CLRWDT();
	smbus8_write_data(lcd_i2c, lcd_addr, 0x00, cmd, len);
}

void ssd1306_commandbyte(u8 cmd)
{
	if(!lcd_available) return;
	smbus8_write_byte(lcd_i2c, lcd_addr, 0x00, cmd);
}

void ssd1306_data(u8 *data, int len)
{
	if(!lcd_available) return;
	CLRWDT();
	smbus8_write_data(lcd_i2c, lcd_addr, 0x40, data, len);
}

/*void ssd1306_databyte(u8 data)
{
	if(!lcd_available) return;
	smbus8_write_data(lcd_i2c, lcd_addr, 0x40, &data, 1);
}*/

void ssd1306_setpos(u8 x, u8 y)
{
	u8 data[3] = {0xb0, 2, 0x10};
	
	data[0] += y;
	x += 2;	// to avoid unused pixels at the margin
	data[1] = x & 0x0f;
	data[2] = 0x10 | (x>>4);
	
	ssd1306_command(data,3);
}

void lcd_clear_screen(u8 ystart)
{
	u8 buf[SSD1306_LCDWIDTH];
	memset(buf,0,SSD1306_LCDWIDTH);

    for (u8 row=ystart; row<(SSD1306_LCDHEIGHT / 8); row++) 
	{
		ssd1306_setpos(0,row);

		ssd1306_data(buf,SSD1306_LCDWIDTH);
    }
}

// x: 0...127
// y: 0..8 (8 pixel row))
void lcd_draw8x8(u8 *buffer, u8 x, u8 y)
{
	// print an 8x8 pixel block
	ssd1306_setpos(x,y);
	
	ssd1306_data(buffer, 8);
}

// the font map goes top down
void print_char(char c, u8 xpos, u8 ypos, u8 underline) 
{
	u8 idx = c - ' ';
	if(idx >= 97) idx = 0;
	
	// turn font by 90 deg
	u8 map[8];
	memset(map,0,8);
	
	for(u8 y=0; y<8; y++)
	{
		for(u8 x=0; x<8; x++)
		{
			u8 b = (font8x8_basic[idx][y] & (1<<x))?1:0;
			if(y==7 && underline) b=1;
			map[x] |= b<<y;
		}
	}
	
	lcd_draw8x8(map,xpos,ypos);
}

void print_string(char *s, u8 x, u8 y, u8 underline)
{
	printf("OLD %02d %02d %s\n",x,y,s);
	for(u8 i=0; i<strlen(s); i++)
	{
		print_char(s[i],x+i*8,y,underline);
	}
}

static u8 LO[4] =
{
	24,
	25,
	26,
	27
};

static const u16 out_freq_table[10] = {
    3,
	21,
	28,
	29,
	50,
	70,
	144,
	145,
	435,
	439
};


// called by main loop abt.3 times per second
u8 booting = 0;
void show_display()
{
	//if(!lcd_available) return;
	
	static u8 old_GPS_lock = 0xff;
	static u8 old_GPS_fix = 0xff;
	static u8 old_ubx_ack = 0xff;
	static u16 old_outqrg = 0;
	static u8 old_lnbqrg = 0xff;
	static u8 old_GPS_sat = 0xff;
	static char old_GPS_lat[GPS_STRLENGTH] = {"             "};
	static char old_GPS_lon[GPS_STRLENGTH] = {"             "};
	static char old_GPS_time[9] = {"x"};
	static u8 old_LNB_laststat = 255;
	static u8 old_si5328locked = 255;
	static u8 old_adf4351_lockstat = 255;

	// show which components are up and running and which are locking
	if(booting == 2 && (!adf4351_lockstat || !si5328locked || !LNB_laststat))
		booting = 0;
	
	if(booting == 0)
	{
		// prepare screen
		lcd_clear_screen(1);
		print_string("Firmware:       ",0,1,0);
		print_string(VERSION_NUMBER,10*8,1,0);
		print_string("System SETUP ...",0,2,0);
		if(special_cfg != 0 && special_cfg != 255)
		{
			print_string("mode:           ",0,3,0);
			print_string(itoa(special_cfg),8*6,3,0);
		}
		else
			print_string("normal mode     ",0,3,0);
		print_string("LNB-power:      ",0,4,0);
		print_string("Sysclock :      ",0,5,0);
		print_string("Downmixer:      ",0,6,0);
		booting = 1;
		old_LNB_laststat = 255;
		old_si5328locked = 255;
		old_adf4351_lockstat = 255;
		return;
	}
	
	if(booting == 1 && (!adf4351_lockstat || !si5328locked || !LNB_laststat))
	{
		// show status
		if(LNB_laststat != old_LNB_laststat)
		{
			if(LNB_laststat) 
				print_string("OK",11*8,4,0); 
			else 
				print_string(itoa(LNB_error),11*8,4,0);
			
			old_LNB_laststat = LNB_laststat;
		}
		
		if(si5328locked != old_si5328locked)
		{
			if(si5328locked) print_string("LOCK",11*8,5,0); else print_string("wait",11*8,5,0);
			old_si5328locked = si5328locked;
		}
		
		if(adf4351_lockstat != old_adf4351_lockstat)
		{
			if(adf4351_lockstat) print_string("LOCK",11*8,6,0); else print_string("wait",11*8,6,0);
			old_adf4351_lockstat = adf4351_lockstat;
		}
		
		if(ubx_ack && old_GPS_sat != GPS_sat)
		{
			print_string("GPS SATs:       ",0,7,0);
			print_string(itoa(GPS_sat),9*8,7,0);
			if(GPS_lock == 1)
				print_string("LOCK ",11*8,7,0);
			else
				print_string("NOlck",11*8,7,0);
			old_GPS_sat = GPS_sat;
		}
		return;
	}
	else
	{
		if(booting != 2)
		{
			// all ready, go to normal display mode
			lcd_clear_screen(1);
			booting = 2;
			old_GPS_lock = 0xff;
			old_outqrg = 0;
			old_lnbqrg = 0xff;
			old_GPS_sat = 0xff;
			old_GPS_fix = 0xff;
			*old_GPS_lat = 0;
			*old_GPS_lon = 0;
		}
	}
	
	// normal display mode starts here
	
	char *p;
	u8 xpos;
	
	u16 outqrg = out_freq_table[actualADFswitch];
	
	if(old_GPS_lock != GPS_lock || old_ubx_ack != ubx_ack)
	{
		if(!GPS_lock && !ubx_ack)
		{
			if(ext10MHzReference == 0)
				print_string("O/TCXO",10*8,0,0);
			else
				print_string("ext10M",10*8,0,0);
			// no GPS, just clear the lines
			print_string("                ",0,5,0);
			print_string("System ready    ",0,6,0);
			print_string("                ",0,7,0);
		}
		else if(ubx_ack && !GPS_lock)
		{
			if(use_GPS)
				print_string(" GPS  ",10*8,0,0);
			else
				print_string(" OCXO ",10*8,0,0);	// special config 2: OCXO mode with GPS display
			print_string("                ",0,5,0);
			print_string("searching GPS   ",0,6,0);
			print_string("satellites:     ",0,7,0);
			print_string(itoa(GPS_sat),12*8,7,0);
		}
		else
		{
			if(use_GPS)
				print_string(" GPS  ",10*8,0,0);
			else
				print_string(" OCXO ",10*8,0,0);	// special config 2: OCXO mode with GPS display
			print_string("                ",0,5,0);
			print_string("                ",0,6,0);
			print_string("                ",0,7,0);
			old_GPS_sat = 0xff;
			old_GPS_fix = 0xff;
			*old_GPS_lat = 0;
			*old_GPS_lon = 0;
		}
		
		old_GPS_lock = GPS_lock;
		
		if(old_ubx_ack != ubx_ack)
			old_ubx_ack = ubx_ack;
	}
	
	if(GPS_time[0] != ' ' && memcmp(GPS_time,old_GPS_time,9))
	{
		print_string(GPS_time,0,1,0);
		memcpy(old_GPS_time,GPS_time,9);
	}
	
	if(old_outqrg != outqrg)
	{
		print_string("OUT:     ",0,3,0);
		p = itoa(outqrg);
		xpos = 8-strlen(p);
		print_string(p, xpos*8, 3, 0);
		print_string(".50", 8*8, 3, 0);
		print_string("MHz",12*8,3,0);	
	
		old_outqrg = outqrg;
	}
	
	if(old_lnbqrg != currentLO)
	{
		print_string("LNB:",0,4,0);
		if(currentLO == 27)
			print_string("25.45", 6*8, 4, 0);
		else
		{
			print_string(itoa(currentLO), 6*8, 4, 0);
			print_string(".00", 8*8, 4, 0);
		}
		print_string("MHz",12*8,4,0);	
		
		old_lnbqrg = currentLO;
	}
	
	if(GPS_lock)
	{
		if(	old_GPS_sat != GPS_sat ||
			(old_GPS_fix != GPS_fix && use_GPS) ||
			memcmp(old_GPS_lat,GPS_lat,GPS_STRLENGTH) || 
			memcmp(old_GPS_lon,GPS_lon,GPS_STRLENGTH))
		{
			print_string("SAT:",0,5,0);
			p = itoa(GPS_sat);
			xpos = 8-strlen(p);
			print_string("       ",4*8,5,0);	// clear line
			print_string(itoa(GPS_sat),xpos*8,5,0);
			
			if(GPS_fix)
				print_string("lock  ",10*8,5,0);
			else
				print_string("unlock",10*8,5,0);
			
			print_string("LAT:",0,6,0);
			print_string(GPS_lat,8*6,6,0);
			
			print_string("LON:",0,7,0);
			print_string(GPS_lon,8*5,7,0);
			
			if(GPS_lon[0] != ' ' && GPS_lat[0] != ' ')
				print_string(CalculateGridSquare(),8*9,1,0);
			
			old_GPS_sat = GPS_sat;
			old_GPS_fix = GPS_fix;
			memcpy(old_GPS_lat,GPS_lat,GPS_STRLENGTH);
			memcpy(old_GPS_lon,GPS_lon,GPS_STRLENGTH);
		}
		
		/*
		if(old_GPS_sat != GPS_sat)
		{
			print_string("SAT:",0,5,0);
			p = itoa(GPS_sat);
			xpos = 8-strlen(p);
			print_string("       ",4*8,5,0);	// clear line
			print_string(itoa(GPS_sat),xpos*8,5,0);

			old_GPS_sat = GPS_sat;
			old_GPS_fix = 0xff;
		}
		
		if(old_GPS_fix != GPS_fix && use_GPS)
		{
			if(GPS_fix)
				print_string("lock  ",10*8,5,0);
			else
				print_string("unlock",10*8,5,0);
				
			old_GPS_fix = GPS_fix;
		}

		u8 poschanged = 0;
		if(memcmp(old_GPS_lat,GPS_lat,GPS_STRLENGTH))
		{
			print_string("LAT:",0,6,0);
			print_string(GPS_lat,8*6,6,0);

			memcpy(old_GPS_lat,GPS_lat,GPS_STRLENGTH);
			poschanged = 1;
		}

		if(memcmp(old_GPS_lon,GPS_lon,GPS_STRLENGTH))
		{
			print_string("LON:",0,7,0);
			print_string(GPS_lon,8*5,7,0);

			memcpy(old_GPS_lon,GPS_lon,GPS_STRLENGTH);
			poschanged = 1;
		}
		
		if(poschanged)
		{
			if(GPS_lon[0] != ' ' && GPS_lat[0] != ' ')
				print_string(CalculateGridSquare(),8*9,1,0);
		}
		 * */
	}
}

// returns a pointer to an ASCII string in a static buffer
char *itoa(int value) 
{
	static char buffer[12];
	u8 c = sizeof(buffer)-1;
	
	memset(buffer,0,sizeof(buffer));

	do                             // write least significant digit of value that's left
	{
		buffer[--c] = (value % 10) + '0';    
		value /= 10;
	} while (value);

	return &buffer[c];
}

void lcd_settingsOK(u8 mode)
{
	lcd_clear_screen(0);
	print_string("AMSAT-DL  QO-100",0,0,0);
	print_string("Settings OK",0,2,0);
	print_string("Mode:",0,4,0);
	print_string(itoa(mode),8*7,4,0);
	print_string("please",0,6,0);
	print_string("power OFF now",0,7,0);
}