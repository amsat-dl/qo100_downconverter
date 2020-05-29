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
/* 
 * File:   eeprom.h
 * Author: kurt, DJ0ABR
 *
 * Created on 3. November 2019, 15:22
 * 
 * Flashing the device will erase all EEPROM memory to 255
 */

#include <config.h>
#include <pinmap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <eeprom.h>

//! @brief      Reads a single byte of data from the EEPROM.
//! @param      address     The EEPROM address to write the data to (note that not all
//!                         16-bits of this variable may be supported).
//! @returns    The byte of data read from EEPROM.
//! @warning    This function does not return until read operation is complete.
u8 Eeprom_ReadByte(u8 address)
{

    // Set address registers
    EEADR = address;

    EECON1bits.EEPGD = 0;       // Select EEPROM Data Memory
    EECON1bits.CFGS = 0;        // Access flash/EEPROM NOT config. registers
    EECON1bits.RD = 1;          // Start a read cycle

    // A read should only take one cycle, and then the hardware will clear
    // the RD bit
	u8 to = 0;
    while(EECON1bits.RD == 1)
	{
		if(++to == 255) break;
	}

    return EEDATA;              // Return data

}

//! @brief      Writes a single byte of data to the EEPROM.
//! @param      address     The EEPROM address to write the data to (note that not all
//!                         16-bits of this variable may be supported).
//! @param      data        The data to write to EEPROM.
//! @warning    This function does not return until write operation is complete.
void Eeprom_WriteByte(u8 address, u8 data)
{    
    // Set address registers
    EEADR = address;

    EEDATA = data;          // Write data we want to write to SFR
    EECON1bits.EEPGD = 0;   // Select EEPROM data memory
    EECON1bits.CFGS = 0;    // Access flash/EEPROM NOT config. registers
    EECON1bits.WREN = 1;    // Enable writing of EEPROM (this is disabled again after the write completes)

    // The next three lines of code perform the required operations to
    // initiate a EEPROM write
    EECON2 = 0x55;          // Part of required sequence for write to internal EEPROM
    EECON2 = 0xAA;          // Part of required sequence for write to internal EEPROM
    EECON1bits.WR = 1;      // Part of required sequence for write to internal EEPROM

    // Loop until write operation is complete
	u8 to = 0;
    while(PIR2bits.EEIF == 0)
    {
		__delaywdt_us(10);
        if(++to == 255) break;
    }

    PIR2bits.EEIF = 0;      //Clearing EEIF bit (this MUST be cleared in software after each write)
    EECON1bits.WREN = 0;    // Disable write (for safety, it is re-enabled next time a EEPROM write is performed)
}

/*
 * EEPROM Memory Map:
 * 
 * (we don't use address 0 because it gets corrupted most easily)
 * 
 * Address: 200
 * 
 * special mode number
 * or
 * 255 ... not programed, default, normal operation
 * 
 */

void eeprom_setmode(u8 mode)
{
	Eeprom_WriteByte(EEPROM_CFGADR,mode);
}

u8 eeprom_getmode()
{
	return Eeprom_ReadByte(EEPROM_CFGADR);
}
