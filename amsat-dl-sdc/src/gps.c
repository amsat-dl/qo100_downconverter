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
#include <stdlib.h>
#include <pinmap.h>
#include <driver/gpio.h>
#include "gps.h"
#include "timer.h"
#include <driver/usart.h>

void print_string(char *s, u8 x, u8 y, u8 underline);
void send_UBX_string(u8 class, u8 id, u16 len, u8 *payload);
char *itoa(int value);

u8 GPS_lock = 0;
u8 GPS_fix = 0;
u8 GPS_sat = 0;
char GPS_lat[GPS_STRLENGTH] = {"        "};
char GPS_lon[GPS_STRLENGTH] = {"        "};
char GPS_time[9] = {" "};
u8 ubx_ack = 0;

void eval_gps(char *gpsbuf)
{
char elem[15][16];	// 15 elements, max len=16
u8 eidx=0;

	// following strtol requires that gpsbuf length > 2, or it crashes
	// so we check for valid length
	if(strlen(gpsbuf) < 20 || strlen(gpsbuf) > 100)
		return;	// invalid length

	// check checksum
	u8 chksum = gpsbuf[0];
	for(u8 i=1; i<strlen(gpsbuf)-3; i++)
		chksum ^= gpsbuf[i];
	
	u8 chksumgps = strtol(gpsbuf + (strlen(gpsbuf)-2),NULL,16);
	if(chksumgps != chksum) return;
	
	// check sentence
	if(gpsbuf[2] == 'G' && gpsbuf[3] == 'G' && gpsbuf[4] == 'A')
	{
		// GPGGA Sentence
		//printf("%s\n",gpsbuf);
		
		// split elements
		char *ps = gpsbuf;
		while(1)
		{
			char *pe = strchr(ps,',');
			if(pe == NULL) break;
			memset(elem[eidx],0,16);
			memcpy(elem[eidx++],ps,pe-ps);
			ps = pe+1;
			if(ps >= (gpsbuf + strlen(gpsbuf))) break;
			if(eidx >= 15) break;
			CLRWDT();
		}
		
		//for(u8 i=0; i<eidx; i++) printf("%d:%s\n",i,elem[i]);
		// elem idxs
		// 1 ... time: HHMMSS
		// 2 ... Lat: ddmm.mmmm...
		// 3 ... N or S
		// 4 ... Lon: ddmm.mmmm...
		// 5 ... E or W
		// 6 ... fix: 0=no fix, 1 or 2: fix
		// 7 ... no of sats
		
		GPS_time[0] = elem[1][0];
		GPS_time[1] = elem[1][1];
		GPS_time[2] = ':';
		GPS_time[3] = elem[1][2];
		GPS_time[4] = elem[1][3];
		GPS_time[5] = ':';
		GPS_time[6] = elem[1][4];
		GPS_time[7] = elem[1][5];
		GPS_time[8] = 0;
		
		if(elem[6][0] != '1' && elem[6][0] != '2')
		{
			//printf("no fix: %s\n",elem[6]);
			GPS_lock = 0;
			GPS_sat = 0;
			*GPS_lat = 0;
			*GPS_lon = 0;
			return;
		}
		GPS_lock = 1;
		
		GPS_sat = atoi(elem[7]);
		
		memcpy(GPS_lat,elem[2],2);
		GPS_lat[2] = 'd';
		memcpy(GPS_lat+3,elem[2]+2,5);
		GPS_lat[8] = elem[3][0];
		GPS_lat[9] = 0;
		
		memcpy(GPS_lon,elem[4],3);
		GPS_lon[3] = 'd';
		memcpy(GPS_lon+4,elem[4]+3,5);
		GPS_lon[9] = elem[5][0];
		GPS_lon[10] = 0;
	}
}

u8 ubx_class = 0;
u8 ubx_id = 0;
u16 ubx_len = 0;
u16 p_len = 0;
u8 payload[50];
char gpsbuf[85];	// max. length of a GPS sentence can be 82
u8 gpsbufidx = 0xff;

