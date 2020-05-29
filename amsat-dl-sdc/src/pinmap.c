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
#include <config.h>
#include "pinmap.h"

// ADF4351 connections
GPIO_DEFINE_PIN_C(PIN_ADF_LOCK,     A, 0)
GPIO_DEFINE_PIN_C(PIN_ADF_CE,       A, 1)
GPIO_DEFINE_PIN_C(PIN_ADF_LE,       A, 2)
GPIO_DEFINE_PIN_C(PIN_SIO_DATA,     A, 3)
GPIO_DEFINE_PIN_C(PIN_SIO_CLK,      A, 4)
	
// si5328 connections
// additionally uses SCA and SCL pins for I2C interface
GPIO_DEFINE_PIN_C(PIN_SI5328_LOL,   A, 5);	// PLL Loss Of Lock
GPIO_DEFINE_PIN_C(PIN_SI5328_CS_CA,    A, 7)

// LEDs	
GPIO_DEFINE_PIN_C(PIN_LED_SI_LOCK,   A, 6)
GPIO_DEFINE_PIN_C(PIN_LED3,         A, 6)
GPIO_DEFINE_PIN_C(PIN_LED_CPU_OK,   C, 0)
GPIO_DEFINE_PIN_C(PIN_LED0,         C, 0)
GPIO_DEFINE_PIN_C(PIN_LED_ADF_LOCK, C, 1)
GPIO_DEFINE_PIN_C(PIN_LED1,         C, 1)
GPIO_DEFINE_PIN_C(PIN_LED_LNB_GOOD, C, 2)
GPIO_DEFINE_PIN_C(PIN_LED2,         C, 2)

// I2C
GPIO_DEFINE_PIN_C(PIN_I2C_SCL,      C, 3)
GPIO_DEFINE_PIN_C(PIN_I2C_SDA,      C, 4)

// UART for GPS Receiver
GPIO_DEFINE_PIN_C(PIN_USART_TX,     C, 6)
GPIO_DEFINE_PIN_C(PIN_USART_RX,     C, 7)

// channel switch
GPIO_DEFINE_PIN_C(PIN_SJ3,          B, 1)
GPIO_DEFINE_PIN_C(PIN_SJ4,          B, 2)
GPIO_DEFINE_PIN_C(PIN_SJ5,          B, 3)
GPIO_DEFINE_PIN_C(PIN_SJ6,          B, 4)

// LNB selection jumpers
GPIO_DEFINE_PIN_C(PIN_LNB_QRG2,     B, 0)
GPIO_DEFINE_PIN_C(PIN_LNB_QRG1,     B, 5)

// LNBH26PQR LNB-Voltage Controller
// additionally uses SCA and SCL pins for I2C interface
GPIO_DEFINE_PIN_C(PIN_LNB_FAULT,    C, 5)
