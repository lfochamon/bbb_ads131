;********************************************************
; Defines for the ADS131E08 AFE IC from Texas Instrument
;********************************************************


;*******************************
;***** Registers addresses *****
;*******************************

; Device settings (read-only)
ID          .set    0x00

; Global settings
CONFIG1     .set    0x01
CONFIG2     .set    0x02
CONFIG3     .set    0x03
FAULT       .set    0x04

; Channel settings
CH1SET      .set    0x05
CH2SET      .set    0x06
CH3SET      .set    0x07
CH4SET      .set    0x08
CH5SET      .set    0x09
CH6SET      .set    0x0A
CH7SET      .set    0x0B
CH8SET      .set    0x0C

; Fault detect status registers (read-only)
FAULT_STATP .set    0x12
FAULT_STATN .set    0x13

; GPIOs
GPIO        .set    0x14


;********************************
;**** Registers definitions *****
;********************************

; Global settings
; CONFIG1 (default = 0x91)
CONFIG1_MASK    .set    0x90
DAISY_IN        .set    0x20    ; Enable multiple read back mode
                                ;       (default = 0, daisy-chain)
CLK_EN          .set    0x10    ; Enable oscilator output
                                ;       (default = 0, disabled)
DR_64K          .set    0x0     ; 64 kSps (16 bits)
DR_32K          .set    0x1     ; 32 kSps (16 bits, default)
DR_16K          .set    0x2     ; 16 kSps (24 bits)
DR_08K          .set    0x3     ;  8 kSps (24 bits)
DR_04K          .set    0x4     ;  4 kSps (24 bits)
DR_02K          .set    0x5     ;  2 kSps (24 bits)
DR_01K          .set    0x6     ;  1 kSps (24 bits)

; CONFIG2 (default = 0xE0)
CONFIG2_MASK    .set    0xE0
INT_TEST        .set    0x10    ; Enable internal test signals
                                ;       (default = 0, external)
TEST_AMP0       .set    0x4     ; Test signal amplitude (default = 0, 1x)
TEST_FREQ_21    .set    0x0     ; Test frequency @ fclk / 2^21 (default)
TEST_FREQ_20    .set    0x1     ; Test frequency @ fclk / 2^20
TEST_FREQ_DC    .set    0x3     ; Test frequency @ DC

; CONFIG3 (default = 0x40)
CONFIG3_MASK    .set    0x40
PDB_REFBUF      .set    0x80    ; Enable internal reference buffer
                                ;       (default = 0, disabled)
VREF_4V         .set    0x10    ; Sets VREFP = 4V (default = 0, VREFP = 2.4V)
OPAMP_REF       .set    0x8     ; Non-inverting op amp input to (AVDD + AVSS)/2
                                ;       (default = 0, connected to OPAMPP)
PDB_OPAMP       .set    0x4     ; Enable op amp
                                ;       (default = 0, op amp powered down)

; FAULT (default = 0x00)
COMP_TH_5       .set    0x0     ; Comparator threshold 5% - 95% (default)
COMP_TH_7_5     .set    0x1     ; Comparator threshold 7.5% - 92.5%
COMP_TH_10      .set    0x2     ; Comparator threshold 10% - 90%
COMP_TH_12_5    .set    0x3     ; Comparator threshold 12.5% - 87.5%
COMP_TH_15      .set    0x4     ; Comparator threshold 15% - 85%
COMP_TH_20      .set    0x5     ; Comparator threshold 20% - 80%
COMP_TH_25      .set    0x6     ; Comparator threshold 25% - 75%
COMP_TH_30      .set    0x7     ; Comparator threshold 30% - 70%

; Channel settings
; CHnSET (default = 0x10)
CH_PD           .set    0x80    ; Channel power down
                                ;       (default = 0, normal operation)
CH_GAIN_1       .set    0x10    ; Channel PGA gain 1x (default)
CH_GAIN_2       .set    0x20    ; Channel PGA gain 2x
CH_GAIN_4       .set    0x40    ; Channel PGA gain 4x
CH_GAIN_8       .set    0x50    ; Channel PGA gain 8x
CH_GAIN_12      .set    0x60    ; Channel PGA gain 12x
CH_NORMAL       .set    0x0     ; Channel MUX: normal (default)
CH_SHORTED      .set    0x1     ; Channel MUX: inputs shorted
CH_VDD          .set    0x3     ; Channel MUX: MVDD
CH_TEMP         .set    0x4     ; Channel MUX: temperature sensor
CH_TEST         .set    0x5     ; Channel MUX: test signal

; GPIO
; GPIO (default = 0x0F)
GPIO4           .set    0x80    ; Data for GPIO4
GPIO3           .set    0x40    ; Data for GPIO3
GPIO2           .set    0x20    ; Data for GPIO2
GPIO1           .set    0x10    ; Data for GPIO1
GPIO_CTRL_4     .set    0x8     ; Control for GPIO4
                                ;       (0 = output; 1 = input, default)
GPIO_CTRL_3     .set    0x4     ; Control for GPIO3
                                ;       (0 = output; 1 = input, default)
GPIO_CTRL_2     .set    0x2     ; Control for GPIO2
                                ;       (0 = output; 1 = input, default)
GPIO_CTRL_1     .set    0x1     ; Control for GPIO1
                                ;       (0 = output; 1 = input, default)


;***********************
;**** SPI commands *****
;***********************
; System commands
ADS_CMD_WAKEUP          .set    0x02    ; Wake-up from standby mode
ADS_CMD_STANDBY         .set    0x04    ; Enter standby mode
ADS_CMD_RESET           .set    0x06    ; Reset the device
ADS_CMD_START           .set    0x08    ; Start or restart (synchronize) conversions
ADS_CMD_STOP            .set    0x0A    ; Stop conversion
ADS_CMD_OFFSETCAL       .set    0x1A    ; Channel offset calibration

; Data read commands
ADS_CMD_RDATAC          .set    0x10    ; Enable continuous data read mode
                                    ;       (default at power-up)
ADS_CMD_SDATAC          .set    0x11    ; Stop continuous data read mode
ADS_CMD_RDATA           .set    0x12    ; Read data command

; Register Read Commands
ADS_CMD_RREG            .set    0x20    ; Read registers (0b001r rrrr 0b000n nnnn:
                                        ;       read n+1 registers starting at r)
ADS_CMD_WREG            .set    0x40    ; Write registers (0b010r rrrr 0b000n nnnn:
                                        ;       write n+1 registers starting at r)
