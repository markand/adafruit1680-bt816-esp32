#if defined(EVE_ESP32)

#include <esp_err.h>
#include <esp_log.h>

#include <endian.h>

#include "eve.h"
#include "eve_esp32.h"

#define TAG       "eve"
#define DEVC(s)   ((struct devc *)(s))

#define ACQUIRE(devc) do {                                              \
	gpio_set_level((devc)->pin_cs, 0);                              \
	spi_device_acquire_bus((devc)->handle, portMAX_DELAY);          \
} while (0)

#define RELEASE(devc) do {                                              \
	spi_device_release_bus((devc)->handle);                         \
	gpio_set_level((devc)->pin_cs, 1);                              \
} while (0)

/*
 * The automatic spics_io_num does not seem to work all the time so create our
 * own interface but for that we need to keep track of the CS pin ourselves as
 * we need to set spics_io_num to -1 to indicate IDF to not touch it.
 */
struct devc {
	spi_device_handle_t handle;
	gpio_num_t pin_cs;
	gpio_num_t pin_pd;
};

static struct devc devices[EVE_ESP32_DEV_MAX];

int
eve__open(struct devc *devc, const struct eve_cfg *cfg)
{
	esp_err_t err;
	spi_device_interface_config_t devc_cfg = {};
	gpio_config_t gpio_cfg = {};

	devc_cfg.mode           = cfg->spi_mode;
	devc_cfg.clock_speed_hz = cfg->spi_clk_speed;
	devc_cfg.spics_io_num   = -1;
	devc_cfg.queue_size     = cfg->queue_size;

	gpio_cfg.pin_bit_mask   = BIT64(cfg->pin_cs) | BIT64(cfg->pin_pd);
	gpio_cfg.mode           = GPIO_MODE_OUTPUT;
	gpio_cfg.intr_type      = GPIO_INTR_DISABLE;

	ESP_LOGD(TAG, "opening device");
	ESP_LOGD(TAG, "  - SPI mode:       %d", (int)cfg->spi_mode);
	ESP_LOGD(TAG, "  - SPI speed:      %d", (int)cfg->spi_clk_speed);
	ESP_LOGD(TAG, "  - SPI queue size: %d", (int)cfg->queue_size);
	ESP_LOGD(TAG, "  - CS pin:         %d", (int)cfg->pin_cs);
	ESP_LOGD(TAG, "  - PD pin:         %d", (int)cfg->pin_pd);

	if ((err = gpio_config(&gpio_cfg)) != ESP_OK) {
		ESP_LOGW(TAG, "unable to setup GPIO pins: %s", esp_err_to_name(err));
		return -1;
	}

	devc->pin_cs = cfg->pin_cs;
	devc->pin_pd = cfg->pin_pd;

	gpio_set_level(devc->pin_pd, 0);
	gpio_set_level(devc->pin_cs, 1);

	if ((err = spi_bus_add_device(cfg->spi_host, &devc_cfg, &devc->handle)) != ESP_OK) {
		ESP_LOGW(TAG, "unable to add device: %s", esp_err_to_name(err));
		return -1;
	}

	return 0;
}

static int
eve__read(struct devc *devc, uint32_t address, void *value, int n)
{
	esp_err_t err;
	spi_transaction_t tx = {}, rx = {};

	/* address write transaction. */
	tx.length     = 32;
	tx.flags      = SPI_TRANS_USE_TXDATA;
	tx.tx_data[0] = (address >> 16) & 0xff;
	tx.tx_data[1] = (address >>  8) & 0xff;
	tx.tx_data[2] = (address >>  0) & 0xff;

	/* read transaction result. */
	rx.length     = n * 8;
	rx.rx_buffer  = value;

	ACQUIRE(devc);

	if ((err = spi_device_polling_transmit(devc->handle, &tx)) == ESP_OK)
		err = spi_device_polling_transmit(devc->handle, &rx);
	if (err != ESP_OK)
		ESP_LOGW(TAG, "host memory read transaction error: %s", esp_err_to_name(err));

	RELEASE(devc);

	return err == ESP_OK ? 0 : -1;
}

