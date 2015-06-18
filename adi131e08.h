/***************************************************************/
/* PRUSS driver for the ADI131E08 AFE IC from Texas Instrument */
/* Designed and tested for the BeagleBone Black                */
/***************************************************************/

/* NOTE: Before including this file, define ADI_START, ADI_RESET, and ADI_DRDY. */

/* WARNING: Most of these macros modify r28 and r29 (spi.h) without warning. */

#ifndef ADI_START
  #error ADI_START pin undefined.
#endif

#ifndef ADI_RESET
  #error ADI_RESET pin undefined.
#endif

#ifndef ADI_DRDY
  #error ADI_DRDY pin undefined.
#endif

/* 50 ns < t_SCLK < (t_DR – 4 t_CLK) / (N_bits*N_channels + 24) */
/* Using main clock @ 2.048 MHz:
 * 64 kSps: t_SCLK < 89 ns
 * 32 kSps: t_SCLK < 192 ns
 * 16 kSps: t_SCLK < 280 ns
 *  8 kSps: t_SCLK < 569 ns
 *  4 kSps: t_SCLK < 1.14 us
 *  2 kSps: t_SCLK < 2.3 us
 *  1 kSps: t_SCLK < 4.6 us */

/* WARNING: For CLK @ 2.048 MHz: if the SCLK period is < 62 ns, then SCLK needs
 * to be bursty to meet t_SDECODE = 489 ns (not implemented) */

#include "adi131_defs.h"
#include "spi.h"

/* Wait for 2*[N] instructions: [N]/1e5 ms */
.macro ADI_WAIT
.mparam N
    MOV     r28, N
    SUB     r28, r28, 1
    DELAY:
        SUB     r28, r28, 1
        QBNE    DELAY, r28, 0
.endm

.macro ADI_STARTUP
    /* RESET = 1, START = 0, CS = 1 */
    SET     ADI_RESET
    CLR     ADI_START
    SET     SPI_CS

    ADI_WAIT    15*1000*1000   // Wait for 150 ms

    /* Pulse RESET */
    CLR         ADI_RESET
    ADI_WAIT    60             // Wait for 600 ns
    SET         ADI_RESET

    ADI_WAIT    1000*1000      // Wait for 10 ms

    /* Stop continuous data conversion mode */
    CLR     SPI_CS      // CS = 0
    MOV     r28.b0, SDATAC
    SPI_TX  r28.b0
.endm

.macro ADI_WRITE_REG
.mparam reg_addr, data
    MOV     r28, WREG | reg_addr | data
    SPI_TX  r28.b2
    SPI_TX  r28.b1
    SPI_TX  r28.b0
.endm

.macro ADI_READ_REG
.mparam reg_addr, reg_data
    MOV     r28, RREG | reg_addr
    SPI_TX  r28.b1
    SPI_TX  r28.b0
    SPI_RX  reg_data
.endm

.macro ADI_GET_DATA16
.mparam stat_reg = r1, reg_chan1 = r2.w2, reg_chan2 = r2.w0, reg_chan3 = r3.w2, reg_chan4 = r3.w0, reg_chan5 = r4.w2, reg_chan6 = r4.w0, reg_chan7 = r5.w2, reg_chan8 = r5.w0
    /* Reset STAT register */
    MOV     stat_reg, 0

    /* Wait for DRDY = 0 */
    WBC     ADI_DRDY

    CLR     SPI_CS      // Enable SPI (CS = 0)

    /* Receive STAT packet (24 bits) */
    SPI_RX  stat_reg.b2
    SPI_RX  stat_reg.b1
    SPI_RX  stat_reg.b0

    /* Receive channel 1 packet (16 bits) */
    SPI_RX  reg_chan1.b1
    SPI_RX  reg_chan1.b0

    /* Receive channel 2 packet (16 bits) */
    SPI_RX  reg_chan2.b1
    SPI_RX  reg_chan2.b0

    /* Receive channel 3 packet (16 bits) */
    SPI_RX  reg_chan3.b1
    SPI_RX  reg_chan3.b0

    /* Receive channel 4 packet (16 bits) */
    SPI_RX  reg_chan4.b1
    SPI_RX  reg_chan4.b0

    /* Receive channel 5 packet (16 bits) */
    SPI_RX  reg_chan5.b1
    SPI_RX  reg_chan5.b0

    /* Receive channel 6 packet (16 bits) */
    SPI_RX  reg_chan6.b1
    SPI_RX  reg_chan6.b0

    /* Receive channel 7 packet (16 bits) */
    SPI_RX  reg_chan7.b1
    SPI_RX  reg_chan7.b0

    /* Receive channel 8 packet (16 bits) */
    SPI_RX  reg_chan8.b1
    SPI_RX  reg_chan8.b0

    SET     SPI_CS      // Disable SPI (CS = 1)
.endm
