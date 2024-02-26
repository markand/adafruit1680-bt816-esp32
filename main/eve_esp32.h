#ifndef EVE_ESP32_H
#define EVE_ESP32_H

#include <stdint.h>

#include <driver/gpio.h>
#include <driver/spi_master.h>

#if defined(EVE_ESP32)

/**
 * Controls how many devices the system has.
 */
#ifndef EVE_ESP32_DEV_MAX
#       define EVE_ESP32_DEV_MAX 1
#endif

#endif

struct eve_cfg {
	gpio_num_t pin_cs;
	gpio_num_t pin_pd;
	int8_t  spi_mode;
	int64_t spi_clk_speed;
	spi_host_device_t spi_host;
	int16_t queue_size;
};

#endif /* !EVE_ESP32_H */
