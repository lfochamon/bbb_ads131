.origin 0                        // start of program in PRU memory

/* Interrupt */
#define PRU_INT_VALID 32
#define PRU0_PRU1_INTERRUPT 1   // PRU_EVTOUT_
#define PRU1_PRU0_INTERRUPT 2   // PRU_EVTOUT_
#define PRU0_ARM_INTERRUPT 3    // PRU_EVTOUT_0
#define PRU1_ARM_INTERRUPT 4    // PRU_EVTOUT_1
#define ARM_PRU0_INTERRUPT 5    // PRU_EVTOUT_
#define ARM_PRU1_INTERRUPT 6    // PRU_EVTOUT_

/* Name PRU constants */
#define PRU1_RAM c24
#define PRU0_RAM c24
#define SHARED_RAM c28

/* Name PRU register banks */
#define XFR_BANK0 10
#define XFR_BANK1 11
#define XFR_BANK2 12
#define XFR_PRU 14


/* Enable the OCP master port */
LBCO    r0, C4, 4, 4    // Load SYSCFG into r0 using c4 constant
CLR     r0, r0, 4       // Clear STANDBY_INIT (bit 4)
SBCO    r0, C4, 4, 4    // Store modified SYSCFG back

/* Set c24 = 0 and c25 = 0x2000 (data RAM in PRU1 and PRU0) */
MOV     r0, 0x24020     // Load CTBIR0 address in r0
MOV     r1, 0           // c24 = 0x00000n00, c25 = 0x00002n00
SBBO    r1, r0, 0, 4    // Store r1 into CTBIR0 (pointer r0)

/* Set C28 = 0x00010000 (shared PRU RAM) */
MOV     r0, 0x24028     // Load CTPPR0 address in r0
MOV     r1, 0x0100      // c28 = 0x00nnnn00
SBBO    r1, r0, 0, 4    // Store r1 into CTBIR0 (pointer r0)

/* Setup auxiliary registers */
MOV     r0, 18          // Load PRU1 interrupt number in r0 (PRU SRM p. 222)
LBCO    r28, c24, 0, 4  // Load DDR RAM address in r28


/* Start main loop */
MAINLOOP:
    MOV     r29, 0                  // Initialize byte counter in r29

    WBS     R30, 30                 // Wait for PRU1 interrupt signal
    SBCO    r0, C0, 0x24, 4         // Clear PRU1 interrupt

    XIN     XFR_BANK0, r1, 100      // Retrieve data from scratch pad
    SBCO    r1, r28, r29, 100       // Write data to DDR RAM
    ADD     r29, r29, 100

    /* Send interrupt to host*/
    MOV     R31.b0, PRU_INT_VALID | PRU0_ARM_INTERRUPT


    WBS     R30, 30                 // Wait for PRU1 interrupt signal
    SBCO    r0, C0, 0x24, 4         // Clear PRU1 interrupt

    XIN     XFR_BANK0, r1, 100      // Retrieve data from scratch pad
    SBCO    r1, r28, r29, 100       // Write data to DDR RAM
    ADD     r29, r29, 100

    /* Send interrupt to host*/
    MOV     R31.b0, PRU_INT_VALID | PRU0_ARM_INTERRUPT

    QBA     MAINLOOP                // [TODO]: make loop conditional


/* Stop PRU */
HALT
