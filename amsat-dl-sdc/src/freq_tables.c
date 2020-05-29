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
#include <pinmap.h>
#include <stdio.h>
#include <string.h>


// ADF4351 frequencies for selected output frequency in kHz and each LNB LO qrg
const u32 freq_table_25_24[10] = {
	// if a 25 MHz LNB is driven with 24 MHz
    1126000, //3,55
	1108000, //21,55
	1101000, //28,55
	1100000, //29,55
	1079000, //50,55
	1059000, //70,55
	985000,  //144,55
	984000,  //145,55
	694000,  //435,55
	690000,  //439,55
};

const u32 freq_table_25_25[10] = {
	// if a 25 MHz LNB is driven with 25 MHz
    736000, //	3,55
	718000, //  21,55
	711000, //	28,55
	710000, //	29,55
	689000, //	50,55 (distortions !!!)
	669000, //	70,55
	595000, //	144,55
	594000, //	145,55
	304000, //	435,55
	300000, //	439,55
};

const u32 freq_table_27_26[10] = {
	// if a 27 MHz LNB is driven with 26 MHz
    1097111, //3,55
	1079111, //21,55
	1072111, //28,55,
	1071111, //29,55,
	1050111, //50,55,
	1030111, //70,55,
	956111, //144,55,
	955111, //145,55,
	665111, //435,55,
	661111, //439,55,
};

const u32 freq_table_27_OPT[10] = {
	// if a 27 MHz LNB is driven with 25,454545 MHz
    1294080, //3,55
	1276080, //21,55
	1269080, //28,55
	1268080, //29,55
	1247080, //50,55
	1227080, //70,55
	1153080, //144,55
	1152080, //145,55
	862080, //435,55
	858080, //439,55
};
