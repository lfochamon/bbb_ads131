/***************************************************************/
/* PRUSS driver for the ADI131E08 AFE IC from Texas Instrument */
/* Designed and tested for the BeagleBone Black                */
/***************************************************************/

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
    MOV r0, N
    SUB r0, r0, 1
    DELAY:
        SUB r0, r0, 1
        QBNE DELAY, r0, 0
.endm

.macro ADI_STARTUP
    /* RESET = 1, START = 0, CS = 1 */
    SET ADI_RESET
    CLR ADI_START
    SET SPI_CS

    ADI_WAIT 15*1000*1000   // Wait for 150 ms

    /* Pulse RESET */
    CLR ADI_RESET
    ADI_WAIT 60             // Wait for 600 ns
    SET ADI_RESET

    ADI_WAIT 1000*1000      // Wait for 10 ms

    /* Stop continuous data conversion mode */
    CLR SPI_CS      // CS = 0
    MOV r1.b0, SDATAC
    SPI_TX r1.b0
.endm

.macro ADI_WRITE_REG
.mparam reg_addr, data
    MOV r1, WREG | reg_addr | data
    SPI_TX r1.b2
    SPI_TX r1.b1
    SPI_TX r1.b0
.endm

.macro ADI_READ_REG
.mparam reg_addr, reg_data
    MOV r1, RREG | reg_addr
    SPI_TX r1.b1
    SPI_TX r1.b0
    SPI_RX reg_data
.endm
