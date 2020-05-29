/* 
 * File:   gps.h
 * Author: kurt
 *
 * Created on 4. Oktober 2019, 01:02
 */

#ifndef GPS_H
#define	GPS_H

#ifdef	__cplusplus
extern "C" {
#endif

void eval_gps(char *gpsbuf);
void rx_gps(int c);
void init_gps(void);
u32 getGPSfrequency(void);
char *CalculateGridSquare(void);
void clear_GPS_shift(void);
    
#define GPS_STRLENGTH 15

extern u8 GPS_sat;
extern u8 GPS_lock;
extern u8 GPS_fix;
extern char GPS_time[9];
extern u8 ubx_ack;
extern char GPS_lat[GPS_STRLENGTH];
extern char GPS_lon[GPS_STRLENGTH];

#ifdef	__cplusplus
}
#endif

#endif	/* GPS_H */

