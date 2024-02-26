#ifndef EVE_H
#define EVE_H

#include <stddef.h>
#include <stdint.h>

/*
 * All descriptions are taken from EVE Series Programming Guide (v2.4) and
 * BT81X datasheet.
 */

/*
 * Host commands (p4.1.5)
 */
#define EVE_CMD_ACTIVE                  ((uint8_t)0x00)
#define EVE_CMD_STANDBY                 ((uint8_t)0x41)
#define EVE_CMD_SLEEP                   ((uint8_t)0x42)
#define EVE_CMD_PWRDOWN                 ((uint8_t)0x43)
#define EVE_CMD_CLKEXT                  ((uint8_t)0x44)
#define EVE_CMD_CLKINT                  ((uint8_t)0x48)
#define EVE_CMD_CLKSEL                  ((uint8_t)0x61)
#define EVE_CMD_RST_PULSE               ((uint8_t)0x68)
#define EVE_CMD_PINDRIVE                ((uint8_t)0x70)
#define EVE_CMD_PIN_PD_STAT             ((uint8_t)0x71)

/* Interrupts (p4.1.6) */
#define EVE_IRQ_SWAP                    ((uint8_t)0x01)
#define EVE_IRQ_TOUCH                   ((uint8_t)0x02)
#define EVE_IRQ_TAG                     ((uint8_t)0x04)
#define EVE_IRQ_SOUND                   ((uint8_t)0x08)
#define EVE_IRQ_PLAYBACK                ((uint8_t)0x10)
#define EVE_IRQ_CMDEMPTY                ((uint8_t)0x20)
#define EVE_IRQ_CMDFLAG                 ((uint8_t)0x40)
#define EVE_IRQ_CONVCOMPLETE            ((uint8_t)0x80)

/* Registers (p5.1). */

/*
 * For convenience these macros are already set to the appropriate address
 * rather than their offset.
 *
 * Example, EVE_REG_FRAMES is defined at offset 0x004 but on the memory map
 * EVE_MAP_RAM_REG. If you wish to get its relative address mask to 0xfff.
 */
