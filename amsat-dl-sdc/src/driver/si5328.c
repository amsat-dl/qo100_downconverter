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

#include "si5328.h"
#include "gps.h"
#include "i2c_native.h"
#include "secretsettings.h"

/*
 * Principle of operation
 * ======================
 * The Si5328 accepts two input clocks ranging from 8 kHz to 710 MHz and generates
 * two output clocks ranging from 8 kHz to 808 MHz. The Si5328 can also use its 
 * TCXO as a clock source for frequency synthesis.
 * 
 * internal OCXO mode: 
 * -------------------
 * 40 MHz from TCXO is connected to XA
 * XA is internally routed the CKIN2
 * the 40 MHz on CKIN2 is deviced by N32 (Register 46,47,48) by 20: 40MHz/20 = 2MHz
 * these 2 MHz are the input of the DSPLL
 * 
 * external GPS mode: 
 * ------------------
 * GPSDO 10 MHz is connected to CKIN1
 * to get a DSPLL clock of 2 MHz the register N32 must by: 5
 * 
 * similar for both modes:
 * -----------------------
 * the DSPLL multiplies by N2 (Register 40,41). N2 are two deiver in series, first N2_HS then N2_LS
 * this results in 2MHz * 8 (N2_HS) * 330 (N2_LS) = 5280 MHz which is the DSPLL output.
 * 
 * The DSPLL output is deviced by N1_HS: 5280/11 = 480 MHz
 * 
 * this is deviced by N1_LS for CKOUT1 and N2_L2 for CKOUT2
 * CKOUT1 is the LO frequency of the LNB: 480MHz/20(NC1_LS) = 24 MHz
 * CKOUT2 is the reference for the ADF4351: 480MHz/12(NC2_LS) = 40 MHz
 * 
 */

void setQRG(void);

u8 si5328locked = 0;
u8 currentLO = 0;
u8 ext10MHzReference = 0;
u8 use_GPS = 1;

#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))

typedef struct {
    u8 addr;
    u8 value;
} reg_set;

#define REG_BLOCK_LS(reg, value) {reg, value>>8}, {reg+1, value&0xFF}

#define FREE_RUN	0x40	// enable free run mode -> XAXB connected to CKIN2
#define CK_PRIOR2	0x04	// CKIN1 (GPS) has priority
#define CK_PRIOR1	0x00
#define BWSEL_REG	0xf0	// High BW: lower input jitter attenuation, but less self generated jitter
							// Low BW (higher reg val): better input jitter attenuation, but more self generated jitter
							// GPS module: higher values work much better
							// Bodnar always uses 15
#define CKSEL_CKIN2	0x40	// use CKIN2 fot XA/XB connection (free run mode))
#define AUTOSEL_REG	0x80	// auto revertive selection
#define HIST_DEL	0x17	// 10111 = 839 ms (longer than default for si5328 according to ref manual)
#define HIST_AVG	0xC8	// 11001 = 13,422 history averaging time
#define SFOUT2_REG	0x10	// CKOUT2: CMOS levels
#define SFOUT1_REG	0x05	// CKOUT1: LVPECL levels
#define FOS_THR		0x20	// we don't use FOS, so this setting has no meaning
#define VALTIME		0x08	// alarm remove time 100ms
#define LOCKT		0x01	// Retrigger intv. 106ms, this long time give better sync (ref. man recommends min. 53ms)
							// the LOCKT setting has no influence on jitter

/*
 * when the GPS module is vaiable:
 * ===============================
 * - by reading the serial line: check if a GPS module is available and
 *   if GPS is already valid
 * - if not: freerun mode and manual selection of CKIN2 (which is the T/OCXO via XAXB)
 * - if yes: freerun mode, with automatic clock selection
 */

// this register set contains all registers WITHOUT dividers
static const reg_set reg_set_univ_CKIN1_GPS[] = {
	
	{ 0, 0x14 },						// no free run
	{ 1, 0xe0 | CK_PRIOR2 | CK_PRIOR1},
	{ 2, 0x02 | BWSEL_REG},
	{ 3, 0x05 },						// use CKIN_1
	{ 4, 0x00 | HIST_DEL},				// manually select CKIN_1

    { 6, 0x00 | SFOUT2_REG | SFOUT1_REG},
	{ 9, 0x00 | HIST_AVG},
	{19, 0x00 | FOS_THR | VALTIME | LOCKT},
	{20, 0x3e},		// activate LOL output and INT_C1B output
    {21, 0xfc},		// ignore CSEL Pin -> Register 3 selects clock
	{55, 0x08 },
	{137, 0x01},	// fast lock

	// enable monitoring
	// LOS monitoring in Reg. 138 is enabled by default
	// LOS and FOS monitoring in Reg. 139 is enabled by default
	// disable FOS because we do not have any reference signal
	{139, 0xfc},
};

