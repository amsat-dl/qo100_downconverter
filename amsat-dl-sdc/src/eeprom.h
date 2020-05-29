/* 
 * File:   eeprom.h
 * Author: kurt
 *
 * Created on 3. November 2019, 15:22
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define EEPROM_CFGADR  200

    
u8 Eeprom_ReadByte(u8 address);
void Eeprom_WriteByte(u8 address, u8 data);
void eeprom_setmode(u8 mode);
u8 eeprom_getmode(void);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

