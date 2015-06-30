/******************************************************************************/
/*  bbb_pru.cmd                                                               */
/*  Linker Script                                                             */
/******************************************************************************/


/******************************************************************************/
/*  Link options                                                              */
/******************************************************************************/
-cr                                         /* Init. variables at load time   */
-stack  0x0100                              /* Software stack size            */
-heap   0x0100                              /* Heap size                      */

--disable_auto_rts                          /* Don't link with libc */
--entry_point=main                          /* Define entry point   */


/******************************************************************************/
/*  PRU memory map configurations                                             */
/******************************************************************************/
MEMORY
{
    PAGE 0:
       PRU_IRAM :   origin = 0x00000000     length = 0x00002000

    PAGE 1:
       PRU_DRAM :   origin = 0x00000000     length = 0x00002000
}


/******************************************************************************/
/*  Memory sections allocation                                                */
/******************************************************************************/
SECTIONS
{
    .text   :   > PRU_IRAM, PAGE 0      /* Code                               */

    .bss    :   > PRU_DRAM, PAGE 1      /* Unitialized global variables       */
    .data   :   > PRU_DRAM, PAGE 1      /* Init. global variables (non-const) */
    .rodata :   > PRU_DRAM, PAGE 1      /* Constant data                      */
    .sysmem :   > PRU_DRAM, PAGE 1      /* Heap for dynamic memory allocation */
    .stack  :   > PRU_DRAM, PAGE 1      /* Software stack                     */
    .cinit  :   > PRU_DRAM, PAGE 1      /* Init. tables for C global vars     */
    .const  :   > PRU_DRAM, PAGE 1      /* Init. constant data                */
}
