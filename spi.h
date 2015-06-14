/**************************/
/* SPI bit-banging macros */
/**************************/

#ifndef SPI_SCLK
  #error SPI SCLK pin undefined.
#endif

#ifndef SPI_MOSI
  #error SPI MOSI pin undefined.
#endif

#ifndef SPI_MISO
  #error SPI MISO pin undefined.
#endif

#ifndef SPI_CS
  #error SPI CS pin undefined.
#endif

#ifndef SPI_SCLK_DELAY
  #error SPI: You must define the SCLK frequency by assigning SPI_SCLK_DELAY.
#endif


.macro NOP
  MOV r0, r0
.endm


.macro SPI_WAIT
.mparam delay, stop_early = 0
  MOV r0, delay
  SUB r0, r0, 2
  SUB r0, r0, stop_early
  NOP
  DELAY:
    SUB r0, r0, 1
    QBNE DELAY, r0, 0
.endm


.macro SPI_TX_CLK
.mparam delay
  SET SPI_SCLK              // SCLK = 1
  SPI_WAIT SPI_SCLK_DELAY
  CLR SPI_SCLK              // SCLK = 0
  SPI_WAIT SPI_SCLK_DELAY, 1
  NOP
  // Room for one more instruction before SCLK = 1
.endm


.macro SPI_TX
.mparam MSG_REG
  MOV SPI_MOSI, MSG_REG.t7  // Set output (bit 7)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t6  // Set output (bit 6)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t5  // Set output (bit 5)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t4  // Set output (bit 4)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t3  // Set output (bit 3)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t2  // Set output (bit 2)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t1  // Set output (bit 1)
  SPI_TX_CLK SPI_CLOCK
  MOV SPI_MOSI, MSG_REG.t0  // Set output (bit 0)
  SPI_TX_CLK SPI_CLOCK
  NOP
.endm


.macro SPI_RX_CLK
.mparam delay
  CLR SPI_SCLK              // SCLK = 0
  SPI_WAIT SPI_SCLK_DELAY
  SET SPI_SCLK              // SCLK = 1
  SPI_WAIT SPI_SCLK_DELAY, 1
  NOP
  // Room for one more instruction before SCLK = 1
.endm


.macro SPI_RX
.mparam MSG_REG
  SET SPI_SCLK              // SCLK = 1
  SPI_WAIT SPI_SCLK_DELAY, 1
  NOP

  MOV MSG_REG.t7, SPI_MISO  // Get input (bit 7)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t6, SPI_MISO  // Get input (bit 6)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t5, SPI_MISO  // Get input (bit 5)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t4, SPI_MISO  // Get input (bit 4)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t3, SPI_MISO  // Get input (bit 3)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t2, SPI_MISO  // Get input (bit 2)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t1, SPI_MISO  // Get input (bit 1)
  SPI_RX_CLK SPI_CLOCK
  MOV MSG_REG.t0, SPI_MISO  // Get input (bit 0)

  CLR SPI_SCLK              // SCLK = 0
  SPI_WAIT SPI_SCLK_DELAY
.endm