#define EVE_REG_ID                      ((uint32_t)0x302000) /* ro, 8b   */
#define EVE_REG_FRAMES                  ((uint32_t)0x302004) /* ro, 32b  */
#define EVE_REG_CLOCK                   ((uint32_t)0x302008) /* ro, 32b  */
#define EVE_REG_FREQUENCY               ((uint32_t)0x30200C) /* rw, 28b  */
#define EVE_REG_RENDERMODE              ((uint32_t)0x302010) /* rw, 1b   */
#define EVE_REG_SNAPY                   ((uint32_t)0x302014) /* rw, 11b  */
#define EVE_REG_SNAPSHOT                ((uint32_t)0x302018) /* rw, 1b   */
#define EVE_REG_SNAPFORMAT              ((uint32_t)0x30201C) /* rw, 6b   */
#define EVE_REG_CPURESET                ((uint32_t)0x302020) /* rw, 3b   */
#define EVE_REG_TAP_CRC                 ((uint32_t)0x302024) /* ro, 3b   */
#define EVE_REG_TAP_MASK                ((uint32_t)0x302028) /* rw, 32b  */
#define EVE_REG_HCYCLE                  ((uint32_t)0x30202C) /* rw, 12b  */
#define EVE_REG_HOFFSET                 ((uint32_t)0x302030) /* rw, 12b  */
#define EVE_REG_HSIZE                   ((uint32_t)0x302034) /* rw, 12b  */
#define EVE_REG_HSYNC0                  ((uint32_t)0x302038) /* rw, 12b  */
#define EVE_REG_HSYNC1                  ((uint32_t)0x30203C) /* rw, 12b  */
#define EVE_REG_VCYCLE                  ((uint32_t)0x302040) /* rw, 12b  */
#define EVE_REG_VOFFSET                 ((uint32_t)0x302044) /* rw, 12b  */
#define EVE_REG_VSIZE                   ((uint32_t)0x302048) /* rw, 12b  */
#define EVE_REG_VSYNC0                  ((uint32_t)0x30204C) /* rw, 10b  */
#define EVE_REG_VSYNC1                  ((uint32_t)0x302050) /* rw, 10b  */
#define EVE_REG_DLSWAP                  ((uint32_t)0x302054) /* rw, 2b   */
#define EVE_REG_ROTATE                  ((uint32_t)0x302058) /* rw, 3b   */
#define EVE_REG_OUTBITS                 ((uint32_t)0x30205C) /* rw, 9b   */
#define EVE_REG_DITHER                  ((uint32_t)0x302060) /* rw, 1b   */
#define EVE_REG_SWIZZLE                 ((uint32_t)0x302064) /* rw, 4b   */
#define EVE_REG_CSPREAD                 ((uint32_t)0x302068) /* rw, 1b   */
#define EVE_REG_PCLK_POL                ((uint32_t)0x30206C) /* rw, 1b   */
#define EVE_REG_PCLK                    ((uint32_t)0x302070) /* rw, 8b   */
#define EVE_REG_TAG_X                   ((uint32_t)0x302074) /* rw, 11b  */
#define EVE_REG_TAG_Y                   ((uint32_t)0x302078) /* rw, 11b  */
#define EVE_REG_TAG                     ((uint32_t)0x30207C) /* ro, 8b   */
#define EVE_REG_VOL_PB                  ((uint32_t)0x302080) /* rw, 8b   */
#define EVE_REG_VOL_SOUND               ((uint32_t)0x302084) /* rw, 8b   */
#define EVE_REG_SOUND                   ((uint32_t)0x302088) /* rw, 16b  */
#define EVE_REG_PLAY                    ((uint32_t)0x30208C) /* rw, 1b   */
#define EVE_REG_GPIO_DIR                ((uint32_t)0x302090) /* rw, 8b   */
#define EVE_REG_GPIO                    ((uint32_t)0x302094) /* rw, 8b   */
#define EVE_REG_GPIOX_DIR               ((uint32_t)0x302098) /* rw, 16b  */
#define EVE_REG_GPIOX                   ((uint32_t)0x30209C) /* rw, 16b  */
#define EVE_REG_INT_FLAGS               ((uint32_t)0x3020A8) /* ro, 8b   */
#define EVE_REG_INT_EN                  ((uint32_t)0x3020AC) /* rw, 1b   */
#define EVE_REG_INT_MASK                ((uint32_t)0x3020B0) /* rw, 8b   */
#define EVE_REG_PLAYBACK_START          ((uint32_t)0x3020B4) /* rw, 20b  */
#define EVE_REG_PLAYBACK_LENGTH         ((uint32_t)0x3020B8) /* rw, 20b  */
#define EVE_REG_PLAYBACK_READPTR        ((uint32_t)0x3020B8) /* ro, 20b  */
#define EVE_REG_PLAYBACK_FREQ           ((uint32_t)0x3020C0) /* rw, 16b  */
#define EVE_REG_PLAYBACK_FORMAT         ((uint32_t)0x3020C4) /* rw, 2b   */
#define EVE_REG_PLAYBACK_LOOP           ((uint32_t)0x3020C8) /* rw, 1b   */
#define EVE_REG_PLAYBACK_PLAY           ((uint32_t)0x3020CC) /* rw, 1b   */
#define EVE_REG_PWM_HZ                  ((uint32_t)0x3020D0) /* rw, 14b  */
#define EVE_REG_PWM_DUTY                ((uint32_t)0x3020D4) /* rw, 8b   */
#define EVE_REG_MACRO_0                 ((uint32_t)0x3020D8) /* rw, 32b  */
#define EVE_REG_MACRO_1                 ((uint32_t)0x3020DC) /* rw, 32b  */
#define EVE_REG_CMD_READ                ((uint32_t)0x3020F8) /* rw, 12b  */
#define EVE_REG_CMD_WRITE               ((uint32_t)0x3020FC) /* ro, 12b  */
#define EVE_REG_CMD_DL                  ((uint32_t)0x302100) /* rw, 13b  */
#define EVE_REG_TOUCH_MODE              ((uint32_t)0x302104) /* rw, 2b   */
#define EVE_REG_TOUCH_ADC_MODE          ((uint32_t)0x302108) /* rw, 1b   */
#define EVE_REG_CTOUCH_EXTENDED         ((uint32_t)0x302108) /* rw, 1b   */
#define EVE_REG_TOUCH_CHARGE            ((uint32_t)0x30210C) /* rw, 16b  */
#define EVE_REG_EHOST_TOUCH_X           ((uint32_t)0x30210C) /* rw, 16b  */
#define EVE_REG_HOST_SETTLE             ((uint32_t)0x302110) /* rw, 4b   */
#define EVE_REG_HOST_OVERSAMPLE         ((uint32_t)0x302114) /* rw, 4b   */
#define EVE_REG_EHOST_TOUCH_ID          ((uint32_t)0x302114) /* rw, 4b   */
#define EVE_REG_TOUCH_RZTHRES           ((uint32_t)0x302118) /* rw, 16b  */
#define EVE_REG_EHOST_TOUCH_Y           ((uint32_t)0x302118) /* rw, 16b  */
#define EVE_REG_TOUCH_RAW_XY            ((uint32_t)0x30211C) /* ro, 32b  */
#define EVE_REG_CTOUCH_TOUCH1_XY        ((uint32_t)0x30211C) /* ro, 32b  */
#define EVE_REG_TOUCH_RZ                ((uint32_t)0x302120) /* ro, 16b  */
#define EVE_REG_CTOUCH_TOUCH4_Y         ((uint32_t)0x302120) /* ro, 16b  */
#define EVE_REG_TOUCH_SCREEN_XY         ((uint32_t)0x302124) /* ro, 16b  */
#define EVE_REG_CTOUCH_TOUCH0_XY        ((uint32_t)0x302124) /* ro, 16b  */
#define EVE_REG_TOUCH_TAG_XY            ((uint32_t)0x302128) /* ro, 32b  */
#define EVE_REG_TOUCH_TAG               ((uint32_t)0x30212C) /* ro, 8b   */
#define EVE_REG_TOUCH_TAG1_XY           ((uint32_t)0x302130) /* ro, 32b  */
#define EVE_REG_TOUCH_TAG1              ((uint32_t)0x302134) /* ro, 8b   */
#define EVE_REG_TOUCH_TAG2_XY           ((uint32_t)0x302138) /* ro, 32b  */
#define EVE_REG_TOUCH_TAG2              ((uint32_t)0x30213C) /* ro, 8b   */
#define EVE_REG_TOUCH_TAG3_XY           ((uint32_t)0x302140) /* ro, 32b  */
#define EVE_REG_TOUCH_TAG3              ((uint32_t)0x302144) /* ro, 8b   */
#define EVE_REG_TOUCH_TAG4_XY           ((uint32_t)0x302148) /* ro, 32b  */
#define EVE_REG_TOUCH_TAG4              ((uint32_t)0x30214C) /* ro, 8b   */
#define EVE_REG_TOUCH_TRANSFORM_A       ((uint32_t)0x302150) /* rw, 32b  */
#define EVE_REG_TOUCH_TRANSFORM_B       ((uint32_t)0x302154) /* rw, 32b  */
#define EVE_REG_TOUCH_TRANSFORM_C       ((uint32_t)0x302158) /* rw, 32b  */
#define EVE_REG_TOUCH_TRANSFORM_D       ((uint32_t)0x30215C) /* rw, 32b  */
#define EVE_REG_TOUCH_TRANSFORM_E       ((uint32_t)0x302160) /* rw, 32b  */
#define EVE_REG_TOUCH_TRANSFORM_F       ((uint32_t)0x302164) /* rw, 32b  */
#define EVE_REG_TOUCH_CONFIG            ((uint32_t)0x302168) /* rw, 16b  */
#define EVE_REG_CTOUCH_TOUCH4_X         ((uint32_t)0x30216C) /* ro, 16b  */
#define EVE_REG_EHOST_TOUCH_ACK         ((uint32_t)0x302170) /* rw, 4b   */
#define EVE_REG_BIST_EN                 ((uint32_t)0x302174) /* rw, 1b   */
#define EVE_REG_TRIM                    ((uint32_t)0x302180) /* rw, 5b   */
#define EVE_REG_ANA_COMP                ((uint32_t)0x302184) /* rw, 8b   */
#define EVE_REG_SPI_WIDTH               ((uint32_t)0x302188) /* rw, 3b   */
#define EVE_REG_TOUCH_DIRECT_XY         ((uint32_t)0x30218C) /* ro, 32b  */
#define EVE_REG_CTOUCH_TOUCH2_XY        ((uint32_t)0x30218C) /* ro, 32b  */
#define EVE_REG_TOUCH_DIRECT_Z1Z2       ((uint32_t)0x302190) /* ro, 32b  */
#define EVE_REG_CTOUCH_TOUCH3_XY        ((uint32_t)0x302190) /* ro, 32b  */
#define EVE_REG_DATESTAMP               ((uint32_t)0x302564) /* ro, 128b */
#define EVE_REG_CMDB_SPACE              ((uint32_t)0x302574) /* rw, 12b  */
#define EVE_REG_CMDB_WRITE              ((uint32_t)0x302578) /* wo, 32b  */
#define EVE_REG_ADAPTIVE_FRAMERAT       ((uint32_t)0x30257C) /* rw, 1b   */
#define EVE_REG_PLAYBACK_PAUSE          ((uint32_t)0x3025EC) /* rw, 1b   */
#define EVE_REG_FLASH_STATUS            ((uint32_t)0x3025F0) /* rw, 2b   */

