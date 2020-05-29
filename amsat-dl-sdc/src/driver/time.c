#include <mtcrt/types.h>
#include <pic18f2520.h>

#include "time.h"

//
//basic timing
//

//high-precision-timer
static u16 g_nHPTlast;
static u32 g_nHPTacc;

void time_init(void)
{
	//init high-precison-timer (= free-run-timer #0)
    T0CONbits.TMR0ON = 0; //disable timer
    
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 1;
    T0CONbits.T0PS = 2; //1:8
    TMR0H = 0;
    TMR0L = 0;
    
    T0CONbits.TMR0ON = 1; //enable timer
}


//
//high-precison-timer & udelay
//
static inline u16 readTimerValue()
{
    u16 low = TMR0L;
    u16 high = TMR0H;

    return (low|(high<<8));
}

static inline u32 usec2hpt(u32 usec)
{
    return ((usec*100LLU)+50LLU)/63;
}

void hpt_reset(void)
{
	//reset timer
	g_nHPTlast = readTimerValue();
	g_nHPTacc  = 0;
}

u32 hpt_get_value(void)
{
    u16 now = readTimerValue();

	//update raw-value
	if (now >= g_nHPTlast) {
		//normal mode
		g_nHPTacc += now - g_nHPTlast;
	} else {
		//we have an overflow ...
		g_nHPTacc += now + (0xFFFF - g_nHPTlast);
	}
	g_nHPTlast = now;

	//return usec-value
	return g_nHPTacc;
}

void udelay(u32 usec)
{
    u32 hpt;

	//reset timer
	hpt_reset();

	//apply correction-factor
	hpt = usec2hpt(usec);

	//do wait
	while(hpt_get_value() < hpt);
}
