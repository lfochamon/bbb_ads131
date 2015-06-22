#include "pru.h"

int pru_setup()
{
  /* Initialize the PRU and prussdrv module (always returns 0) */
  prussdrv_init();

  /* Open interrupt(s): you should open at least PRU_EVTOUT_0 */
  if (prussdrv_open(PRU_EVTOUT_0) != 0) {
    fprintf(stderr, "There was an error while opening PRU_EVTOUT_0.\n");
    fprintf(stderr, "Did you load the PRU DTO?\n");
    return(-1);
  }

  if (prussdrv_open(PRU_EVTOUT_1) != 0) {
    fprintf(stderr, "There was an error while opening PRU_EVTOUT_1.\n");
    fprintf(stderr, "Did you load the PRU DTO?\n");
    return(-1);
  }

  /* Initialize struct with INTC interrupt map (see PRU ref. guide, Fig. 97) */
  tpruss_intc_initdata pru_intc_data = PRUSS_INTC_INITDATA;

  /* Initialize INTC */
  if (prussdrv_pruintc_init(&pru_intc_data) != 0) {
    return(-1);
    fprintf(stderr, "An error occurred while initializing INTC.");
  }

  return(0);
}


int pru_mmap(int pru_number, uint32_t **pru_mem)
{
    void *pruMem;

    if (pru_number == 0) {
        prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruMem);
    } else {
        prussdrv_map_prumem(PRUSS0_PRU1_DATARAM, &pruMem);
    }

    *pru_mem = (uint32_t *) pruMem;

    return(0);
}


int pru_start(int pru_number, char *program)
{
  /* Load and execute the PRU program */
  if (prussdrv_exec_program(pru_number, program) != 0) {
    fprintf(stderr, "An error occurred while loading and executing the PRU program.");
    return(-1);
  }

  return(0);
}


int pru_stop(int pru_number)
{
  /* Halt and disable the PRU (returns -1 if argument is not 0 or 1) */
  if (prussdrv_pru_disable(pru_number) != 0) {
    fprintf(stderr, "The argument passed to prussdrv_pru_disable() must be 0 or 1.\n");
    return(-1);
  }

  return(0);
}


int pru_cleanup()
{
  /* Release PRU clocks and disable prussdrv module (always returns 0) */
  prussdrv_exit();
  return(0);
}
