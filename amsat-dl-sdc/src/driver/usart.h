#ifndef __DRIVER_USART_H__
#define __DRIVER_USART_H__

//initialize usart-port
extern void usart_init(u32 baud);
//write char to usart-port
extern char usart_putc(char c);
//read char from usart-port
extern int usart_getc(void);

void rx_usart(void);
void putgps(u8 c);


#endif //__DRIVER_USART_H__
