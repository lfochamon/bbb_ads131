/**********************************************************/
/* Defines for the ADI131E08 AFE IC from Texas Instrument */
/**********************************************************/


/*******************************/
/***** Registers addresses *****/
/*******************************/
/* Note: register addresses are padded with 0x00 to make
 * read/write commands more straightforward. */

/* Device settings (read-only) */
#define ID 0x0000

/* Global settings */
#define CONFIG1 0x0100
#define CONFIG2 0x0200
#define CONFIG3 0x0300
#define FAULT 0x0400

/* Channel settings */
#define CH1SET 0x0500
#define CH2SET 0x0600
#define CH3SET 0x0700
#define CH4SET 0x0800
#define CH5SET 0x0900
#define CH6SET 0x0A00
#define CH7SET 0x0B00
#define CH8SET 0x0C00

/* Fault detect status registers (read-only) */
#define FAULT_STATP 0x1200
#define FAULT_STATN 0x1300

/* GPIOs */
#define GPIO 0x1400


/*********************************/
/***** Registers definitions *****/
/*********************************/

/* Global settings */
/* CONFIG1 (default = 0x91) */
#define CONFIG1_MASK 0x90
#define DAISY_IN     0x20   /* Enable multiple read back mode (default = 0, daisy-chain) */
#define CLK_EN       0x10   /* Enable oscilator output (default = 0, disabled) */
#define DR_64K       0x0    /* 64 kSps (16 bits) */
#define DR_32K       0x1    /* 32 kSps (16 bits, default) */
#define DR_16K       0x2    /* 16 kSps (24 bits) */
#define DR_08K       0x3    /*  8 kSps (24 bits) */
#define DR_04K       0x4    /*  4 kSps (24 bits) */
#define DR_02K       0x5    /*  2 kSps (24 bits) */
#define DR_01K       0x6    /*  1 kSps (24 bits) */

/* CONFIG2 (default = 0xE0) */
#define CONFIG2_MASK 0xE0
#define INT_TEST     0x10   /* Enable internal test signals (default = 0, external) */
#define TEST_AMP0    0x4    /* Test signal amplitude (default = 0, 1x) */
#define TEST_FREQ_21 0x0    /* Test frequency @ fclk / 2^21 (default) */
#define TEST_FREQ_20 0x1    /* Test frequency @ fclk / 2^20 */
#define TEST_FREQ_DC 0x3    /* Test frequency @ DC */

/* CONFIG3 (default = 0x40) */
#define CONFIG3_MASK 0x40
#define PDB_REFBUF   0x80   /* Enable internal reference buffer (default = 0, disabled) */
#define VREF_4V      0x10   /* Sets VREFP = 4V (default = 0, VREFP = 2.4V) */
#define OPAMP_REF    0x8    /* Non-inverting op amp input to (AVDD + AVSS)/2 (default = 0, connected to OPAMPP) */
#define PDB_OPAMP    0x4    /* Enable op amp (default = 0, op amp powered down) */

/* FAULT (default = 0x00) */
#define COMP_TH_5    0x0    /* Comparator threshold 5% - 95% (default) */
#define COMP_TH_7_5  0x1    /* Comparator threshold 7.5% - 92.5% */
#define COMP_TH_10   0x2    /* Comparator threshold 10% - 90% */
#define COMP_TH_12_5 0x3    /* Comparator threshold 12.5% - 87.5% */
#define COMP_TH_15   0x4    /* Comparator threshold 15% - 85% */
#define COMP_TH_20   0x5    /* Comparator threshold 20% - 80% */
#define COMP_TH_25   0x6    /* Comparator threshold 25% - 75% */
#define COMP_TH_30   0x7    /* Comparator threshold 30% - 70% */

/* Channel settings */
/* CHnSET (default = 0x10) */
#define CH_PD        0x80   /* Channel power down (default = 0, normal operation) */
#define CH_GAIN_1    0x10   /* Channel PGA gain 1x (default) */
#define CH_GAIN_2    0x20   /* Channel PGA gain 2x */
#define CH_GAIN_4    0x40   /* Channel PGA gain 4x */
#define CH_GAIN_8    0x50   /* Channel PGA gain 8x */
#define CH_GAIN_12   0x60   /* Channel PGA gain 12x */
#define CH_NORMAL    0x0    /* Channel MUX: normal (default) */
#define CH_SHORTED   0x1    /* Channel MUX: inputs shorted */
#define CH_VDD       0x3    /* Channel MUX: MVDD */
#define CH_TEMP      0x4    /* Channel MUX: temperature sensor */
#define CH_TEST      0x5    /* Channel MUX: test signal */

/* GPIO */
/* GPIO (default = 0x0F) */
#define GPIO4        0x80   /* Data for GPIO4 */
#define GPIO3        0x40   /* Data for GPIO3 */
#define GPIO2        0x20   /* Data for GPIO2 */
#define GPIO1        0x10   /* Data for GPIO1 */
#define GPIO_CTRL_4  0x8    /* Control for GPIO4 (0 = output; 1 = input, default) */
#define GPIO_CTRL_3  0x4    /* Control for GPIO3 (0 = output; 1 = input, default) */
#define GPIO_CTRL_2  0x2    /* Control for GPIO2 (0 = output; 1 = input, default) */
#define GPIO_CTRL_1  0x1    /* Control for GPIO1 (0 = output; 1 = input, default) */


/************************/
/***** SPI commands *****/
/************************/
/* System commands */
#define WAKEUP    0x02  /* Wake-up from standby mode */
#define STANDBY   0x04  /* Enter standby mode */
#define RESET     0x06  /* Reset the device */
#define START     0x08  /* Start or restart (synchronize) conversions */
#define STOP      0x0A  /* Stop conversion */
#define OFFSETCAL 0x1A  /* Channel offset calibration */

/* Data read commands */
#define RDATAC 0x10     /* Enable continuous data read mode (default at power-up) */
#define SDATAC 0x11     /* Stop continuous data read mode */
#define RDATA  0x12     /* Read data command */

/* Register Read Commands */
#define RREG 0x2000     /* Read registers (0b001r rrrr 0b000n nnnn: read n+1 registers starting at r) */
#define WREG 0x4000     /* Write registers (0b010r rrrr 0b000n nnnn: write n+1 registers starting at r) */
