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

#include "adf4351.h"
#include "gps.h"
#include "si5328.h"

#include <pinmap.h>

//
//prototypes
//
static void adf4351_default(adf4351_ctx* ctx);
static void adf4351_setup(adf4351_ctx* ctx);
static void adf4351_write_latch(u32 value);
static int  adf4351_calc_counters(adf4351_ctx* ctx);

//dump all registers
static void adf4351_regdump(adf4351_ctx* ctx);

static void adf4351_write_reg0(adf4351_ctx* ctx);
static void adf4351_write_reg1(adf4351_ctx* ctx);
static void adf4351_write_reg2(adf4351_ctx* ctx);
static void adf4351_write_reg3(adf4351_ctx* ctx);
static void adf4351_write_reg4(adf4351_ctx* ctx);
static void adf4351_write_reg5(adf4351_ctx* ctx);
static inline u32 gcd(u32 a, u32 b);

//
//main control
//
void adf4351_init(adf4351_ctx* ctx)
{
	memset(ctx, 0, sizeof(*ctx));

	//copy settings
	ctx->init = TRUE;

	//init latch enable and chipselect gpios
    gpio_set_lvl_low(PIN_ADF_LE);
    gpio_set_lvl_high(PIN_ADF_CE);

	//apply default settings
	adf4351_default(ctx);
    
    __delaywdt_us(100);
}

static u32 g_freq = 0;

/*
 * the required freuencies with 1kHz resolution work with a prescaler of 5
 * The Ref frequ is 40 MHz. Divided by 5 is the fpdf = 8 MHz.
 * 
 * Input blocks:
 * doubler: 0 (off)	reg2.refdbl=0;
 * counter: 5		reg2.refdiv=0;
 * divider: 0 (off)	reg2.rcnt=5;
 * 
 * calculate the N divider accoreding the manual
 * 
 * Output blocks:
 * the output divider is set to 2,4 or 8 to move the VCO output
 * frequency between 2,2 and 4,4 GHz
 * 
 * For details see the Excel sheet: adf4351_calc.ods hosted by DJ0ABR (dj0abr@darc.de)
 * this sheet shows how we can get 1kHz resolution with only full integers for all divider blocks
 * 
 */

int adf4351_set_freq(adf4351_ctx* ctx, u32 freq)
{
    //check status
	if(!ctx->init) return ERROR_NODEV;
	
	// substract a correction value if the LNBLO is not exactly as the default
	s32 fcorr = 0;
	if(GPS_lock == 1 && ext10MHzReference == 0 && use_GPS == 1)
	{
		fcorr = calc_mixer_corr_value();
		freq -= fcorr;
	}
	
	if(freq < 699980 || freq > 700020) 
		printf("ADF:%ld kHz (corr:%ld)\n",freq,fcorr);
	
    g_freq = freq;
	
	ctx->ref = 40000;			// Reference input gets 40 MHz
	ctx->vco_freq = freq;		// we want this output frequency
    ctx->reg1.prescaler = 1;	// internal prescaler 8/9 because we need the VCO up to 4,4GHz
	
	// shift the VCO output frequency in a range between 2,2 to 4,4 GHz by selecting the right output divider
	if(ctx->vco_freq < 550000)
		ctx->reg4.divsel = 3; // = :8
	else if(ctx->vco_freq < 1100000)
		ctx->reg4.divsel = 2; // = :4
	else
		ctx->reg4.divsel = 1; // = :2
	
	ctx->reg2.rcnt   = 5;		// input divider 40MHz/5 = 8MHz(pfd) for all frequencies
	ctx->pfd = ctx->ref / ctx->reg2.rcnt;
	ctx->reg2.refdiv = 0;		// no input divider
	ctx->reg2.refdbl = 0;		// no input doubler
	ctx->reg3.bscm = 0;
	ctx->reg4.bsclkdiv = (40000/ctx->reg2.rcnt)/125+1; // should be <=125  =  fpfd/bsclkdiv = 8000/64 = 125

	// finally calculate the N counter
	adf4351_calc_counters(ctx);

	if(ctx->reg4.vcopd == 0) {
		adf4351_setup(ctx);
    }

	return 0;
}