/* Memory map (p5). */
#define EVE_MAP_RAM_G                   ((uint32_t)0x00000000) /* 1024kB */
#define EVE_MAP_ROM                     ((uint32_t)0x00200000) /* 1024kB */
#define EVE_MAP_RAM_DL                  ((uint32_t)0x00300000) /* 8kB */
#define EVE_MAP_RAM_REG                 ((uint32_t)0x00302000) /* 4kB */
#define EVE_MAP_RAM_CMD                 ((uint32_t)0x00308000) /* 4kB */
#define EVE_MAP_FLASH                   ((uint32_t)0x00800000) /* 256MB */

/* Display List Commands (p4). */
#define EVE_DLC_ALPHA_FUNC              ((uint32_t)0x09000000)
#define EVE_DLC_BEGIN                   ((uint32_t)0x1f000000)
#define EVE_DLC_BITMAP_EXT_FORMAT       ((uint32_t)0x2e000000)
#define EVE_DLC_BITMAP_HANDLE           ((uint32_t)0x05000000)
#define EVE_DLC_BITMAP_LAYOUT           ((uint32_t)0x07000000)
#define EVE_DLC_BITMAP_LAYOUT_H         ((uint32_t)0x28000000)
#define EVE_DLC_BITMAP_SIZE             ((uint32_t)0x08000000)
#define EVE_DLC_BITMAP_SIZE_H           ((uint32_t)0x29000000)
#define EVE_DLC_BITMAP_SOURCE           ((uint32_t)0x01000000)
#define EVE_DLC_BITMAP_SWIZZLE          ((uint32_t)0x2f000000)
#define EVE_DLC_BITMAP_TRANSFORM_A      ((uint32_t)0x15000000)
#define EVE_DLC_BITMAP_TRANSFORM_B      ((uint32_t)0x16000000)
#define EVE_DLC_BITMAP_TRANSFORM_C      ((uint32_t)0x17000000)
#define EVE_DLC_BITMAP_TRANSFORM_D      ((uint32_t)0x18000000)
#define EVE_DLC_BITMAP_TRANSFORM_E      ((uint32_t)0x19000000)
#define EVE_DLC_BITMAP_TRANSFORM_F      ((uint32_t)0x1A000000)
#define EVE_DLC_BLEND_FUNC              ((uint32_t)0x0b000000)
#define EVE_DLC_CALL                    ((uint32_t)0x1D000000)
#define EVE_DLC_CELL                    ((uint32_t)0x06000000)
#define EVE_DLC_CLEAR                   ((uint32_t)0x26000000)
#define EVE_DLC_CLEAR_COLOR_A           ((uint32_t)0x0f000000)
#define EVE_DLC_CLEAR_COLOR_RGB         ((uint32_t)0x02000000)
#define EVE_DLC_CLEAR_STENCIL           ((uint32_t)0x11000000)
#define EVE_DLC_CLEAR_TAG               ((uint32_t)0x12000000)
#define EVE_DLC_COLOR_A                 ((uint32_t)0x10000000)
#define EVE_DLC_COLOR_MASK              ((uint32_t)0x20000000)
#define EVE_DLC_COLOR_RGB               ((uint32_t)0x04000000)
#define EVE_DLC_DISPLAY                 ((uint32_t)0x00000000)
#define EVE_DLC_END                     ((uint32_t)0x21000000)
#define EVE_DLC_JUMP                    ((uint32_t)0x1e000000)
#define EVE_DLC_LINE_WIDTH              ((uint32_t)0x0e000000)
#define EVE_DLC_MACRO                   ((uint32_t)0x25000000)
#define EVE_DLC_NOP                     ((uint32_t)0x2d000000)
#define EVE_DLC_PALETTE_SOURCE          ((uint32_t)0x2a000000)
#define EVE_DLC_POINT_SIZE              ((uint32_t)0x0d000000)
#define EVE_DLC_RESTORE_CONTEXT         ((uint32_t)0x23000000)
#define EVE_DLC_RETURN                  ((uint32_t)0x24000000)
#define EVE_DLC_SAVE_CONTEXT            ((uint32_t)0x22000000)
#define EVE_DLC_SCISSOR_SIZE            ((uint32_t)0x1c000000)
#define EVE_DLC_SCISSOR_XY              ((uint32_t)0x1b000000)
#define EVE_DLC_STENCIL_FUNC            ((uint32_t)0x0a000000)
#define EVE_DLC_STENCIL_MASK            ((uint32_t)0x13000000)
#define EVE_DLC_STENCIL_OP              ((uint32_t)0x0c000000)
#define EVE_DLC_TAG                     ((uint32_t)0x03000000)
#define EVE_DLC_TAG_MASK                ((uint32_t)0x14000000)
#define EVE_DLC_VERTEX2F                ((uint32_t)0x01000000)
#define EVE_DLC_VERTEX2II               ((uint32_t)0x02000000)
#define EVE_DLC_VERTEX_FORMAT           ((uint32_t)0x27000000)
#define EVE_DLC_VERTEX_TRANSLATE_X      ((uint32_t)0x2b000000)
#define EVE_DLC_VERTEX_TRANSLATE_Y      ((uint32_t)0x2c000000

