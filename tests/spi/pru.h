#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

/* Setup PRU driver: start up prussdrv, open interrupt events, */
/* and initialize intc                                         */
int pru_setup();

/* Map PRU DATARAM */
int pru_mmap(int pru_number, uint32_t **pru_mem);

/* Load binary file and start PRU */
int pru_start(int pru_number, char *program);

/* Stop PRU */
int pru_stop(int pru_number);

/* Clean up: stop prussdrv and release PRU clocks */
int pru_cleanup();


/* INTC initialization data (from pruss_intc_mapping.h) */
/* PRUSS_INTC_INITDATA {
  // Array 1: enables system events listed
  { PRU0_PRU1_INTERRUPT, PRU1_PRU0_INTERRUPT, PRU0_ARM_INTERRUPT,
    PRU1_ARM_INTERRUPT, ARM_PRU0_INTERRUPT, ARM_PRU1_INTERRUPT,  (char)-1  }

  // Array 2: assigns system events to channel numbers
  { {PRU0_PRU1_INTERRUPT,CHANNEL1}, {PRU1_PRU0_INTERRUPT, CHANNEL0},
    {PRU0_ARM_INTERRUPT,CHANNEL2}, {PRU1_ARM_INTERRUPT, CHANNEL3},
    {ARM_PRU0_INTERRUPT, CHANNEL0}, {ARM_PRU1_INTERRUPT, CHANNEL1},
    {-1,-1} }

  // Array 3: links channel numbers to host numbers
  { {CHANNEL0,PRU0}, {CHANNEL1, PRU1},
    {CHANNEL2, PRU_EVTOUT0}, {CHANNEL3, PRU_EVTOUT1}, {-1,-1} }

  // Array 4: creates mask to enable host interrupts or event outs
  // (e.g., PRU0, PRU1, PRU_EVTOUT0, PRU_EVTOUT1)
  (PRU0_HOSTEN_MASK | PRU1_HOSTEN_MASK | PRU_EVTOUT0_HOSTEN_MASK | PRU_EVTOUT1_HOSTEN_MASK)
} */