void rx_gps(int c)
{
static u8 stat = 0;
static u16 len_lsb;

	switch(stat)
	{
	case 0:	// wait for 0xb5 (UBX or $ (NMEA))
		if(c == 0xb5)
		{
			stat++;
		}
		if(c == '$')
		{
			gpsbufidx = 0;
			stat = 100;
		}
		break;
		
	case 1:	// wait for 0x62
		if(c == 0x62)
			stat++;
		else
			stat = 0;	// invalid
		break;
	
	case 2: // get class
		ubx_class = c;
		if(c==1 || c==2 || c==4 || c==5 || c==6 || c==9 || c==0x0a || c==0x0b || c==0x0d || c==0x10 || c==0x13 || c==0x21 || c==0x27 || c==0x28)
			stat++;
		else
			stat = 0;	// invalid class
		break;
		
	case 3: // get ID
		ubx_id = c;
		//printf("GPS:UBX class: %d, id: %d\n",ubx_class,c);
		stat ++;
		break;
		
	case 4: // get length LSB
		len_lsb = c;
		stat++;
		break;
		
	case 5: // get length MSB
		ubx_len = c;
		ubx_len <<= 8;
		ubx_len |= len_lsb;
		//printf("UBX len: %d\n",ubx_len);
		if(ubx_len == 0 || ubx_len >= sizeof(gpsbuf))
			stat = 0;	// finish, no payload
		else
		{
			stat++;
			p_len = 0;
		}
		break;
		
	case 6: // get payload
		if(ubx_class == 5)
		{
			gpsbuf[p_len++] = c;
			if(p_len >= sizeof(gpsbuf) || p_len == ubx_len)
			{
				ubx_ack = 1;
				
				// all read, finish
				stat = 0;
			}
			
		}
		else
			stat = 0;
		break;
		
	// NMEA reception
		
	case 100:
		if(c == '\n' || c == '\r')
		{
			// finished
			gpsbuf[gpsbufidx] = 0;
			eval_gps(gpsbuf);
			stat = 0;
		}
		else
		{
			if(gpsbufidx >= sizeof(gpsbuf))
			{
				// too long, ignore
				stat = 0;
				return;
			}
			gpsbuf[gpsbufidx++] = c;

			// check sentence
			if(gpsbufidx == 5 && (gpsbuf[2] != 'G' || gpsbuf[3] != 'G' || gpsbuf[4] != 'A'))
			{
				// we do not need this sentence, ignore
				stat = 0;
				return;
			}
		}
		break;
	}
}

// UBX-CFG-TP5 message
#define GPSBASEQRG 19000000
#define GPSQRG (GPSBASEQRG + 35)

u8 gps_config_data[] = 
{
	0,		// time pulse selection: 0=TP, 1=TP2
	1,		// message version
	0,		// reserved
	0,
	50,		// Antenne cable delay in ns
	0,
	0,		// RF group delay
	0,
	
	0,		// no output if not locked to Sat
	0,	
	0,
	0,
	// ATTENTION !
	// DO NOT shift these bytes, they are accessed by function: clear_GPS_shift() !!!
	// to avoid a beat ("Schwebung") between the UBLOX internal clock and the TIMEPULSE output
	// we need a small difference between the wanted clock and the real clock
	(u8)(GPSQRG & 0xff),
	(u8)((GPSQRG>>8)&0xff),
	(u8)((GPSQRG>>16)&0xff),
	(u8)((GPSQRG>>24)&0xff),
	
	0,		// pulse len ratio = 0%
	0,
	0,
	0x80,
	0,		// pulse len ratio if locked: 50%
	0,		// "islength" bit is set to 0, so this value is the duty cycle
	0,
	0x80,
	
	0,		// user conf. time pulse delay
	0,
	0,
	0,
	0xef,	// flages, see below
	0,
	0,
	0
};

/*
 * Flags in above message:
 * bit 0 ... active ...			1=enable time pulse output
 * bit 1 ... lockGnssFreq ...	1=lock if GNSS is valid, otherwise use local clock
 * bit 2 ... lockedOtherSet ...	1=use the LOCKED parameters if locked
 * bit 3 ... isFreq ...			1=interpret above parameters as frequency
 * bit 4 ... isLength ...		0=interpret above parameters as duty cycle
 * bit 5 ... alignToTow ...		1=align to top of a second, this is always set by default in FTL products
 * bit 6 ... polarity ...		1 (we don't care)
 * bit 7-10. gridUtcGnss ...	1=use GPS time
 * bit11-13. syncMode ...		0=if locked use locked parameters, never switch back to unlocked parameters even if not locked 
 */

void clear_GPS_shift()
{
	u8 idx = 12;
	gps_config_data[idx++] = (u8)(GPSBASEQRG & 0xff);
	gps_config_data[idx++] = (u8)((GPSBASEQRG>>8)&0xff);
	gps_config_data[idx++] = (u8)((GPSBASEQRG>>16)&0xff);
	gps_config_data[idx++] = (u8)((GPSBASEQRG>>24)&0xff);
}

// returns the real TIMEPULSE frequency of the UBLOX GPS module
u32 getGPSfrequency()
{
	u32 gf = gps_config_data[15];
	gf <<= 8;
	gf |= gps_config_data[14];
	gf <<= 8;
	gf |= gps_config_data[13];
	gf <<= 8;
	gf |= gps_config_data[12];
	
	//printf("GPS actual freq: %ld\n",gf);
	
	return gf;
}

