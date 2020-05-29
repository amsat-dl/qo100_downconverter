#ifndef __DRIVER_GPIO_H__
#define __DRIVER_GPIO_H__

void init_gpios(void);

//gpio-pin-definition
typedef struct gpio_pin_def {
	volatile unsigned char *pPORT;
    volatile unsigned char *pTRIS;
	u8                      mask;   //bit mask for registers
} gpio_pin_def;

//gpio-pin-reference
typedef const gpio_pin_def *gpio_pin;

//helper macros
#define GPIO_DEFINE_PIN_C(name, port, pin) \
	const gpio_pin_def name##_DEF = {&PORT##port, &TRIS##port, (1<<pin)}; \
	const gpio_pin     name       = &name##_DEF;

#define GPIO_DEFINE_PIN_H(name) \
	extern const gpio_pin     name;

//gpio functions
#define gpio_set_dir_in(pin)   *((pin)->pTRIS) |=  (pin)->mask
#define gpio_set_dir_out(pin)  *((pin)->pTRIS) &= ~(pin)->mask
#define gpio_set_lvl_high(pin) *((pin)->pPORT) |=  (pin)->mask
#define gpio_set_lvl_low(pin)  *((pin)->pPORT) &= ~(pin)->mask
#define gpio_get_input(pin)  ((*((pin)->pPORT) &   (pin)->mask) != 0)


#endif // __DRIVER_GPIO_H__