static const reg_set reg_set_univ_CKIN2_XAXB[] = 
{
    { 0, 0x14 | FREE_RUN},				// route XABA to CKIN_2
	{ 2, 0x02 | BWSEL_REG},
	{ 3, 0x05 | CKSEL_CKIN2},			// use CKIN_2
	{ 4, 0x00 | HIST_DEL},				// manually select CKIN_2

    { 6, 0x00 | SFOUT2_REG | SFOUT1_REG},
	{ 9, 0x00 | HIST_AVG},
	{19, 0x00 | FOS_THR | VALTIME | LOCKT},
	{20, 0x3e},		// activate LOL output and INT_C1B output
    {21, 0xfc},		// ignore CSEL Pin -> Register 3 selects clock
	{55, 0x08 },
	{137, 0x01},	// fast lock

	// enable monitoring
	// LOS monitoring in Reg. 138 is enabled by default
	// LOS and FOS monitoring in Reg. 139 is enabled by default
	// disable FOS because we do not have any reference signal
	{139, 0xfc},
};

// divider register values

// default values (24 MHz LNB-LO)
u16 N31_reg4445=1;		// divider of input signal, before DSPLL
u16 N32_reg4748=1;		// divider of input signal, before DSPLL
u16 N2HS_reg40=1;		// High Speed Divider for PLL loop
u16 N2L_reg4142=1;		// Low  Speed Divider for PLL loop
u16 N1HS_reg25=1;		// High speed divider at DSPLL output
u16 NC1_LS_reg3233=1;	// Low  speed divider at DSPLL output for LNB-LO
u16 NC2_LS_reg3536=1;	// Low  speed divider at DSPLL output for 40 MHz output

/*
 * the default values for the LNB LO are 24,25,26 MHz
 * but we shift the GPS reference by a few Hz for better signal quality
 * so we have to correct this frequency error

 * Calculate the ADF4351 correction value (in kHz) if the GPS reference is not on it's default frequency
 * fcorr = 390*(fgps*10*240/5/40) or the corresponding si5328 values
 * fcorr = 390 * ((fgps/N31) * N2H * N2L) / N1HS / N1L
 * then substract this value from the ADF4151 frequency
 * this shifts the output frequency of the mixer to the requested frequency (nearly, +- a few kHz due to the ADF4351's resolution)
*/

s32 calc_mixer_corr_value()
{
	s64 fgps = getGPSfrequency();
	fgps *= 1000;
	fgps /= N31_reg4445;
	fgps *= N2HS_reg40;
	fgps *= N2L_reg4142;
	fgps /= N1HS_reg25;
	fgps /= NC1_LS_reg3233;
	s64 curLO = (s64)currentLO * 1000000000;
	fgps -= curLO;
	fgps /= 1000;
	fgps *= 390;
	if(fgps > 0)
		fgps += 500; // round to next Hz
	else
		fgps -= 500;
	
	s32 v = (s32)(fgps / 1000)+1;
	
	return v;
}

static reg_set reg_set_LNB_LO[] = 
{
	{44 ,0},
	{45 ,0},
	{47 ,0},
	{48 ,0},
	{40 ,0},
	{41, 0},
	{42, 0},
	{25, 0},
	{32, 0},
	{33, 0},
	{35, 0},
	{36, 0},
};

#define _LSB(x)  (x & 0xff)
#define _MSB(x)  ((x >> 8))
#define _HIGHNIB(x) ((x-4)<<5)

void set_reg()
{
	u8 idx = 1;
	u8 *p = (u8 *)reg_set_LNB_LO;
	
	p[idx] = (_MSB((N31_reg4445 - 1)));
	idx += 2;
	p[idx] = _LSB(N31_reg4445 - 1);
	idx += 2;
	p[idx] = (_MSB((N32_reg4748 - 1)));
	idx += 2;
	p[idx] = _LSB(N32_reg4748 - 1);
	idx += 2;
	p[idx] = _HIGHNIB(N2HS_reg40);
	idx += 2;
	p[idx] = (_MSB((N2L_reg4142 - 1)));
	idx += 2;
	p[idx] = _LSB(N2L_reg4142 - 1);
	idx += 2;
	p[idx] = _HIGHNIB(N1HS_reg25);
	idx += 2;
	p[idx] = (_MSB((NC1_LS_reg3233 - 1)));
	idx += 2;
	p[idx] = _LSB(NC1_LS_reg3233 - 1);
	idx += 2;
	p[idx] = (_MSB((NC2_LS_reg3536 - 1)));
	idx += 2;
	p[idx] = _LSB(NC2_LS_reg3536 - 1);
	idx += 2;
}

