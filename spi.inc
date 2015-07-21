;************************
; SPI bit-banging macros
;************************

; NOTE: Before including this file, define SPI_SCLK, SPI_MOSI, SPI_MISO,
;       SPI_CS, and SPI_SCLK_DELAY

; WARNING: Most of these macros modify r29 without warning.

    .if $defined(SPI_SCLK) = 0
        .emsg "SPI: SPI_SCLK pin undefined."
    .endif

    .if $defined(SPI_MOSI) = 0
        .emsg "SPI: SPI_MOSI pin undefined."
    .endif

    .if $defined(SPI_MISO) = 0
        .emsg "SPI: SPI_MISO pin undefined."
    .endif

    .if $defined(SPI_CS) = 0
        .emsg "SPI: SPI_CS pin undefined."
    .endif

    .if $defined(SPI_SCLK_DELAY) = 0
        .emsg "SPI: You must define the SCLK frequency using SPI_SCLK_DELAY."
    .endif


NOP .macro
        MOV r0, r0
    .endm


SPI_WAIT    .macro  spi_wait_delay, spi_wait_stop_early
                LDI32   r29, spi_wait_delay
                SUB     r29, r29, 2
                SUB     r29, r29, spi_wait_stop_early
                NOP
delay?:         SUB     r29, r29, 1
                QBNE    delay?, r29, 0
            .endm


LBIT    .macro  lbit_reg1, lbit_bit1, lbit_reg2, lbit_bit2
            QBBS    high?, lbit_reg1, lbit_bit1
            CLR     lbit_reg2, lbit_reg2, lbit_bit2
            QBA     end?
high?:      SET     lbit_reg2, lbit_reg2, lbit_bit2
            NOP
end?:
        .endm


SPI_TX_CLK  .macro
                SET         r30, r30, SPI_SCLK          ; SCLK = 1
                SPI_WAIT    SPI_SCLK_DELAY, 0
                CLR         r30, r30, SPI_SCLK          ; SCLK = 0
                SPI_WAIT    SPI_SCLK_DELAY, 1
                NOP
                ; Room for one more instruction before SCLK = 1
            .endm


SPI_TX  .macro  spi_tx_reg
            LBIT        spi_tx_reg, 7, r30, SPI_MOSI    ; Set output (bit 7)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 6, r30, SPI_MOSI    ; Set output (bit 6)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 5, r30, SPI_MOSI    ; Set output (bit 5)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 4, r30, SPI_MOSI    ; Set output (bit 4)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 3, r30, SPI_MOSI    ; Set output (bit 3)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 2, r30, SPI_MOSI    ; Set output (bit 2)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 1, r30, SPI_MOSI    ; Set output (bit 1)
            SPI_TX_CLK
            LBIT        spi_tx_reg, 0, r30, SPI_MOSI    ; Set output (bit 0)
            SPI_TX_CLK
            NOP

            CLR r30, r30, SPI_MOSI                      ; Reset output to low
        .endm


SPI_RX_CLK  .macro
                CLR         r30, r30, SPI_SCLK          ; SCLK = 0
                SPI_WAIT    SPI_SCLK_DELAY, 0
                SET         r30, r30, SPI_SCLK          ; SCLK = 1
                SPI_WAIT    SPI_SCLK_DELAY, 1
                NOP
                ; Room for one more instruction before SCLK = 1
            .endm


SPI_RX      .macro  spi_rx_reg
                SET         r30, r30, SPI_SCLK              ; SCLK = 1
                SPI_WAIT    SPI_SCLK_DELAY, 1
                NOP

                LBIT        r31, SPI_MISO, spi_rx_reg, 7    ; Get input (bit 7)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 6    ; Get input (bit 6)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 5    ; Get input (bit 5)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 4    ; Get input (bit 4)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 3    ; Get input (bit 3)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 2    ; Get input (bit 2)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 1    ; Get input (bit 1)
                SPI_RX_CLK
                LBIT        r31, SPI_MISO, spi_rx_reg, 0    ; Get input (bit 0)

                CLR         r30, r30, SPI_SCLK              ; SCLK = 0
                SPI_WAIT    SPI_SCLK_DELAY, 0
            .endm
