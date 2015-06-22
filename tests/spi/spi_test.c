#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#include "pru.h"

#define PRU_N 1

int main(int argc, char *argv[])
{
    uint32_t *pru1_mem;

    /* PRU code only works if executed as root */
    if (getuid() != 0) {
      fprintf(stderr, "This program needs to run as root.\n");
        exit(EXIT_FAILURE);
    }


    /* PRU START UP */
    if (pru_setup() != 0) {
        pru_cleanup();
        exit(EXIT_FAILURE);
    }

    pru_mmap(PRU_N, &pru1_mem);


    /* Begin test */
    printf("Start SPI test.\n");

    *(pru1_mem) = 0xFFFFFFFF;

    if (pru_start(PRU_N, "./spi_test.bin") != 0) {
        fprintf(stderr, "Error starting the PRU.\n");
        pru_cleanup();
        exit(EXIT_FAILURE);
    }

    /* Wait for PRU_EVTOUT_1 and send shared RAM data */
    prussdrv_pru_wait_event(PRU_EVTOUT_1);
    prussdrv_pru_clear_event(PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);

    printf("Received data: %#010x.\n", *(pru1_mem));

    /* PRU CLEAN UP */
    pru_stop(PRU_N);
    pru_cleanup();
    printf("Close SPI test.\n");

    return(EXIT_SUCCESS);
}