// activate 10 MHz output in ublox module
// only every 2 seconds and only if not done already
// do this 100 times max.
void init_gps()
{
	if(gps_init_timer == 1 && ubx_ack == 0)
	{
		gps_init_timer = 0;
		// configure max8 for 10 MHz output
		send_UBX_string(6,0x31,sizeof(gps_config_data),gps_config_data);
		// max answers with 05,01 (ACK)
	}
}

void send_UBX_string(u8 class, u8 id, u16 len, u8 *payload)
{
	u8 txbuf[50];
	int idx = 0;
	static u8 giveup = 0;
	
	if(giveup == 100) printf("GPS:not found\n");
	if(++giveup > 100) return;
	
	printf("GPS:init Ublox %d of 100\n",giveup);
	
	txbuf[idx++] = 0xb5;
	txbuf[idx++] = 0x62;
	txbuf[idx++] = class;
	txbuf[idx++] = id;
	txbuf[idx++] = (u8)(len & 0xff);
	txbuf[idx++] = (u8)(len >> 8);
	for(int i=0; i<len; i++)
	{
		txbuf[idx++] = payload[i];
	}
	
	// calc chksum
	u8 CK_A = 0, CK_B = 0;
	for(int i=2; i<idx; i++)
	{
		CK_A = CK_A + txbuf[i];
		CK_B = CK_B + CK_A;
	}
	txbuf[idx++] = CK_A;
	txbuf[idx++] = CK_B;
	
	if(idx >= 50)
		return; // send_UBX_string: idx too long
	
	for(int i=0; i<idx; i++)
		putgps(txbuf[i]);
	
	printf("\n");
}

/* calc qthloc from GPS_lat and GPS_lon
 * 
 * JO12AB
 * 
 * J ... longitude A..R ... 180W to 180E
 * 1 ... longitude 0..9 ... +0-18deg
 * A ... longitude a..x ... +0-2deg
 * 
 * O ... latitude  A..R ... 90S to 90N
 * 2 ... latitude  0..9 ... +0-9deg
 * B ... latitude  a..x ... +0-1deg (finest resolution: 1/24 = 0.04167 deg)
 * 
 * GPS delivers 1/100 min, which is 1/6000 deg.
 * therefore we multiplay degrees with 10.000, so we can use integer arithmetics
*/

#define GVAL(x,y)  ((x-'0')*y)

char *CalculateGridSquare() 
{
static char grid[7];

	// calculate lon
	u32 lon = GVAL(GPS_lon[0],100) + GVAL(GPS_lon[1],10) + GVAL(GPS_lon[2],1);		// full degrees
	u32 lon_min = GVAL(GPS_lon[4],10) + GVAL(GPS_lon[5],1);	// full minutes
	lon_min *= 100;
	lon_min += (GVAL(GPS_lon[7],10) + GVAL(GPS_lon[8],1));	// min * 1000
	lon_min = (lon_min * 100) / 60;				// deg * 1000
	lon = lon*10000 + lon_min;					// complete lat * 1000
	// we have now lon * 10000
    // the last char is W or E, west is negative
    if(GPS_lon[9] == 'W') lon = -lon;
    // and make it positive, 0 starts in west -180
    lon += 1800000;

   	// calculate lat
	u32 lat = GVAL(GPS_lat[0],10) + GVAL(GPS_lat[1],1);		// full degrees
	u32 lat_min = GVAL(GPS_lat[3],10) + GVAL(GPS_lat[4],1);	// full minutes
	lat_min *= 100;
	lat_min += (GVAL(GPS_lat[6],10) + GVAL(GPS_lat[7],1));	// min * 1000
	lat_min = (lat_min * 100) / 60;				// deg * 1000
	lat = lat*10000 + lat_min;					// complete lat * 1000
	// i.e.: 48d4927 is now 488211 which is 48,8211 deg * 10.000
	// so we have now lat * 10000
    // the last char is N or S, south is negative
    if(GPS_lat[8] == 'S') lat = -lat;
    // and make it positive, 0 starts in south -90
    lat += 900000;

    u32 lonm = lon * 60L;
    u32 longf = lonm / (20L*60L*10000L);
    u32 lonrest1 = lonm - longf*(20L*60L*10000L);
    u32 lonmid = lonrest1 / (2L*60L*10000L);

    u32 latm = lat * 60L;
    u32 latgf = latm / (10L*60L*10000L);
    u32 latrest1 = latm - latgf*(10L*60L*10000L);
    u32 latmid = latrest1 / (1L*60L*10000L);
    
    grid[0] = 'A' + longf;
    grid[2] = '0' + lonmid;
    grid[4] = 'A' + ((lonrest1 - lonmid*(2L*60L*10000L)) / (5L*10000L));
    
    grid[1] = 'A' + latgf;
    grid[3] = '0' + latmid;
    grid[5] = 'A' + ((latrest1 - latmid*(1L*60L*10000L)) / (25L*1000L));
    
    grid[6] = 0;
    
	return grid;
}