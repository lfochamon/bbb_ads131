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
.mparam reg1 = r1, reg2 = r2, reg3 = r3, reg4 = r4, reg5 = r5
    /* Reset STAT register */
    MOV     reg1, 0

    /* Wait for DRDY = 0 */
    WBC     ADI_DRDY

    CLR     SPI_CS      // Enable SPI (CS = 0)

    /* Receive STAT packet (24 bits) */
    SPI_RX  reg1.b2
    SPI_RX  reg1.b1
    SPI_RX  reg1.b0

    /* Receive channel 1 packet (16 bits) */
    SPI_RX  reg2.w2.b1
    SPI_RX  reg2.w2.b0

    /* Receive channel 2 packet (16 bits) */
    SPI_RX  reg2.w0.b1
    SPI_RX  reg2.w0.b0

    /* Receive channel 3 packet (16 bits) */
    SPI_RX  reg3.w2.b1
    SPI_RX  reg3.w2.b0

    /* Receive channel 4 packet (16 bits) */
    SPI_RX  reg3.w0.b1
    SPI_RX  reg3.w0.b0

    /* Receive channel 5 packet (16 bits) */
    SPI_RX  reg4.w2.b1
    SPI_RX  reg4.w2.b0

    /* Receive channel 6 packet (16 bits) */
    SPI_RX  reg4.w0.b1
    SPI_RX  reg4.w0.b0

    /* Receive channel 7 packet (16 bits) */
    SPI_RX  reg5.w2,.b1
    SPI_RX  reg5.w2,.b0

    /* Receive channel 8 packet (16 bits) */
    SPI_RX  reg5.w0.b1
    SPI_RX  reg5.w0.b0

    SET     SPI_CS      // Disable SPI (CS = 1)
.endm

.macro ADI_READ_ALL
.mparam id_reg = r1.b3, config1_reg = r1.b2, config2_reg = r1.b1, config3_reg = r1.b0, fault_reg  = r2.b3, ch1set_reg = r2.b2, ch2set_reg = r2.b1, ch3set_reg = r2.b0, ch4set_reg = r3.b3, ch5set_reg = r3.b2, ch6set_reg = r3.b1, ch7set_reg = r3.b0, ch8set_reg = r4.b3, fault_statp_reg = r4.b2, fault_statn_reg = r4.b1, gpio_reg = r4.b0
    CLR     SPI_CS      // Enable SPI (CS = 0)

    MOV     r28, RREG | ID | 0xF    // Read 15 registers starting at ID (0x0)
    SPI_TX  r28.b1
    SPI_TX  r28.b0

    SPI_RX  id_reg
    SPI_RX  config1_reg
    SPI_RX  config2_reg
    SPI_RX  config3_reg
    SPI_RX  fault_reg
    SPI_RX  ch1set_reg
    SPI_RX  ch2set_reg
    SPI_RX  ch3set_reg
    SPI_RX  ch4set_reg
    SPI_RX  ch5set_reg
    SPI_RX  ch6set_reg
    SPI_RX  ch7set_reg
    SPI_RX  ch8set_reg
    SPI_RX  fault_statp_reg
    SPI_RX  fault_statn_reg
    SPI_RX  gpio_reg

    SET     SPI_CS      // Disable SPI (CS = 1)
.endm
