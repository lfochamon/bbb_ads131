.origin 0                        // start of program in PRU memory

/* Pin definitions */
#define SPI_SCLK    r30.t5 /* PRU1_5 GPIO2_11 P8_42 */
#define SPI_MOSI    r30.t0 /* PRU1_0 GPIO2_6  P8_45 */ /* DIN */
#define SPI_MISO    r31.t3 /* PRU1_3 GPIO2_9  P8_44 */ /* DOUT */
#define SPI_CS      r30.t7 /* PRU1_7 GPIO2_13 P8_40 */
#define ADI_START   r30.t4 /* PRU1_4 GPIO2_10 P8_41 */
#define ADI_RESET   r30.t2 /* PRU1_2 GPIO2_8  P8_43 */
#define ADI_DRDY    r31.t1 /* PRU1_1 GPIO2_7  P8_46 */

/* SPI_SCLK_DELAY = floor( (t_SCLK / 5 ns) / 4 ); SPI_SCLK_DELAY >= 3 */
#define SPI_SCLK_DELAY 9

/* Interrupt */
#define PRU_INT_VALID 32
#define PRU0_PRU1_INTERRUPT 1   // PRU_EVTOUT_
#define PRU1_PRU0_INTERRUPT 2   // PRU_EVTOUT_
#define PRU0_ARM_INTERRUPT 3    // PRU_EVTOUT_0
#define PRU1_ARM_INTERRUPT 4    // PRU_EVTOUT_1
#define ARM_PRU0_INTERRUPT 5    // PRU_EVTOUT_
#define ARM_PRU1_INTERRUPT 6    // PRU_EVTOUT_

/* Name PRU register banks */
#define XFR_BANK0 10
#define XFR_BANK1 11
#define XFR_BANK2 12
#define XFR_PRU 14

#include "adi131e08.h"


/* Start up sequence for the ADI */
ADI_STARTUP

/* Activate internal clock */
ADI_WRITE_REG   CONFIG13, CONFIG3_MASK | PDB_REFBUF
ADI_WAIT        2000       // Wait 20 us for the internal clock to start up

/* Configure ADI */
ADI_WRITE_REG   CONFIG1, CONFIG1_MASK | DR_32K    // 16 bits @ 32 kHz, daisy-chain, disable clock output
ADI_WRITE_REG   CONFIG2, CONFIG2_MASK             // external test signal, gain 1x, fclk / 2^21 (all defaults)
ADI_WRITE_REG   CH1SET,  CH_GAIN_1 | CH_SHORTED   // Channel 1 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH2SET,  CH_GAIN_1 | CH_SHORTED   // Channel 2 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH3SET,  CH_GAIN_1 | CH_SHORTED   // Channel 3 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH4SET,  CH_GAIN_1 | CH_SHORTED   // Channel 4 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH5SET,  CH_GAIN_1 | CH_SHORTED   // Channel 5 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH6SET,  CH_GAIN_1 | CH_SHORTED   // Channel 6 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH7SET,  CH_GAIN_1 | CH_SHORTED   // Channel 7 (PGA gain 1x, channel shorted)
ADI_WRITE_REG   CH8SET,  CH_GAIN_1 | CH_SHORTED   // Channel 8 (PGA gain 1x, channel shorted)

/* Put ADI back in continuous data conversion mode */
MOV     r1.b0, RDATAC
SPI_TX  r1.b0

/* START = 1 */
SET     ADI_START

/* Start of main loop */
MAINLOOP:
    /* Wait and retrieve data */
    ADI_GET_DATA16  r1, r2, r3, r4, r5
    ADI_GET_DATA16  r1, r6, r7, r8, r9
    ADI_GET_DATA16  r1, r10, r11, r12, r13
    ADI_GET_DATA16  r1, r14, r15, r16, r17
    ADI_GET_DATA16  r1, r18, r19, r20, r21
    ADI_GET_DATA16  r1, r22, r23, r24, r25

    XOUT    XFR_BANK0, R1, 100                          // Send data to scratch pad
    MOV     R31.b0, PRU_INT_VALID | PRU1_PRU0_INTERRUPT // Send interrupt to PRU0

    QBA MAINLOOP        // [TODO]: make loop conditional


/* Send interrupt to host*/
MOV     R31.b0, PRU_INT_VALID | PRU1_ARM_INTERRUPT

/* Stop PRU */
HALT
