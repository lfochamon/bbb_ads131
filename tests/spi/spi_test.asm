.origin 0                        // start of program in PRU memory
.entrypoint BEGIN                // program entry point

#define INS_PER_US 200
#define INS_PER_DELAY_LOOP 2
#define DELAY_US  100 * 1000 * (INS_PER_US / INS_PER_DELAY_LOOP)

#define SPI_SCLK r30.t5 /* P8_42 */
#define SPI_MOSI r30.t0 /* P8_45 */
#define SPI_MISO r31.t3 /* P8_44 */
#define SPI_CS   r30.t7 /* P8_40 */

#define SPI_SCLK_DELAY 9

#define PRU0_R31_VEC_VALID 32
#define PRU_EVTOUT_1 4

#include "../../spi.h"


BEGIN:
// Enable the OCP master port
LBCO    r0, C4, 4, 4    // load SYSCFG reg into r0 (use c4 const addr)
CLR     r0, r0, 4       // clear bit 4 (STANDBY_INIT)
SBCO    r0, C4, 4, 4    // store the modified r0 back at the load addr


// Start test
MOV     r1, 0xF1AA
SPI_TX  r1.b0   // Send 0b10101010
SPI_RX  r1.b2   // Receive data on third byte of r1
SPI_TX  r1.b1   // Send 0b11110001

// Store received data in RAM
MOV     r3, 0           // Load RAM address to r3
SBBO    r1, r3, 0, 4    // Store data


// Wait 100 ms
MOV     r2, DELAY_US
DELAY:
SUB     r2, r2, 1
QBNE    DELAY, r2, 0


END:
// Send interrupt to host
MOV     R31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_1
HALT