void set_reg24()
{
	if(ext10MHzReference == 0)
		N31_reg4445 = 19;	// divider of input signal, before DSPLL
	else
		N31_reg4445 =10;	// if the 10 MHz Ref Input is in use
	
	if(ext10MHzReference == 1)
	{
		// use 10 MHz Ref Input
		printf("si:ext. 10 MHz ref\n");
		N2HS_reg40 = 10;	
		N2L_reg4142 = 528;
	}
	else if(tcxo_no_GPS == 1 && ubx_ack == 0)
	{
		// TCXO version, special config: use TCXO without GPS
		printf("si:TCXO 38,4 MHz\n");
		N32_reg4748 = 20;
		N2HS_reg40 = 5;	
		N2L_reg4142 = 550;
	}
	else if(ubx_ack == 0 || use_GPS == 0)
	{
		// no GPS detected, use internal OCXO with 40 MHz
		// or GPS detected, but user do not want to use the timepulse (special config)
		printf("si:40 MHz OCXO\n");
		N32_reg4748 = 40;
		N2HS_reg40 = 10;
		N2L_reg4142 = 528;
	}
	else if(GPS_lock == 0)
	{
		// GPS detected, but not locked, use internal 38,4 MHz TCXO
		printf("si:GPS unlocked: TCXO 38,4 MHz\n");
		N32_reg4748 = 20;
		N2HS_reg40 = 5;	
		N2L_reg4142 = 550;
	}
	else
	{
		// GPS detected and locked, use GPS
		printf("si:GPS locked\n");
		N2HS_reg40 = 10;	
		N2L_reg4142 = 528;
	}
	
	N1HS_reg25 = 11;		// High speed divider at DSPLL output
	NC1_LS_reg3233 = 20;// Low  speed divider at DSPLL output for LNB-LO
	NC2_LS_reg3536 = 12;// Low  speed divider at DSPLL output for 40 MHz output

	set_reg();
}

void set_reg25()
{
	if(ext10MHzReference == 0)
		N31_reg4445 = 19;	// divider of input signal, before DSPLL
	else
		N31_reg4445 =10;	// if the 10 MHz Ref Input is in use
	
	if(ext10MHzReference == 1)
	{
		// use 10 MHz Ref Input
		printf("si:ext. 10 MHz\n");
		N2HS_reg40 = 5;	
		N2L_reg4142 = 1120;
	}
	else if(tcxo_no_GPS == 1 && ubx_ack == 0)
	{
		// TCXO version, special config: use TCXO without GPS
		printf("si:TCXO 38,4 MHz\n");
		N32_reg4748 = 24;
		N2HS_reg40 = 5;	
		N2L_reg4142 = 700;
	}
	else if(ubx_ack == 0 || use_GPS == 0)
	{
		// no GPS detected, use internal OCXO with 40 MHz
		// or GPS detected, but user do not want to use the timepulse (special config)
		printf("si:40 MHz OCXO\n");
		N32_reg4748 = 40;
		N2HS_reg40 = 5;		// High Speed Divider for PLL loop
		N2L_reg4142 = 1120;	// Low  Speed Divider for PLL loop
	}
	else if(GPS_lock == 0)
	{
		// GPS detected, but not locked, use internal 38,4 MHz TCXO
		printf("si:GPS unlocked, TCXO 38,4 MHz\n");
		N32_reg4748 = 24;
		N2HS_reg40 = 5;	
		N2L_reg4142 = 700;
	}
	else
	{
		// GPS detected and locked, use GPS
		printf("si:GPS locked\n");
		N2HS_reg40 = 5;		// High Speed Divider for PLL loop
		N2L_reg4142 = 1120;	// Low  Speed Divider for PLL loop
	}

	N1HS_reg25 = 7;		// High speed divider at DSPLL output
	NC1_LS_reg3233 = 32;// Low  speed divider at DSPLL output for LNB-LO
	NC2_LS_reg3536 = 20;// Low  speed divider at DSPLL output for 40 MHz output

	set_reg();
}

