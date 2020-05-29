/* 
 * File:   secretsettings.h
 * Author: kurt
 *
 * Created on 3. November 2019, 16:06
 */

#ifndef SECRETSETTINGS_H
#define	SECRETSETTINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

void secret_settings(void);

extern u8 special_cfg;
extern u8 tcxo_no_GPS;

#ifdef	__cplusplus
}
#endif

#endif	/* SECRETSETTINGS_H */

