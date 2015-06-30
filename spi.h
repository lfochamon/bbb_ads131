;************************
; SPI bit-banging macros
;************************

; NOTE: Before including this file, define SPI_SCLK, SPI_MOSI, SPI_MISO,
;       SPI_CS, and SPI_SCLK_DELAY

; WARNING: Most of these macros modify r29 without warning.

.if $defined(SPI_SCLK)
.emsg SPI: SPI_SCLK pin undefined.
.endif

.if $defined(SPI_MOSI)
.emsg SPI: SPI_MOSI pin undefined.
.endif

.if $defined(SPI_MISO)
.emsg SPI: SPI_MISO pin undefined.
.endif

.if $defined(SPI_CS)
.emsg SPI: SPI_CS pin undefined.
.endif

.if $defined(SPI_SCLK_DELAY)
.emsg SPI: You must define the SCLK frequency by assigning SPI_SCLK_DELAY.
.endif


NOP .macro
        MOV r0, r0
    .endm


SPI_WAIT    .macro  n_delay, stop_early = 0
                LDI32   r29, n_delay
                SUB     r29, r29, 2
                SUB     r29, r29, stop_early
                NOP
delay?:         SUB     r29, r29, 1
                QBNE    delay?, r29, 0
            .endm


LBIT    .macro  bit1, bit2
            QBBS    high?, bit1
            CLR     bit2
            QBA     end?
high?:      SET     bit2
            NOP
end?:
        .endm


SPI_TX_CLK  .macro
                SET         SPI_SCLK            ; SCLK = 1
                SPI_WAIT    SPI_SCLK_DELAY
                CLR         SPI_SCLK            ; SCLK = 0
                SPI_WAIT    SPI_SCLK_DELAY, 1
                NOP
                ; Room for one more instruction before SCLK = 1
            .endm


SPI_TX  .macro  msg_reg
            LBIT        msg_reg.t7, SPI_MOSI    ; Set output (bit 7)
            SPI_TX_CLK
            LBIT        msg_reg.t6, SPI_MOSI    ; Set output (bit 6)
            SPI_TX_CLK
            LBIT        msg_reg.t5, SPI_MOSI    ; Set output (bit 5)
            SPI_TX_CLK
            LBIT        msg_reg.t4, SPI_MOSI    ; Set output (bit 4)
            SPI_TX_CLK
            LBIT        msg_reg.t3, SPI_MOSI    ; Set output (bit 3)
            SPI_TX_CLK
            LBIT        msg_reg.t2, SPI_MOSI    ; Set output (bit 2)
            SPI_TX_CLK
            LBIT        msg_reg.t1, SPI_MOSI    ; Set output (bit 1)
            SPI_TX_CLK
            LBIT        msg_reg.t0, SPI_MOSI    ; Set output (bit 0)
            SPI_TX_CLK
            NOP

            CLR SPI_MOSI                        ; Reset output to low
        .endm


SPI_RX_CLK  .macro
                CLR         SPI_SCLK            ; SCLK = 0
                SPI_WAIT    SPI_SCLK_DELAY
                SET         SPI_SCLK            ; SCLK = 1
                SPI_WAIT    SPI_SCLK_DELAY, 1
                NOP
                ; Room for one more instruction before SCLK = 1
            .endm


SPI_RX      .macro  msg_reg
                SET         SPI_SCLK                ; SCLK = 1
                SPI_WAIT    SPI_SCLK_DELAY, 1
                NOP

                LBIT        SPI_MISO, msg_reg.t7    ; Get input (bit 7)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t6    ; Get input (bit 6)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t5    ; Get input (bit 5)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t4    ; Get input (bit 4)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t3    ; Get input (bit 3)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t2    ; Get input (bit 2)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t1    ; Get input (bit 1)
                SPI_RX_CLK  SPI_CLOCK
                LBIT        SPI_MISO, msg_reg.t0    ; Get input (bit 0)

                CLR         SPI_SCLK                ; SCLK = 0
                SPI_WAIT    SPI_SCLK_DELAY
            .endm
