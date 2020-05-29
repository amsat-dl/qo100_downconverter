#ifndef __STDLIB_ERRORCODES_H__
#define __STDLIB_ERRORCODES_H__

#define ERROR_FAILED		-1		//general error
#define ERROR_INVAL			-2		//invalid argument
#define ERROR_NODEV			-3		//no such device
#define ERROR_TIMEOUT		-4		//timeout
#define ERROR_BUS			-5		//bus error
#define ERROR_ARBLOST		-6		//arbitration lost
#define ERROR_NACK			-7		//no ACK received
#define ERROR_CRC			-8		//CRC error
#define ERROR_INVALID		-9		//invalid data
#define ERROR_MISMATCH		-10		//rx/tx mismatch
#define ERROR_RANGE			-11		//out of range
#define ERROR_UNKDEV		-12		//unknown device / device not supported
#define ERROR_OVERRUN		-13		//overrun
#define ERROR_PROTOCOL		-14		//communication protocol error
#define ERROR_REQFAILED		-15     //request failed
#define ERROR_BUFTOOSMALL	-16     //buffer too small
#define ERROR_NOTIMPL		-17     //not implemented
#define ERROR_NOTAG			-18		//tag id not found
#define ERROR_INCOMPATIBLE  -19     //device/program is incompatible
#define ERROR_EMPTY			-20     //program is empty
#define ERROR_BUSY			-21		//ressource is busy
#define ERROR_DATAAVAIL		-22     //data is available (CI)

#define ERROR_NOTYETREADY	-100	//tuner is not ready yet (try again later)

#define ERROR_REMOTE		-1000

#endif //__STDLIB_ERRORCODES_H__
