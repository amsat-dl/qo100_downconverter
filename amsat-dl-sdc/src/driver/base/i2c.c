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
#include "i2c.h"

//
//I2C operations
//
/*
int i2c_setbaud(i2c_dev dev, int baud)
{
	if(!dev) return ERROR_NODEV;
	if(!((i2c_driver_ops*)dev)->setbaud) return 0;
	return ((i2c_driver_ops*)dev)->setbaud(dev, baud);
}
*/
int i2c_transfer(i2c_dev dev, i2c_msg *msg, int num)
{
	if(!dev) return ERROR_NODEV;
	return ((i2c_driver_ops*)dev)->transfer(dev, msg, num);
}

int i2c_probe_addr(i2c_dev dev, u8 addr)
{
	i2c_msg msg;
	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 0;
	msg.data  = NULL;
	return i2c_transfer(dev, &msg, 1);
}
/*
int i2c_read_byte(i2c_dev dev, u8 addr, u8 *data)
{
	return i2c_read_data(dev, addr, data, 1);
}

int i2c_write_byte(i2c_dev dev, u8 addr, u8 data)
{
	return i2c_write_data(dev, addr, &data, 1);
}

int i2c_read_data(i2c_dev dev, u8 addr, u8 *data, int len)
{
	i2c_msg msg;
	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
	msg.len   = len;
	msg.data  = data;
	return i2c_transfer(dev, &msg, 1);
}

int i2c_write_data(i2c_dev dev, u8 addr, const u8 *data, int len)
{
	i2c_msg msg;
	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = len;
	msg.data  = (u8*)data;
	return i2c_transfer(dev, &msg, 1);
}
*/

//
//SMBUS wrapper
//
int smbus8_write_byte(i2c_dev dev, u8 addr, u8 reg, u8  data)
{
	i2c_msg msg;
	u8 tmp[2];

	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 2;
	msg.data  = tmp;

	tmp[0] = reg;
	tmp[1] = data;

	return i2c_transfer(dev, &msg, 1);
}

