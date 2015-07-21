;*************************************************************
; PRUSS driver for the ADS131E08 AFE IC from Texas Instrument
; Designed and tested for the BeagleBone Black
;*************************************************************

; NOTE: Before including this file: define ADS_START, ADS_RESET, and ADS_DRDY.
; WARNING: Most of these macros modify r28 and r29 (spi.h) without warning.

    .if $defined(ADS_START) = 0
        .emsg "ADS_START pin undefined."
    .endif

    .if $defined(ADS_RESET) = 0
        .emsg "ADS_RESET pin undefined."
    .endif

    .if $defined(ADS_DRDY) = 0
        .emsg "ADS_DRDY pin undefined."
    .endif

; 50 ns < t_SCLK < (t_DR – 4 t_CLK) / (N_bits*N_channels + 24)
; Using main clock @ 2.048 MHz:
;   64 kSps: t_SCLK < 89 ns
;   32 kSps: t_SCLK < 192 ns
;   16 kSps: t_SCLK < 280 ns
;    8 kSps: t_SCLK < 569 ns
;    4 kSps: t_SCLK < 1.14 us
;    2 kSps: t_SCLK < 2.3 us
;    1 kSps: t_SCLK < 4.6 us

; WARNING: For CLK @ 2.048 MHz: if the SCLK period is < 62 ns, then SCLK needs
; to be bursty to meet t_SDECODE = 489 ns (not implemented)

    .include "ads131_defs.inc"
    .include "spi.inc"

; Wait for 2*[ads_wait_n] instructions: [ads_wait_n]/1e5 ms
ADS_WAIT    .macro  ads_wait_n
                LDI32   r28, ads_wait_n
                SUB     r28, r28, 1
delay?:         SUB     r28, r28, 1
                QBNE    delay?, r28, 0
            .endm

ADS_STARTUP .macro
                ; RESET = 1, START = 0, CS = 1
                SET             r30, r30, ADS_RESET
                CLR             r30, r30, ADS_START
                SET             r30, r30, SPI_CS

                ADS_WAIT        15*1000*1000        ; Wait for 150 ms

                ; Pulse RESET
                CLR             r30, r30, ADS_RESET
                ADS_WAIT        60                  ; Wait for 600 ns
                SET             r30, r30, ADS_RESET

                ADS_WAIT        1000*1000           ; Wait for 10 ms

                ; Stop continuous data conversion mode
                ADS_SEND_CMD    ADS_SDATAC
            .endm

ADS_SEND_CMD    .macro  ads_send_cmd_command
                    CLR         r30, r30, SPI_CS                ; CS = 0
                    NOP         ; Meet t_CSSC (> 6ns after CS = 0)

                    LDI         r28.b0, ads_send_cmd_command
                    JAL         r28.w2, spi_tx_call             ; Call SPI_TX

                    ; Meet t_SCCS (> 4 t_CLK ~ 1.9 us before CS = 1)
                    ADS_WAIT    200
                    SET         r30, r30, SPI_CS                ; CS = 1

                    ; Meet t_CSH ~ 1 us
                    ADS_WAIT    100
                .endm

ADS_WRITE_REG   .macro  ads_write_reg_addr, ads_write_reg_data
                    CLR         r30, r30, SPI_CS                ; CS = 0
                    NOP         ; Meet t_CSSC (> 6ns after CS = 0)

                    LDI         r28.b0, ADS_WREG + ads_write_reg_addr
                    JAL         r28.w2, spi_tx_call             ; Call SPI_TX
                    LDI         r28.b0, 0
                    JAL         r28.w2, spi_tx_call             ; Call SPI_TX

                    LDI         r28.b0, ads_write_reg_data
                    JAL         r28.w2, spi_tx_call             ; Call SPI_TX

                    ; Meet t_SCCS (> 4 t_CLK ~ 1.9 us before CS = 1)
                    ADS_WAIT    200
                    SET         r30, r30, SPI_CS                ; CS = 1

                    ; Meet t_CSH ~ 1 us
                    ADS_WAIT    100
                .endm