void set_reg26()
{
	if(ext10MHzReference == 0)
		N31_reg4445 = 19;	// divider of input signal, before DSPLL
	else
		N31_reg4445 =10;	// if the 10 MHz Ref Input is in use

	if(ext10MHzReference == 1)
	{
		// use 10 MHz Ref Input
		printf("si:ext. 10 MHz\n");
		N2HS_reg40 = 5;	
		N2L_reg4142 = 1040;
	}
	else if(tcxo_no_GPS == 1 && ubx_ack == 0)
	{
		// TCXO version, special config: use TCXO without GPS
		printf("si:TCXO 38,4 MHz\n");
		N32_reg4748 = 24;
		N2HS_reg40 = 5;	
		N2L_reg4142 = 650;
	}
	else if(ubx_ack == 0 || use_GPS == 0)
	{
		// no GPS detected, use internal OCXO with 40 MHz
		// or GPS detected, but user do not want to use the timepulse (special config)
		printf("si:40 MHz OCXO\n");
		N32_reg4748 = 40;
		N2HS_reg40 = 5;		// High Speed Divider for PLL loop
		N2L_reg4142 = 1040;	// Low  Speed Divider for PLL loop
	}
	else if(GPS_lock == 0)
	{
		// GPS detected, but not locked, use internal 38,4 MHz TCXO
		printf("si:GPS unlocked, TCXO 38,4 MHz\n");
		N32_reg4748 = 24;
		N2HS_reg40 = 5;	
		N2L_reg4142 = 650;
	}
	else
	{
		// GPS detected and locked, use GPS
		printf("si:GPS locked\n");
		N2HS_reg40 = 5;		// High Speed Divider for PLL loop
		N2L_reg4142 = 1040;	// Low  Speed Divider for PLL loop
	}

	N1HS_reg25 = 5;		// High speed divider at DSPLL output
	NC1_LS_reg3233 = 40;// Low  speed divider at DSPLL output for LNB-LO
	NC2_LS_reg3536 = 26;// Low  speed divider at DSPLL output for 40 MHz output

	set_reg();
}

void set_reg27() // 27,2 MHz
{
	N31_reg4445 = 133;	// divider of input signal, before DSPLL
	N2HS_reg40 = 8; 	// High Speed Divider for PLL loop
	N2L_reg4142 = 4760;	// Low  Speed Divider for PLL loop
	N1HS_reg25 = 4;		// High speed divider at DSPLL output
	NC1_LS_reg3233 = 50;// Low  speed divider at DSPLL output for LNB-LO
	NC2_LS_reg3536 = 34;// Low  speed divider at DSPLL output for 40 MHz output

	set_reg();
}


//internal helper
static void write_regset(i2c_dev i2c, u8 addr, const reg_set *set, u16 len);
static void si5328_perform_ical(i2c_dev i2c, u8 addr);
static u8 read_reg(i2c_dev i2c, u8 addr, u8 reg);

int si5328_init(i2c_dev i2c, u8 addr)
{
    int res = i2c_probe_addr(i2c, addr);
    if(res == 0) 
	{
        printf("si:found.\n");
		smbus8_write_byte(i2c, addr, 136, 0x80);	// reset si5328
		__delaywdt_ms(100);
		// init registers
		printf("si:init\n");
		if(ext10MHzReference == 0)
		{
			// by default select the internal OCXO/TCXO as clock source
			write_regset(i2c, addr, reg_set_univ_CKIN2_XAXB, ELEMENTS(reg_set_univ_CKIN2_XAXB));
		}
		else
		{
			// for the ext. 10 MHz Reference use the GPS map (which uses the TIMEPULSE line)
			write_regset(i2c, addr, reg_set_univ_CKIN1_GPS, ELEMENTS(reg_set_univ_CKIN1_GPS));
		}
		// set LNB LO registers and do ICAL
		setQRG();
    } 
	else 
	{
        printf("si:not found!\n");  
    }    
    
    return res;
}

void si5328_set_freq(i2c_dev i2c, u8 addr, u8 freq_MHz)
{
	if(freq_MHz == 24) 
		set_reg24();
	
	if(freq_MHz == 25) 
		set_reg25();
	
	if(freq_MHz == 26) 
		set_reg26();
	
	if(freq_MHz == 27) 
		set_reg27();
	
	write_regset(i2c, addr, reg_set_LNB_LO, ELEMENTS(reg_set_LNB_LO));
	printf("si:LNB: %d\n",freq_MHz);
	si5328_perform_ical(i2c, addr);
}