// calc all counters for given frequency
static int adf4351_calc_counters(adf4351_ctx* ctx)
{
    ctx->reg2.ldf = 0;
	// MOD is 4000 for all frequencies
	ctx->reg1.mod = 4000;
	
	// calculate the VFO output frequency according the output divider
	u64 vfoout = 0;
	if(ctx->vco_freq < 550000)
		vfoout = ctx->vco_freq * 8L;
	else if(ctx->vco_freq < 1100000)
		vfoout = ctx->vco_freq * 4L;
	else
		vfoout = ctx->vco_freq * 2L;

	// calculate INT and FRAC. We need some decimals but want to use integer arithmetic
	// therefore multiply by 10000 and do the calculations
	// this requires 64bit integers
	u64 Nmal10000 = (vfoout*10000L) / (u64)ctx->pfd;
	u32 INT = (u32)(Nmal10000/10000);
	ctx->reg0.integer = (u16)(INT);
	u32 rest = Nmal10000 - (u32)ctx->reg0.integer * 10000L;
	ctx->reg0.frac = (u16)((u32)(ctx->reg1.mod/1000) * rest) / 10;
	
	return 0;
}

//get PLL-lock
BOOL adf4351_get_lock(adf4351_ctx* ctx)
{
	return (gpio_get_input(PIN_ADF_LOCK)==0)?FALSE:TRUE;
}

u8 adf4351_lockstat = 0;

void check_ADF5341_status(adf4351_ctx* ctx)
{
	
	
	u8 stat = gpio_get_input(PIN_ADF_LOCK);
	
	if(stat != adf4351_lockstat)
	{
		if(stat == 0) 
		{
			printf("ADF:unlocked\n");
			gpio_set_lvl_high(PIN_LED_ADF_LOCK);
		} 
		else 
		{
			printf("ADF:locked\n");
			gpio_set_lvl_low(PIN_LED_ADF_LOCK);
		}
		adf4351_lockstat = stat;
	}
}

//set mode of operation
void adf4351_set_mode(adf4351_ctx* ctx, BOOL enable)
{
	//check status
	if(!ctx->init) return;

	//update register
	if(enable) {
		ctx->reg4.vcopd = 0;		//no VCO power down
		ctx->reg4.rfoutena = 1;		//RF out enable
		ctx->reg2.pd = 0;           //Power-Down disable

		//do complete power-up sequence just to be sure
		adf4351_setup(ctx);
	}  else {
		ctx->reg4.vcopd = 1;		//VCO power down
		ctx->reg4.rfoutena = 0;		//RF out enable
		ctx->reg2.pd = 1;           //Power-Down enable

		//write control register
		adf4351_write_reg4(ctx);
		adf4351_write_reg2(ctx);
	}
}

//set tuning parameters
void adf4351_set_tune(adf4351_ctx* ctx, u8 icp, u8 pout, BOOL lownoise)
{
    //update registers
    ctx->reg2.lownoise = lownoise?0:3;
    ctx->reg2.cpc      = icp;
    ctx->reg4.rfoutpwr = pout;

    /*if(ctx->reg4.vcopd == 0) {
        adf4351_setup(ctx);
    }*/
}

//set ADF4351 to default state
static void adf4351_default(adf4351_ctx* ctx)
{
	ctx->reg1.phase     = 1; //recommeded value
	ctx->reg2.muxout    = 6; //digital lock detect
	ctx->reg2.cpc       = 7;
	ctx->reg2.lownoise  = 3; //low spurs mode
	ctx->reg2.pdpol     = 1;
	ctx->reg4.fbsel     = 1; //feedback directly from vco core
	ctx->reg4.rfoutena  = 1; //RF output enabled
	ctx->reg4.rfoutpwr  = 3;
    ctx->reg4.vcopd     = 1; //VCO power down
    ctx->reg5.ldpinmod  = 1; //digital lock detect
}

static void adf4351_setup(adf4351_ctx* ctx)
{
    adf4351_write_reg5(ctx);
    adf4351_write_reg4(ctx);
    adf4351_write_reg3(ctx);
    adf4351_write_reg2(ctx);
    adf4351_write_reg1(ctx);
    adf4351_write_reg0(ctx);
}

static void adf4351_write_reg0(adf4351_ctx* ctx)
{
	if(ctx->init) {
        u32 data = 0;

        data |= (((u32)ctx->reg0.frac&0x0FFF)<<3);        
        data |= (((u32)ctx->reg0.integer&0xFFFF)<<15);

        adf4351_write_latch(data);
    }
}

static void adf4351_write_reg1(adf4351_ctx* ctx)
{
    if(ctx->init) {
        u32 data = 1;

        data |= (((u32)ctx->reg1.mod&0x0FFF)<<3);
        data |= (((u32)ctx->reg1.phase&0x0FFF)<<15);
        data |= (((u32)ctx->reg1.prescaler&0x0001)<<27);
        
        adf4351_write_latch(data);
    }
}

