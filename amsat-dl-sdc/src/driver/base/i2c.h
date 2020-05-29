#ifndef __BASE_I2C_H__
#define __BASE_I2C_H__

#include <stdlib.h>

//
//I2C abstraction layer
//

//I2C flags
#define I2C_READ			(1<<0)	//read data
#define I2C_STARTCOND		(1<<1)	//generate start-condition
#define I2C_STOPCOND			(1<<2)	//generate stop-condition
#define I2C_IGNORE_NAK		(1<<3)	//ignore NAK

//I2C structures
typedef struct i2c_msg {
	u16   addr;		//slave address
	u8    flags;	//flags
	u16   len;		//message length
	void* data;		//data buffer
} i2c_msg;

//smbus8 regset
typedef struct {
	u8 reg;
	u8 val;
} smbus8_regset;

//smbus16 regset
typedef struct {
	u16 reg;
	u16 val;
} smbus16_regset;

//dummy-type for I2C-device
DECLARE_HANDLE(i2c_dev);

//I2C driver ops
typedef struct i2c_driver_ops {
	int (*transfer)(i2c_dev dev, i2c_msg *msg, int num);
    int (*setbaud)(i2c_dev dev, int baud);
} i2c_driver_ops;

//i2c operations
extern int i2c_setbaud   (i2c_dev dev, int baud);
extern int i2c_transfer  (i2c_dev dev, i2c_msg *msg, int num);
extern int i2c_probe_addr(i2c_dev dev, u8 addr);
extern int i2c_read_byte (i2c_dev dev, u8 addr, u8 *data);
extern int i2c_write_byte(i2c_dev dev, u8 addr, u8  data);
extern int i2c_read_data (i2c_dev dev, u8 addr, u8 *data, int len);
extern int i2c_write_data(i2c_dev dev, u8 addr, const u8 *data, int len);

//SMBUS wrapper
extern int smbus8_write_byte   (i2c_dev dev, u8 addr, u8 reg, u8  data);
extern int smbus8_write_data     (i2c_dev dev, u8 addr, u8 reg, const u8 *data, int len);
extern int smbus8_read_byte    (i2c_dev dev, u8 addr, u8 reg, u8 *data);
extern int smbus8_read_data      (i2c_dev dev, u8 addr, u8 reg, u8 *data, int len);
extern int smbus8_write_short  (i2c_dev dev, u8 addr, u8 reg, u16 data);
extern int smbus8_read_short   (i2c_dev dev, u8 addr, u8 reg, u16*data);
extern int smbus8_write_regset (i2c_dev dev, u8 addr, const smbus8_regset *regset);
extern int smbus16_write_data    (i2c_dev dev, u8 addr, u16 reg, const u8 *data, int len);
extern int smbus16_write_data_raw(i2c_dev dev, u8 addr, u16 reg, const u8 *data, int len);
extern int smbus16_read_data     (i2c_dev dev, u8 addr, u16 reg, u8 *data, int len);
extern int smbus16_read_data_raw (i2c_dev dev, u8 addr, u16 reg, u8 *data, int len);
extern int smbus16_write_byte  (i2c_dev dev, u8 addr, u16 reg, u8  data);
extern int smbus16_read_byte   (i2c_dev dev, u8 addr, u16 reg, u8 *data);
extern int smbus16_write_short (i2c_dev dev, u8 addr, u16 reg, u16 data);
extern int smbus16_read_short  (i2c_dev dev, u8 addr, u16 reg, u16*data);
extern int smbus16_write_regset(i2c_dev dev, u8 addr, const smbus16_regset *regset);

#endif //__BASE_I2C_H__