#define EVE_PRIM_BITMAPS                ((uint32_t)0x00000001)
#define EVE_PRIM_POINTS                 ((uint32_t)0x00000002)
#define EVE_PRIM_LINES                  ((uint32_t)0x00000003)
#define EVE_PRIM_LINE_STRIP             ((uint32_t)0x00000004)
#define EVE_PRIM_EDGE_STRIP_R           ((uint32_t)0x00000005)
#define EVE_PRIM_EDGE_STRIP_L           ((uint32_t)0x00000006)
#define EVE_PRIM_EDGE_STRIP_A           ((uint32_t)0x00000007)
#define EVE_PRIM_EDGE_STRIP_B           ((uint32_t)0x00000008)
#define EVE_PRIM_RECTS                  ((uint32_t)0x00000009)

#define EVE_CLEAR_TAG                   ((uint32_t)0x00000001)
#define EVE_CLEAR_STENCIL               ((uint32_t)0x00000002)
#define EVE_CLEAR_COLOR                 ((uint32_t)0x00000004)

/*
 * Opaque configuration detailed individually in platform code.
 */
struct eve_cfg;

/**
 * Initialize device.
 */
intptr_t
eve_init(const struct eve_cfg *cfg);

int
eve_power(intptr_t devc, int enable);

#if 0

/**
 * Send a download list command.
 */
int
eve_dl(intptr_t devc, uint32_t data);

#endif

/**
 * Send a host command.
 */
int
eve_cmd(intptr_t devc, uint8_t cmd, uint8_t param);

int
eve_read8(intptr_t devc, uint32_t address, uint8_t *value);

int
eve_read16(intptr_t devc, uint32_t address, uint16_t *value);

int
eve_read32(intptr_t devc, uint32_t address, uint32_t *value);

int
eve_write8(intptr_t devc, uint32_t address, uint8_t value);

int
eve_write16(intptr_t devc, uint32_t address, uint16_t value);

int
eve_write32(intptr_t devc, uint32_t address, uint32_t value);

/**
 * Dispose resource.
 */
void
eve_finish(intptr_t devc);

#endif /* !EVE_H */
