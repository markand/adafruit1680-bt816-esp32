#ifndef PB_SYSCONFIG_H
#define PB_SYSCONFIG_H

#include <driver/gpio.h>

#define PB_SCONF_PIN_CS         GPIO_NUM_@PB_PIN_CS@
#define PB_SCONF_PIN_PD         GPIO_NUM_@PB_PIN_PD@
#define PB_SCONF_PIN_MOSI       GPIO_NUM_@PB_PIN_MOSI@
#define PB_SCONF_PIN_MISO       GPIO_NUM_@PB_PIN_MISO@
#define PB_SCONF_PIN_SCLK       GPIO_NUM_@PB_PIN_SCLK@

#endif /* !PB_SYSCONFIG_H */