static int
eve__write(struct devc *devc, uint32_t address, const void *value, int n)
{
	esp_err_t err;
	spi_transaction_t txaddr = {}, txdata = {};

	/* address write transaction. */
	txaddr.length     = 24;
	txaddr.flags      = SPI_TRANS_USE_TXDATA;
	txaddr.tx_data[0] = ((address >> 16) & 0xff) | 0x80;
	txaddr.tx_data[1] = ((address >>  8) & 0xff);
	txaddr.tx_data[2] = ((address >>  0) & 0xff);

	/* write transaction data. */
	txdata.length     = n * 8;
	txdata.tx_buffer  = value;

	ACQUIRE(devc);

	if ((err = spi_device_polling_transmit(devc->handle, &txaddr)) == ESP_OK)
		err = spi_device_polling_transmit(devc->handle, &txdata);
	if (err != ESP_OK)
		ESP_LOGW(TAG, "host memory write transaction error: %s", esp_err_to_name(err));

	RELEASE(devc);

	return err == ESP_OK ? 0 : -1;
}

intptr_t
eve_init(const struct eve_cfg *cfg)
{
	assert(cfg);

	struct devc *devc = NULL;

	for (int i = 0; i < EVE_ESP32_DEV_MAX; ++i) {
		if (devices[i].handle == NULL) {
			devc = &devices[i];
			break;
		}
	}

	if (!devc) {
		ESP_LOGW(TAG, "no more devices available");
		return -1;
	}
	if (eve__open(devc, cfg) < 0)
		return -1;

	return (intptr_t)devc;
}

int
eve_power(intptr_t devc, int enable)
{
	gpio_set_level(DEVC(devc)->pin_pd, enable);

	return 0;
}

/*
 * As of 5.1.2 the only error is bad GPIO pin so we expect user to be
 * smart enough to choose correct one.
 */

int
eve_cmd(intptr_t devc, uint8_t cmd, uint8_t param)
{
	esp_err_t err;
	spi_transaction_t tx = {};

	tx.length     = 24;
	tx.tx_data[0] = cmd;
	tx.tx_data[1] = param;
	tx.flags      = SPI_TRANS_USE_TXDATA;

	gpio_set_level(DEVC(devc)->pin_cs, 0);

	if ((err = spi_device_polling_transmit(DEVC(devc)->handle, &tx)) != ESP_OK)
		ESP_LOGW(TAG, "command failed: %s", esp_err_to_name(err));

	gpio_set_level(DEVC(devc)->pin_cs, 1);

	return err == ESP_OK ? 0 : -1;
}

int
eve_read8(intptr_t devc, uint32_t address, uint8_t *value)
{
	return eve__read(DEVC(devc), address, value, 1);
}

int
eve_read16(intptr_t devc, uint32_t address, uint16_t *value)
{
	return eve__read(DEVC(devc), address, value, 2);
}

int
eve_read32(intptr_t devc, uint32_t address, uint32_t *value)
{
	assert(value);

	return eve__read(DEVC(devc), address, value, 4);
}

int
eve_write8(intptr_t devc, uint32_t address, uint8_t value)
{
	return eve__write(DEVC(devc), address, &value, 1);
}

int
eve_write16(intptr_t devc, uint32_t address, uint16_t value)
{
	return eve__write(DEVC(devc), address, &value, 2);
}

int
eve_write32(intptr_t devc, uint32_t address, uint32_t value)
{
	return eve__write(DEVC(devc), address, &value, 4);
}

void
eve_finish(intptr_t devc)
{
	struct devc *self = DEVC(devc);

	if (!devc)
		return;

	gpio_set_level(self->pin_pd, 0);
	gpio_set_level(self->pin_cs, 1);

	spi_bus_remove_device(self->handle);

	self->handle = NULL;
	self->pin_cs = 0;
	self->pin_pd = 0;
}

#endif /* !EVE_ESP32 */
