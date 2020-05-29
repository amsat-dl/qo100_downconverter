/* 
 * File:   lnb.h
 * Author: be
 *
 * Created on 29. April 2019, 12:32
 */

#ifndef LNB_H
#define	LNB_H

#include <driver/base/i2c.h>

#ifdef	__cplusplus
extern "C" {
#endif

extern void lnb_configure(i2c_dev i2c, u8 addr);
extern u8 check_LNB_status(i2c_dev i2c, u8 addr);

extern u8 LNB_laststat;
extern u8 LNB_error;

#ifdef	__cplusplus
}
#endif

#endif	/* LNB_H */

