/* 
 * File:   i2c.h
 * Author: be
 *
 * Created on 26. April 2019, 15:00
 */

#ifndef I2C_H
#define	I2C_H

#include <driver/base/i2c.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef struct i2c_data_native {
    i2c_driver_ops	ops;
    
    u32 baud;
} i2c_data_native;


extern void i2c_native_init(i2c_data_native *ctx, u32 baud);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