ADS_READ_REG    .macro  ads_read_reg_addr, ads_read_reg_data
                    CLR         r30, r30, SPI_CS                ; CS = 0
                    NOP         ; Meet t_CSSC (> 6ns after CS = 0)

                    LDI         r28.b0, ADS_RREG + ads_read_reg_addr
                    JAL         r28.w2, spi_tx_call             ; Call SPI_TX
                    LDI         r28.b0, 0
                    JAL         r28.w2, spi_tx_call             ; Call SPI_TX

                    JAL         r28.w2, spi_rx_call             ; Call SPI_RX
                    MOV         ads_read_reg_data, r28.b0

                    ; Meet t_SCCS (> 4 t_CLK ~ 1.9 us before CS = 1)
                    ADS_WAIT    200
                    SET         r30, r30, SPI_CS                ; CS = 1

                    ; Meet t_CSH ~ 1 us
                    ADS_WAIT    100
                .endm

ADS_GET_DATA16  .macro  ads_get_data16_reg1, ads_get_data16_reg2, ads_get_data16_reg3, ads_get_data16_reg4, ads_get_data16_reg5
                    ; Reset STAT register
                    LDI32   ads_get_data16_reg1, 0

                    ; Wait for DRDY = 0
                    WBC     r31, ADS_DRDY

                    CLR     r30, r30, SPI_CS                    ; CS = 0
                    NOP     ; Meet t_CSSC (> 6ns after CS = 0)

                    ; Receive STAT packet (24 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg1:.b2, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg1:.b1, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg1:.b0, r28.b0

                    ; Receive channel 1 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg2:.b3, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg2:.b2, r28.b0

                    ; Receive channel 2 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg2:.b1, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg2:.b0, r28.b0

                    ; Receive channel 3 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg3:.b3, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg3:.b2, r28.b0

                    ; Receive channel 4 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg3:.b1, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg3:.b0, r28.b0

                    ; Receive channel 5 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg4:.b3, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg4:.b2, r28.b0

                    ; Receive channel 6 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg4:.b1, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg4:.b0, r28.b0

                    ; Receive channel 7 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg5:.b3, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg5:.b2, r28.b0

                    ; Receive channel 8 packet (16 bits)
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg5:.b1, r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     :ads_get_data16_reg5:.b0, r28.b0

                    ; Meet t_SCCS (> 4 t_CLK ~ 1.9 us before CS = 1)
                    ADS_WAIT    200
                    SET     r30, r30, SPI_CS                    ; CS = 1

                    ; Meet t_CSH ~ 1 us
                    ADS_WAIT    100
                .endm

ADS_READ_ALL    .macro  ads_read_all_id, ads_read_all_config1, ads_read_all_config2, ads_read_all_config3, ads_read_all_fault, ads_read_all_ch1set, ads_read_all_ch2set, ads_read_all_ch3set, ads_read_all_ch4set, ads_read_all_ch5set, ads_read_all_ch6set, ads_read_all_ch7set, ads_read_all_ch8set, ads_read_all_fault_statp, ads_read_all_fault_statn, ads_read_all_gpio
                    CLR     r30, r30, SPI_CS                    ; CS = 0
                    NOP     ; Meet t_CSSC (> 6ns after CS = 0)

                    ; Read 16 registers starting at ID (0x0)
                    LDI     r28.b0, ADS_RREG + ID
                    JAL     r28.w2, spi_tx_call                 ; Call SPI_TX
                    LDI     r28.b0, 0x0F
                    JAL     r28.w2, spi_tx_call                 ; Call SPI_TX

                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_id,            r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_config1,       r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_config2,       r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_config3,       r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_fault,         r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch1set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch2set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch3set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch4set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch5set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch6set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch7set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_ch8set,        r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_fault_statp,   r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_fault_statn,   r28.b0
                    JAL     r28.w2, spi_rx_call                 ; Call SPI_RX
                    MOV     ads_read_all_gpio,          r28.b0

                    ; Meet t_SCCS (> 4 t_CLK ~ 1.9 us before CS = 1)
                    ADS_WAIT    200
                    SET     r30, r30, SPI_CS                    ; CS = 1

                    ; Meet t_CSH ~ 1 us
                    ADS_WAIT    100
                .endm


; Initializes calls to SPI library to save memory.
; Input: r28.b0
; Return address: r28.w2
ADS_INIT        .macro
spi_rx_call:        SPI_RX  r28.b0
                    JMP     r28.w2

spi_tx_call:        SPI_TX  r28.b0
                    JMP     r28.w2
                .endm
