#include <assert.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <esp_err.h>

#include <driver/spi_master.h>

#include "eve.h"
#include "eve_esp32.h"
#include "sysconfig.h"

#define TAG                     "main"

#define PB_SPI_CLOCK_SPEED      10000000
#define PB_SPI_QUEUE_SIZE       4
#define PB_SPI_XFER_SIZE_MAX    128
#define PB_SPI_HOST             SPI2_HOST

/* Verify all that stuff? */
#define PB_LCD_60MHZ            0x3938700
#define PB_LCD_HSIZE            800
#define PB_LCD_VSIZE            480
#define PB_LCD_VSYNC0           0
#define PB_LCD_VSYNC1           3
#define PB_LCD_VOFFSET          32
#define PB_LCD_VCYCLE           525
#define PB_LCD_HSYNC0           0
#define PB_LCD_HSYNC1           48
#define PB_LCD_HOFFSET          88
#define PB_LCD_HCYCLE           928
#define PB_LCD_PCLKPOL          0
#define PB_LCD_PCLK             2

static struct {
	intptr_t lcd;
} pb;

static void
init_logs(void)
{
	esp_log_level_set("eve", ESP_LOG_DEBUG);
	esp_log_level_set("main", ESP_LOG_DEBUG);
	ESP_LOGI(TAG, "booting up " PB_VERSION);
}

/* SPI bus. */

static void
init_spi(void)
{
	esp_err_t err;
	spi_bus_config_t bus_conf = {};

	bus_conf.mosi_io_num     = PB_SCONF_PIN_MOSI;
	bus_conf.miso_io_num     = PB_SCONF_PIN_MISO;
	bus_conf.sclk_io_num     = PB_SCONF_PIN_SCLK;
	bus_conf.quadwp_io_num   = -1;
	bus_conf.quadhd_io_num   = -1;
	bus_conf.max_transfer_sz = PB_SPI_XFER_SIZE_MAX;

	/* TODO: check DMA. */
	ESP_LOGI(TAG, "initializing SPI bus");
	err = spi_bus_initialize(PB_SPI_HOST, &bus_conf, SPI_DMA_DISABLED);
	ESP_ERROR_CHECK(err);
}

static void
init_lcd_spi(void)
{
	struct eve_cfg cfg = {};
	uint8_t id = 0xff;
	uint16_t status = 0xffff;

	ESP_LOGI(TAG, "initializing LCD over SPI");

	cfg.pin_cs        = PB_SCONF_PIN_CS;
	cfg.pin_pd        = PB_SCONF_PIN_PD;
	cfg.spi_clk_speed = PB_SPI_CLOCK_SPEED;
	cfg.spi_host      = PB_SPI_HOST;
	cfg.queue_size    = PB_SPI_QUEUE_SIZE;

	/*
	 * This function will initialize appropriates GPIO as output and turn
	 * off the device.
	 */
	pb.lcd = eve_init(&cfg);
	assert(pb.lcd);

	/* Turn back on. */
	vTaskDelay(pdMS_TO_TICKS(50));
	eve_power(pb.lcd, 1);
	vTaskDelay(pdMS_TO_TICKS(50));

	/* Now activate core. */
	eve_cmd(pb.lcd, EVE_CMD_RST_PULSE, 0);
	eve_cmd(pb.lcd, EVE_CMD_ACTIVE, 0);

	/* Wait until it's ready. */
	while (id != 0x7c) {
		eve_read8(pb.lcd, EVE_REG_ID, &id);
		vTaskDelay(pdMS_TO_TICKS(500));
	}

	ESP_LOGI(TAG, "LCD ready");

	/* Check reset status. */
	while (status != 0x00) {
		eve_read16(pb.lcd, EVE_REG_CPURESET, &status);
		vTaskDelay(pdMS_TO_TICKS(25));
	}

	ESP_LOGI(TAG, "LCD CPU state ready");

	/* Make sure to operate at 60Mhz. */
	eve_write32(pb.lcd, EVE_REG_FREQUENCY, PB_LCD_60MHZ);
}

static void
init_lcd_specs(void)
{

	/*
	 * Adafruit 1680:
	 *
	 * - 800x480
	 */
	eve_write16(pb.lcd, EVE_REG_HSIZE,    PB_LCD_HSIZE);
	eve_write16(pb.lcd, EVE_REG_HCYCLE,   PB_LCD_HCYCLE);
	eve_write16(pb.lcd, EVE_REG_HOFFSET,  PB_LCD_HOFFSET);
	eve_write16(pb.lcd, EVE_REG_HSYNC0,   PB_LCD_HSYNC0);
	eve_write16(pb.lcd, EVE_REG_HSYNC1,   PB_LCD_HSYNC1);
	eve_write16(pb.lcd, EVE_REG_VSIZE,    PB_LCD_VSIZE);
	eve_write16(pb.lcd, EVE_REG_VCYCLE,   PB_LCD_VCYCLE);
	eve_write16(pb.lcd, EVE_REG_VOFFSET,  PB_LCD_VOFFSET);
	eve_write16(pb.lcd, EVE_REG_VSYNC0,   PB_LCD_VSYNC0);
	eve_write16(pb.lcd, EVE_REG_VSYNC1,   PB_LCD_VSYNC1);
	eve_write8(pb.lcd,  EVE_REG_PCLK_POL, PB_LCD_PCLKPOL);
#if 0
	eve_write8(REG_SWIZZLE, EVE_SWIZZLE);
	eve_write8(REG_CSPREAD, EVE_CSPREAD);
#endif

	/* Add a basic DL list command to clear to full blue. */
	eve_write32(pb.lcd, EVE_MAP_RAM_DL +  0, EVE_DLC_CLEAR_COLOR_RGB | 0x00000ff0);
	eve_write32(pb.lcd, EVE_MAP_RAM_DL +  4, EVE_DLC_CLEAR | EVE_CLEAR_COLOR | EVE_CLEAR_TAG | EVE_CLEAR_STENCIL);
	eve_write32(pb.lcd, EVE_MAP_RAM_DL +  8, EVE_DLC_DISPLAY);
	eve_write8(pb.lcd, EVE_REG_DLSWAP, 0x2);

	/* turn on the screen (enable DISP) */
	eve_write8(pb.lcd, EVE_REG_GPIO, 0x80);
	eve_write8(pb.lcd, EVE_REG_PCLK, PB_LCD_PCLK);
}

static void
init_lcd(void)
{
	init_lcd_spi();
	init_lcd_specs();
}

void
app_main(void)
{
	init_logs();
	init_spi();
	init_lcd();

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