static void adf4351_write_reg2(adf4351_ctx* ctx)
{
    if(ctx->init) {
        u32 data = 2;

        data |= (((u32)ctx->reg2.cntreset&0x0001)<<3);
        data |= (((u32)ctx->reg2.cpts&0x0001)<<4);
        data |= (((u32)ctx->reg2.pd&0x0001)<<5);
        data |= (((u32)ctx->reg2.pdpol&0x0001)<<6);
        data |= (((u32)ctx->reg2.ldp&0x0001)<<7);
        data |= (((u32)ctx->reg2.ldf&0x0001)<<8);
        data |= (((u32)ctx->reg2.cpc&0x000F)<<9);
        data |= (((u32)ctx->reg2.dblbuff&0x0001)<<13);
        data |= (((u32)ctx->reg2.rcnt&0x03FF)<<14);
        data |= (((u32)ctx->reg2.refdiv&0x0001)<<24);
        data |= (((u32)ctx->reg2.refdbl&0x0001)<<25);
        data |= (((u32)ctx->reg2.muxout&0x0007)<<26);
        data |= (((u32)ctx->reg2.lownoise&0x0003)<<29);

        adf4351_write_latch(data);
    }  
}

static void adf4351_write_reg3(adf4351_ctx* ctx)
{
    if(ctx->init) {
        u32 data = 3;

        data |= (((u32)ctx->reg3.clkdiv&0x0FFF)<<3);
        data |= (((u32)ctx->reg3.clkdivmode&0x0003)<<15);
        data |= (((u32)ctx->reg3.csr&0x0001)<<18);
        data |= (((u32)ctx->reg3.chcancel&0x0001)<<21);
        data |= (((u32)ctx->reg3.abp&0x0001)<<22);
        data |= (((u32)ctx->reg3.bscm&0x0001)<<23);

        adf4351_write_latch(data);
    }
}

static void adf4351_write_reg4(adf4351_ctx* ctx)
{
    if(ctx->init) {
        u32 data = 4;

        data |= (((u32)ctx->reg4.rfoutpwr&0x0003)<<3);
        data |= (((u32)ctx->reg4.rfoutena&0x0001)<<5);
        data |= (((u32)ctx->reg4.auxpwrout&0x0003)<<6);
        data |= (((u32)ctx->reg4.auxoutena&0x0001)<<8);
        data |= (((u32)ctx->reg4.auxoutsel&0x0001)<<9);
        data |= (((u32)ctx->reg4.mtld&0x0001)<<10);
        data |= (((u32)ctx->reg4.vcopd&0x0001)<<11);
        data |= (((u32)ctx->reg4.bsclkdiv&0x00FF)<<12);
        data |= (((u32)ctx->reg4.divsel&0x0007)<<20);
        data |= (((u32)ctx->reg4.fbsel&0x0001)<<23);
        
        adf4351_write_latch(data);
    }
}

static void adf4351_write_reg5(adf4351_ctx* ctx)
{
    if(ctx->init) {
        u32 data = 5;

        data |= (((u32)ctx->reg5.ldpinmod&0x0003)<<22);

        adf4351_write_latch(data);
    }
}

/*static inline u32 gcd(u32 a, u32 b)
{
    while((a%b) != 0) {
        u32 c = a;

        if(a < b) {
            a = b;
            b = c;
        } else {
            a = b;
            b = c%b;
        }
    }
    
    return b;
}*/

//write value into latch
static void adf4351_write_latch(u32 value)
{
    u32 mask = 0x80000000ULL;

    gpio_set_lvl_low(PIN_SIO_CLK);
    __delaywdt_us(1);
	//write 32bit
    while(mask) {
        if(value&mask) {
            gpio_set_lvl_high(PIN_SIO_DATA);
        } else {
            gpio_set_lvl_low(PIN_SIO_DATA);
        }
        __delaywdt_us(1);
        gpio_set_lvl_high(PIN_SIO_CLK);
        __delaywdt_us(1);
        gpio_set_lvl_low(PIN_SIO_CLK);
        __delaywdt_us(1);
        //next bit
        mask >>= 1;
    }

	//pulse latch-enable
	gpio_set_lvl_high(PIN_ADF_LE);
	__delaywdt_us(1);
	gpio_set_lvl_low(PIN_ADF_LE);
	__delaywdt_us(50);
}