int smbus8_write_data(i2c_dev dev, u8 addr, u8 reg, const u8* data, int len)
{
	i2c_msg msg[2];

	msg[0].addr  = addr;
	msg[0].flags = I2C_STARTCOND;
	msg[0].len   = 1;
	msg[0].data  = &reg;

	msg[1].addr  = addr;
	msg[1].flags = I2C_STOPCOND;
	msg[1].len   = len;
	msg[1].data  = (u8*)data;

	return i2c_transfer(dev, msg, 2);
}
/*
int smbus8_write_short(i2c_dev dev, u8 addr, u8 reg, u16 data)
{
	i2c_msg msg;
	u8 tmp[4];

	// Alignment

	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 3;
	msg.data  = tmp + 1;

	tmp[1] = reg;
	* (u16 *) (&tmp[2]) = data;

	return i2c_transfer(dev, &msg, 1);
}
*/
int smbus8_read_byte(i2c_dev dev, u8 addr, u8 reg, u8 *data)
{
	i2c_msg msg[2];

	msg[0].addr  = addr;
	msg[0].flags = I2C_STARTCOND;
	msg[0].len   = 1;
	msg[0].data  = &reg;

	msg[1].addr  = addr;
	msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
	msg[1].len   = 1;
	msg[1].data  = data;

	return i2c_transfer(dev, msg, 2);
}
/*
int smbus8_read_data(i2c_dev dev, u8 addr, u8 reg, u8 *data, int len)
{
	i2c_msg msg[2];

	msg[0].addr  = addr;
	msg[0].flags = I2C_STARTCOND;
	msg[0].len   = 1;
	msg[0].data  = &reg;

	msg[1].addr  = addr;
	msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
	msg[1].len   = len;
	msg[1].data  = data;

	return i2c_transfer(dev, msg, 2);
}

int smbus8_read_short(i2c_dev dev, u8 addr, u8 reg, u16*data)
{
	i2c_msg msg[2];

	msg[0].addr  = addr;
	msg[0].flags = I2C_STARTCOND;
	msg[0].len   = 1;
	msg[0].data  = &reg;

	msg[1].addr  = addr;
	msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
	msg[1].len   = 2;
	msg[1].data  = data;

	return i2c_transfer(dev, msg, 2);
}


int smbus8_write_regset(i2c_dev dev, u8 addr, const smbus8_regset *regset)
{
	i2c_msg msg;
	u8 tmp[2];

	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 2;
	msg.data  = tmp;

	while ((regset->reg != 0xFF) || (regset->val != 0xFF)) {
		int res;

		tmp[0] = regset->reg;
		tmp[1] = regset->val;
		regset++;

		if (0 != (res = i2c_transfer(dev, &msg, 1)))
			return res;

	}

	return 0;
}

int smbus16_write_data(i2c_dev dev, u8 addr, u16 reg, const u8 *data, int len)
{
        i2c_msg msg[2];
        u8 ger[2];

        ger[0] = (reg>>8) & 0x00FF;
        ger[1] = reg & 0x00FF;

        msg[0].addr  = addr;
        msg[0].flags = I2C_STARTCOND;
        msg[0].len   = 2;
        msg[0].data  = ger;

        msg[1].addr  = addr;
        msg[1].flags = I2C_STOPCOND;
        msg[1].len   = len;
        msg[1].data  = (u8*)data;

        return i2c_transfer(dev, msg, 2);
}

int smbus16_write_data_raw(i2c_dev dev, u8 addr, u16 reg, const u8 *data, int len)
{
        i2c_msg msg[2];
        u8 ger[2];

        ger[0] = (reg>>8) & 0x00FF;
        ger[1] = reg & 0x00FF;

        msg[0].addr  = addr;
        msg[0].flags = I2C_STARTCOND;
        msg[0].len   = 2;
        msg[0].data  = ger;

        msg[1].addr  = addr;
        msg[1].flags = I2C_STOPCOND;
        msg[1].len   = len;
        msg[1].data  = (u8*)data;

        return i2c_transfer(dev, msg, 2);
}

int smbus16_write_byte(i2c_dev dev, u8 addr, u16 reg, u8 data)
{
	i2c_msg msg;
	u8 tmp[3];

	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 3;
	msg.data  = tmp;

	tmp[0] = (reg>>8) & 0xFF;
	tmp[1] = (reg>>0) & 0xFF;
	tmp[2] = data;

	return i2c_transfer(dev, &msg, 1);
}

int smbus16_read_byte(i2c_dev dev, u8 addr, u16 reg, u8 *data)
{
	i2c_msg msg[2];

	msg[0].addr  = addr;
	msg[0].flags = I2C_STARTCOND;
	msg[0].len   = 2;
	msg[0].data  = &reg;

	msg[1].addr  = addr;
	msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
	msg[1].len   = 1;
	msg[1].data  = data;

	return i2c_transfer(dev, msg, 2);
}

int smbus16_read_data(i2c_dev dev, u8 addr, u16 reg, u8 *data, int len)
{
        i2c_msg msg[2];
        u8 ger[2];

        ger[0] = (reg>>8) & 0x00FF;
        ger[1] = reg & 0x00FF;

        msg[0].addr  = addr;
        msg[0].flags = I2C_STARTCOND | I2C_STOPCOND;
        msg[0].len   = 2;
        msg[0].data  = ger;

        msg[1].addr  = addr;
        msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
        msg[1].len   = len;
        msg[1].data  = data;

        return i2c_transfer(dev, msg, 2);
}

int smbus16_read_data_raw(i2c_dev dev, u8 addr, u16 reg, u8 *data, int len)
{
        i2c_msg msg[2];
        u8 ger[2];

        ger[0] = (reg>>8) & 0x00FF;
        ger[1] = reg & 0x00FF;

        msg[0].addr  = addr;
        msg[0].flags = I2C_STARTCOND;
        msg[0].len   = 2;
        msg[0].data  = ger;

        msg[1].addr  = addr;
        msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
        msg[1].len   = len;
        msg[1].data  = data;

        return i2c_transfer(dev, msg, 2);
}



int smbus16_write_short(i2c_dev dev, u8 addr, u16 reg, u16 data)
{
	i2c_msg msg;
    u8 tmp[3];

	//

	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 4;
	msg.data  = tmp;

	* (u16 *) (&tmp[0]) = reg;
	* (u16 *) (&tmp[2]) = data;

	return i2c_transfer(dev, &msg, 1);
}

int smbus16_read_short(i2c_dev dev, u8 addr, u16 reg, u16*data)
{
	i2c_msg msg[2];

	msg[0].addr  = addr;
	msg[0].flags = I2C_STARTCOND;
	msg[0].len   = 2;
	msg[0].data  = &reg;

	msg[1].addr  = addr;
	msg[1].flags = I2C_STARTCOND | I2C_STOPCOND | I2C_READ;
	msg[1].len   = 2;
	msg[1].data  = data;

	return i2c_transfer(dev, msg, 2);
}

int smbus16_write_regset(i2c_dev dev, u8 addr, const smbus16_regset *regset)
{
	i2c_msg msg;
	u8 tmp[4];

	msg.addr  = addr;
	msg.flags = I2C_STARTCOND | I2C_STOPCOND;
	msg.len   = 4;
	msg.data  = tmp;

	while ((regset->reg != 0xFFFF) || (regset->val != 0xFFFF)) {
		int res;

		* (u16 *) (&tmp[0]) = regset->reg;
		* (u16 *) (&tmp[2]) = regset->val;
		regset++;

		if (0 != (res = i2c_transfer(dev, &msg, 1)))
			return res;

	}

	return 0;
}
*/