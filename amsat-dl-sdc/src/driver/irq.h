#ifndef __DRIVER_IRQ_H__
#define __DRIVER_IRQ_H__

typedef enum
{
    IRQ_HANDLER_ID_USART = 0,
    IRQ_HANDLER_ID_TIMER0,
    IRQ_HANDLER_ID_CNT
} IRQ_HANDLER_ID;

typedef void(*irq_handler_func)(void *udata);

extern void irq_set_handler(IRQ_HANDLER_ID id, irq_handler_func handler, void *udata);

#endif //__DRIVER_IRQ_H__
 
