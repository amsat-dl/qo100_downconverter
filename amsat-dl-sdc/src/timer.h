/* 
 * File:   timer.h
 * Author: kurt
 *
 * Created on 18. Oktober 2019, 18:39
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

void init_timer(void);

extern u8 gps_init_timer;
extern u8 cpu_blink_timer;
extern u8 adf4351_set_qrg;


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

