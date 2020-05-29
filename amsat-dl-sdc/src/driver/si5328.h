#ifndef __SI5328_H__
#define __SI5328_H__

#include <driver/base/i2c.h>

extern int si5328_init(i2c_dev i2c, u8 addr);
extern void si5328_set_freq(i2c_dev i2c, u8 addr, u8 freq_MHz);
extern u8 check_si5328_status(i2c_dev i2c, u8 addr);
s32 calc_mixer_corr_value(void);
void si5328_checkGPS(i2c_dev i2c, u8 addr);

extern u8 currentLO;
extern u8 actualADFswitch;
extern u8 si5328locked;
extern u8 ext10MHzReference;
extern u8 use_GPS;

#endif //__SI5328_H__
