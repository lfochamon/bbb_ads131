/***************************************************************/
/* PRUSS driver for the ADI131E08 AFE IC from Texas Instrument */
/* Designed and tested for the BeagleBone Black                */
/***************************************************************/

/* Pin definitions */
#define SPI_SCLK r30.t5 /* PRU1_5 GPIO2_11 P8_42 */
#define SPI_MOSI r30.t0 /* PRU1_0 GPIO2_6  P8_45 */ /* DIN */
#define SPI_MISO r31.t3 /* PRU1_3 GPIO2_9  P8_44 */ /* DOUT */
#define SPI_CS   r30.t7 /* PRU1_7 GPIO2_13 P8_40 */
#define START    r30.t4 /* PRU1_4 GPIO2_10 P8_41 */
#define RESET    r30.t2 /* PRU1_2 GPIO2_8  P8_43 */
#define DRDY     r31.t1 /* PRU1_1 GPIO2_7  P8_46 */

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
