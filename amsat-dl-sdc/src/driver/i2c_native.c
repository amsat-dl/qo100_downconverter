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
 * File:   i2c.c
 * Author: be
 *
 * Created on 26. April 2019, 15:00
 */
#include <config.h>
#include <string.h>
#include <stdio.h>
#include "i2c_native.h"

//config
#define I2C_TIMEOUT_INT		 2500 //us
#define I2C_TIMEOUT_BUSFREE	10000 //us

//prototypes
static void i2c_reset(i2c_data_native *ctx);
static void i2c_do_setbaud(i2c_data_native *ctx, int baud);
static int  i2c_wait_busfree(u32 timeout);
static int  i2c_wait_int(u32 timeout);
static int  i2c_start(u8 addr, BOOL read, BOOL rep, BOOL wantack);
static void i2c_stop(i2c_data_native *ctx);
static int  i2c_read(u8 *data, BOOL ack);
static int  i2c_write(u8 data, BOOL wantack);
static int  i2c_transfer_native(i2c_dev dev, i2c_msg *msg, int num);

void i2c_native_init(i2c_data_native *ctx, u32 baud)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->baud = baud;

	//reset status
	i2c_reset(ctx);

	//return filled device-data
	ctx->ops.transfer = i2c_transfer_native;
}

#define I2C_MASTER_MODE 0x08
#define I2C_FW_CTRL_MASTER_MODE 0x0B

static void i2c_reset(i2c_data_native *ctx)
{
    u32 reg;

    SSPCON1bits.SSPEN = 0;  //disable MSSP
    SSPCON1bits.SSPM = I2C_MASTER_MODE; //I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    
    SSPSTATbits.SMP = 1; //standard speed mode
    SSPSTATbits.CKE = 0; //disable SMBus specific inputs
	
	TRISC |= 0b00011000;	// Make SCL & SDA to be inputs
    
    //I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    reg = (_XTAL_FREQ / (4*ctx->baud)) - 1;
    SSPADD = (reg<127)?reg:127;
    
    SSPCON1bits.SSPEN = 1;  //enable MSSP
}

//wait until the bus is free
/*static int i2c_wait_busfree(u32 timeout)
{
	//check if i2c is enabled
	if(SSPCON1bits.SSPEN==0) return 0;

	//wait until bus is free
	while((PORTCbits.RC3 != 1) || (PORTCbits.RC4 != 1)) {
		__delaywdt_us(1);
		if(--timeout == 0) {
			//-> timeout
			return ERROR_TIMEOUT;
		}
	}

	//-> everything ok
	return 0;
}*/

//wait until current operation has finshed
static int i2c_wait_int(u32 timeout)
{
	//wait for interrupt-flag
	while((PIR1bits.SSPIF == 0) && (SSPCON1bits.WCOL == 0)) {
		__delaywdt_us(1);
		if(--timeout == 0) {
			//-> timeout
			return ERROR_TIMEOUT;
		}
	}

	//check for bus-error
	if(SSPCON1bits.WCOL != 0) {
		//-> bus error
		SSPCON1bits.WCOL = 0;
		return ERROR_BUS;
	}

	//-> everything ok
	return 0;
}

//create start-condition
static int i2c_start(u8 addr, BOOL read, BOOL rep, BOOL wantack)
{
    int res;

    PIR1bits.SSPIF = 0;
	//generate (repeated) start-codition
	if(rep) SSPCON2bits.RSEN = 1;
	else	SSPCON2bits.SEN = 1;

	//wait start condition is done
    res = i2c_wait_int(I2C_TIMEOUT_INT);
	if(res) return res;

    PIR1bits.SSPIF = 0;
    //write device-adddres & direction-bit
	if(read) SSPBUF = (addr<<1) | 1;
	else	 SSPBUF = (addr<<1) | 0;
    
    //wait until byte is written
    res = i2c_wait_int(I2C_TIMEOUT_INT);
	if(res) return res;

	//check if byte was acked
	if(wantack && (SSPCON2bits.ACKSTAT != 0)) {
		return ERROR_NACK;
	}

	//-> everything ok
	return 0;
}

//create stop-condition
static void i2c_stop(i2c_data_native *ctx)
{
    PIR1bits.SSPIF = 0;
	//generate stop-codition
	SSPCON2bits.PEN = 1;

	//wait until stop-codition is generated
	if(i2c_wait_int(I2C_TIMEOUT_BUSFREE)) {
		//error -> reset bus
		i2c_reset(ctx);
	}
}

//read byte
static int i2c_read(u8 *data, BOOL ack)
{
	int res;

    PIR1bits.SSPIF = 0;

	//start reading byte
	SSPCON2bits.RCEN = 1;

	//wait until byte is read
	res = i2c_wait_int(I2C_TIMEOUT_INT);
	if(res) return res;
    
    if(SSPCON2bits.RCEN != 0 || SSPSTATbits.BF == 0) {
        //printf("bla\n");
    }
    //save byte
	*data = SSPBUF;
    
    PIR1bits.SSPIF = 0;
    //send ack
    SSPCON2bits.ACKDT = (ack)?0:1;
    SSPCON2bits.ACKEN = 1;
    res = i2c_wait_int(I2C_TIMEOUT_INT);
    if(res) return res;

    return 0;
}

//write byte
static int i2c_write(u8 data, BOOL wantack)
{
	int res;

    PIR1bits.SSPIF = 0;

	//write byte
	SSPBUF = data;

	//wait until byte is written
	res = i2c_wait_int(I2C_TIMEOUT_INT);
	if(res) return res;

	//check if byte was acked
    if(wantack && (SSPCON2bits.ACKSTAT != 0)) {
		return ERROR_NACK;
	}

	//-> everything ok
	return 0;
}

//do transfer
static int i2c_transfer_native(i2c_dev dev, i2c_msg *msg, int num)
{
	i2c_data_native *ctx = (i2c_data_native*)dev;
	struct i2c_msg *pmsg;
	BOOL read, ack, wantack, inmsg;
	int  i, res;
	u16  left;
	u8*  bdata;

	inmsg = 0;
	for(i=0;i<num;i++) {
		//get current message
		pmsg    = &msg[i];
		bdata	= (u8*)pmsg->data;
		read    = ((pmsg->flags & I2C_READ) != 0);
		wantack = ((pmsg->flags & I2C_IGNORE_NAK) == 0);

		//generate start-condition
		if(pmsg->flags&I2C_STARTCOND) {
			res = i2c_start(pmsg->addr, read, inmsg, wantack);
			if(res) goto error;
			inmsg = 1;
		}

		if(read) {
			//read data
			left = pmsg->len;
			while(left--) {
				ack = (left!=0);
				res = i2c_read(bdata++, ack);
				if(res) goto error;
			}
		} else {
			//write data
			left = pmsg->len;
			while(left--) {
				res = i2c_write(*bdata++, wantack);
				if(res) goto error;
			}
		}

		//generate stop-condition
		if(pmsg->flags&I2C_STOPCOND) {
			i2c_stop(ctx);
			inmsg = 0;
		}
	}
	return 0;

error:
	if(res!=ERROR_BUS && res!=ERROR_ARBLOST) i2c_stop(ctx);
	i2c_reset(ctx);
	return res;
}