static void si5328_perform_ical(i2c_dev i2c, u8 addr)
{
	printf("si:ical\n");
	if (smbus8_write_byte(i2c, addr, 136, 0x40) != 0) 
	{
		printf("si:I2C fail\n");            
	}
}

// switch from local osc to GPS if we get a GPS lock
void si5328_checkGPS(i2c_dev i2c, u8 addr)
{
static u8 old_GPS_lock = 0;
static u8 old_ubx_ack = 0;

	if(old_ubx_ack == 0 && ubx_ack == 1 && GPS_lock == 0)
	{
		old_ubx_ack = ubx_ack;
		
		// GPS modul detected, but no GPS lock
		// switch to local 38,4 MHz osc
		printf("si:GPS ok, no lock\n");
        write_regset(i2c, addr, reg_set_univ_CKIN2_XAXB, ELEMENTS(reg_set_univ_CKIN2_XAXB));
		// set LNB LO registers and do ICAL
		currentLO = 0xff;
		setQRG();
	}

	if(old_GPS_lock == 0 && GPS_lock == 1 && use_GPS == 1)
	{
		old_GPS_lock = 1;
		
		// we just got a GPS lock, switch si5328 to CKIN_1
		printf("GPS:locked\nsi:syncing GPS\n");
        write_regset(i2c, addr, reg_set_univ_CKIN1_GPS, ELEMENTS(reg_set_univ_CKIN1_GPS));
		// set LNB LO registers and do ICAL
		currentLO = 0xff;
		setQRG();
	}
}

// 1=just locked (reported only once after lock is established), 0=no lock
u8 check_si5328_status(i2c_dev i2c, u8 addr)
{
	u8 res, ret = 0;
	static u8 lastlock = 0xff;
	
	u8 lock = gpio_get_input(PIN_SI5328_LOL);
	if(lock != lastlock)
	{
		lastlock = lock;
		if(lock == 1)
		{
			printf("si:unlocked\n");
			si5328locked = 0;
			gpio_set_lvl_high(PIN_LED_SI_LOCK);
		}
		else
		{
			printf("si:locked\n");
			si5328locked = 1;
			gpio_set_lvl_low(PIN_LED_SI_LOCK);
			ret = 1;
		}
	}
	
	/*
	static u8 r131 = 0xff;
	res = read_reg(i2c, addr, 131);
	if(res) 
		smbus8_write_byte(i2c, addr, 131, 0);
	if(res != r131)	// show each error only once
	{
		r131 = res;
		
		if(!(res & 1)) printf("Si5328 XA/XB signal\n");
		if(!(res & 2)) printf("Si5328 CKIN1 signal\n");
		if(!(res & 4)) printf("Si5328 CKIN2 signal\n");
		
		if(res & 1) printf("Si5328 XA/XB loss of signal\n");
		if(res & 2) printf("Si5328 CKIN1 loss of signal\n");
		if(res & 4) printf("Si5328 CKIN2 loss of signal\n");
	}
	*/
	static u8 lastinput = 0xff;
	res = read_reg(i2c, addr, 128);
	if(res != lastinput)
	{
		//if(!(res & 1) && !(res & 2)) printf("Si5328: Input = disconnected !\n");
		if(res & 1) 
		{
			//printf("Si5328: Input = CKIN1 (GPS)\n");
			GPS_fix = 1;
		}
		if(res & 2) 
		{
			//printf("Si5328: Input = CKIN2 (TCXO/OCXO)\n");
			GPS_fix = 0;
		}
		lastinput = res;
	}
	
	// not used because we do not use FOS
	/*
	res = read_reg(i2c, addr, 132);
	if(res & 2) printf("PLL loss of lock\n");
	if(res & 4) printf("CKIN1 freq offset\n");
	if(res & 8) printf("CKIN2 freq offset\n");
	if(res) 
	{
		smbus8_write_byte(i2c, addr, 132, 0);
	}*/
	
	return ret;
}

static void write_regset(i2c_dev i2c, u8 addr, const reg_set *set, u16 len)
{
    int errors = 0;
	
    while(len--) {
        if (0 != smbus8_write_byte(i2c, addr, set->addr, set->value)) errors++;
        set++;
    }
    
 	if(errors > 0)
	{
		printf("si:write_regset %i errors.\n", errors);
	}
}

static u8 read_reg(i2c_dev i2c, u8 addr, u8 reg)
{
	u8 val;
	
	int res = smbus8_read_byte(i2c, addr, reg, &val);
	return val;
}