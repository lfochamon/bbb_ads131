INS_PER_US          .set    200
INS_PER_DELAY_LOOP  .set    2
DELAY_US            .set    100 * 1000 * (INS_PER_US / INS_PER_DELAY_LOOP)

SPI_SCLK            .set    r30.t5      ; P8_42
SPI_MOSI            .set    r30.t0      ; P8_45
SPI_MISO            .set    r31.t3      ; P8_44
SPI_CS              .set    r30.t7      ; P8_40

SPI_SCLK_DELAY      .set    9

PRU_INT_VALID       .set    32
PRU_EVTOUT_1        .set    4


.include "../../spi.h"


; Code starts here
    .text
    .retain
    .retainrefs
    .global         main

main:
; Enable the OCP master port
    LBCO    r0, C4, 4, 4    ; load SYSCFG reg into r0 (use c4 const addr)
    CLR     r0, r0, 4       ; clear bit 4 (STANDBY_INIT)
    SBCO    r0, C4, 4, 4    ; store the modified r0 back at the load addr


; Start test
    LDI32   r1, 0xF1AA
    SPI_TX  r1.b0           ; Send 0b10101010
    SPI_RX  r1.b2           ; Receive data on third byte of r1
    SPI_TX  r1.b1           ; Send 0b11110001

; Store received data in RAM
    MOV     r3, 0           ; Load RAM address to r3
    SBBO    r1, r3, 0, 4    ; Store data


; Wait 100 ms
    LDI32   r2, DELAY_US
delay:
    SUB     r2, r2, 1
    QBNE    DELAY, r2, 0


; Send interrupt to host and stop PRU
    LDI     r31.b0, PRU_INT_VALID + PRU_EVTOUT_1
    HALT
