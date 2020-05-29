/* 
 * File:   config.h
 * Author: be
 *
 * Created on 26. April 2019, 10:32
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <mtstd/errorcodes.h>
#include <mtcrt/types.h>
#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#define VERSION_NUMBER  "1.2"

#define I2C_ADDR_LNBH26  0x08
#define I2C_ADDR_SI5328  0x68
#define I2C_ADDR_SSD1306 0x3c
    
//#define GPS     // if not defined: serial output has debug messages
    
#define _XTAL_FREQ 8000000

#define DECLARE_HANDLE(name)						\
            struct name##__ { int unused; }; 		\
            typedef struct name##__ *name

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

