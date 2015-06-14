/***************************************************************/
/* PRUSS driver for the ADI131E08 AFE IC from Texas Instrument */
/* Designed and tested for the BeagleBone Black                */
/***************************************************************/

/* Pin definitions */
#define SPI_SCLK
#define SPI_MOSI
#define SPI_MISO
#define SPI_CS
#define START
#define RESET
#define DRDY

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

/* SPI_SCLK_DELAY = floor( (t_SCLK / 5 ns) / 4 ) (SPI_SCLK_DELAY >= 3) */
#define SPI_SCLK_DELAY 9
