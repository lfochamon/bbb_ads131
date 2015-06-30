;*************************************************************
; PRUSS driver for the ADI131E08 AFE IC from Texas Instrument
; Designed and tested for the BeagleBone Black
;*************************************************************

; NOTE: Before including this file: define ADI_START, ADI_RESET, and ADI_DRDY.
; WARNING: Most of these macros modify r28 and r29 (spi.h) without warning.

    .if $defined(ADI_START) = 0
        .emsg "ADI_START pin undefined."
    .endif

    .if $defined(ADI_RESET) = 0
        .emsg "ADI_RESET pin undefined."
    .endif

    .if $defined(ADI_DRDY) = 0
        .emsg "ADI_DRDY pin undefined."
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

    .include "adi131_defs.inc"
    .include "spi.inc"

; Wait for 2*[adi_wait_n] instructions: [adi_wait_n]/1e5 ms
ADI_WAIT    .macro  adi_wait_n
                LDI32   r28, adi_wait_n
                SUB     r28, r28, 1
delay?:         SUB     r28, r28, 1
                QBNE    delay?, r28, 0
            .endm

ADI_STARTUP .macro
                ; RESET = 1, START = 0, CS = 1
                SET     r30, r30, ADI_RESET
                CLR     r30, r30, ADI_START
                SET     r30, r30, SPI_CS

                ADI_WAIT    15*1000*1000        ; Wait for 150 ms

                ; Pulse RESET
                CLR         r30, r30, ADI_RESET
                ADI_WAIT    60                  ; Wait for 600 ns
                SET         r30, r30, ADI_RESET

                ADI_WAIT    1000*1000           ; Wait for 10 ms

                ; Stop continuous data conversion mode
                CLR     r30, r30, SPI_CS        ; CS = 0
                LDI     r28.b0, SDATAC
                SPI_TX  r28.b0
            .endm

ADI_WRITE_REG   .macro  adi_write_reg_addr, adi_write_reg_data
                    LDI32   r28, WREG + adi_write_reg_addr + adi_write_reg_data
                    SPI_TX  r28.b2
                    SPI_TX  r28.b1
                    SPI_TX  r28.b0
                .endm

ADI_READ_REG    .macro  adi_read_reg_addr, adi_read_reg_data
                    LDI32   r28, RREG + adi_read_reg_addr
                    SPI_TX  r28.b1
                    SPI_TX  r28.b0
                    SPI_RX  adi_read_reg_data
                .endm

ADI_GET_DATA16  .macro  adi_get_data16_reg1, adi_get_data16_reg2, adi_get_data16_reg3, adi_get_data16_reg4, adi_get_data16_reg5
                    ; Reset STAT register
                    LDI32   adi_get_data16_reg1, 0

                    ; Wait for DRDY = 0
                    WBC     r31, ADI_DRDY

                    CLR     r30, r30, SPI_CS    ; Enable SPI (CS = 0)

                    ; Receive STAT packet (24 bits)
                    SPI_RX  :adi_get_data16_reg1:.b2
                    SPI_RX  :adi_get_data16_reg1:.b1
                    SPI_RX  :adi_get_data16_reg1:.b0

                    ; Receive channel 1 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg2:.b3
                    SPI_RX  :adi_get_data16_reg2:.b2

                    ; Receive channel 2 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg2:.b1
                    SPI_RX  :adi_get_data16_reg2:.b0

                    ; Receive channel 3 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg3:.b3
                    SPI_RX  :adi_get_data16_reg3:.b2

                    ; Receive channel 4 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg3:.b1
                    SPI_RX  :adi_get_data16_reg3:.b0

                    ; Receive channel 5 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg4:.b3
                    SPI_RX  :adi_get_data16_reg4:.b2

                    ; Receive channel 6 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg4:.b1
                    SPI_RX  :adi_get_data16_reg4:.b0

                    ; Receive channel 7 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg5:.b3
                    SPI_RX  :adi_get_data16_reg5:.b2

                    ; Receive channel 8 packet (16 bits)
                    SPI_RX  :adi_get_data16_reg5:.b1
                    SPI_RX  :adi_get_data16_reg5:.b0

                    SET     r30, r30, SPI_CS    ; Disable SPI (CS = 1)
                .endm

ADI_READ_ALL    .macro  adi_read_all_id, adi_read_all_config1, adi_read_all_config2, adi_read_all_config3, adi_read_all_fault, adi_read_all_ch1set, adi_read_all_ch2set, adi_read_all_ch3set, adi_read_all_ch4set, adi_read_all_ch5set, adi_read_all_ch6set, adi_read_all_ch7set, adi_read_all_ch8set, adi_read_all_fault_statp, adi_read_all_fault_statn, adi_read_all_gpio
                    CLR     r30, r30, SPI_CS    ; Enable SPI (CS = 0)

                    ; Read 15 registers starting at ID (0x0)
                    LDI32   r28, RREG + ID + 0xF
                    SPI_TX  r28.b1
                    SPI_TX  r28.b0

                    SPI_RX  adi_read_all_id
                    SPI_RX  adi_read_all_config1
                    SPI_RX  adi_read_all_config2
                    SPI_RX  adi_read_all_config3
                    SPI_RX  adi_read_all_fault
                    SPI_RX  adi_read_all_ch1set
                    SPI_RX  adi_read_all_ch2set
                    SPI_RX  adi_read_all_ch3set
                    SPI_RX  adi_read_all_ch4set
                    SPI_RX  adi_read_all_ch5set
                    SPI_RX  adi_read_all_ch6set
                    SPI_RX  adi_read_all_ch7set
                    SPI_RX  adi_read_all_ch8set
                    SPI_RX  adi_read_all_fault_statp
                    SPI_RX  adi_read_all_fault_statn
                    SPI_RX  adi_read_all_gpio

                    SET     r30, r30, SPI_CS    ; Disable SPI (CS = 1)
                .endm
