# ADS131EXX driver for the BeagleBone Black

PRU driver for the ADS131EXX ADC/AFE IC from Texas Instrument

Includes:
  * PRU SPI driver driver (`spi.inc`) that can be used independently (follow steps 1 and 2 below)
  * Main definitions of the ADS131EXX IC (`ads131_defs.inc`)
  * Driver for the ADS131E08 (8 channels version of the IC, `ads131e08.inc`)

## Use
  1. Define `SPI_SCLK`, `SPI_MOSI`, `SPI_MISO`, and `SPI_CS` pins. For instance,
  ```
    SPI_SCLK      .set  5       ; PRU1_5 GPIO2_11 P8_42
    SPI_MOSI      .set  0       ; PRU1_0 GPIO2_6  P8_45
    SPI_MISO      .set  3       ; PRU1_3 GPIO2_9  P8_44
    SPI_CS        .set  7       ; PRU1_7 GPIO2_13 P8_40
  ```

  2. Define `SPI_SCLK_DELAY` to set the SPI clock frequency using the formula `SPI_SCLK_DELAY = floor( (t_SCLK / 5 ns) / 4 )` and `SPI_SCLK_DELAY >= 3`, where `t_SCLK` is the SPI clock period. For instance,
  ```
    SPI_SCLK_DELAY  .set  9
  ```

  3. Define `ADS_START`, `ADS_RESET`, and `ADS_DRDY` pins. For instance,
  ```
    ADS_START     .set  4       ; PRU1_4 GPIO2_10 P8_41
    ADS_RESET     .set  2       ; PRU1_2 GPIO2_8  P8_43
    ADS_DRDY      .set  1       ; PRU1_1 GPIO2_7  P8_46
  ```

  4. Include the `ads131e08.inc`:
  ```
    .include "ads131e08.inc"
  ```

## Functions descriptions

### SPI (API)
  * `SPI_TX`: Transmit byte from `spi_tx_reg`
    * Arguments:
      * `spi_tx_reg` (input, register): register holding the byte to transmit (e.g., `r1.b0`)
    * Side effects:
      * Modifies `r29`
  * `SPI_RX`: Receive byte into `spi_rx_reg`
    * Arguments:
      * `spi_rx_reg` (output, register): register byte in which to record received data (e.g., `r1.b0`)
    * Side effects:
      * Modifies `r29`


### SPI (INTERNAL)
  * `NOP`: No operation (not in the PRU assembly language)
    * Arguments: none
    * Side effects: none
  * `SPI_WAIT`: Pauses for `2*spi_wait_delay - spi_wait_stop_early` instructions (5 ns each)
    * Arguments:
      * `spi_wait_delay` (input, immediate)
      * `spi_wait_stop_early` (input, immediate)
    * Side effects:
      * Modifies `r29`
  * `LBIT`: Load bit `lbit_bit1` of register `lbit_reg1` to bit `lbit_bit2` of register `lbit_reg2`
    * Arguments:
      * `lbit_reg1` (input, register): Source register
      * `lbit_bit1` (input, immediate): Source register bit
      * `lbit_reg2` (output, register): Destination register
      * `lbit_bit2` (output, immediate): Destination register bit
    * Side effects: none
  * `SPI_TX_CLK`: SPI clock pulse during data transmission
    * Arguments: none
    * Side effects:
      * Modifies `r29`
  * `SPI_RX_CLK`: SPI clock pulse during data reception
    * Arguments: none
    * Side effects:
      * Modifies `r29`


### ADS131E08 (API)
  * `ADS_WAIT`: Pauses for `2*ads_wait_n` instructions (5 ns each)
    * Arguments:
      * `ads_wait_n` (input, immediate)
    * Side effects:
      * Modifies `r28`
  * `ADS_STARTUP`: Start up routine of the ADS131 IC (stops conversion, resets IC, stop continuous data conversion)
    * Arguments: none
    * Side effects:
      * Modifies `r28`
  * `ADS_SEND_CMD`: Send command `ads_send_cmd_command` to ADS131
    * Arguments
      * `ads_send_cmd_command` (input, immediate):
    * Side effects:
      * Modifies `r28`
  * `ADS_WRITE_REG`: Write `ads_write_reg_data` to ADS131 register `ads_write_reg_addr`
    * Arguments:
      * `ads_write_reg_addr` (input, immediate): ADS131 register address
      * `ads_write_reg_data` (input, immediate): Data to be written in ADS131 register
    * Side effects:
      * Modifies `r28`
  * `ADS_READ_REG`: Read ADS131 register `ads_read_reg_addr` into `ads_read_reg_data`
    * Arguments:
      * `ads_read_reg_addr` (input, immediate): ADS131 register address
      * `ads_read_reg_data` (output, register byte): Register byte to save ADS131 register data (e.g., `r1.b0`)
    * Side effects:
      * Modifies `r28`
  * `ADS_GET_DATA16`: Receive data packet from ADS131
    * Arguments:
      * `ads_get_data16_reg1` (output, register): Register containing `STATUS` packet (24 bits)
      * `ads_get_data16_reg2` (output, register): Register containing data from channel 1 (`.b3` and `.b2`) and channel 2 (`.b1` and `.b0`)
      * `ads_get_data16_reg3` (output, register): Register containing data from channel 3 (`.b3` and `.b2`) and channel 4 (`.b1` and `.b0`)
      * `ads_get_data16_reg4` (output, register): Register containing data from channel 5 (`.b3` and `.b2`) and channel 6 (`.b1` and `.b0`)
      * `ads_get_data16_reg5` (output, register): Register containing data from channel 7 (`.b3` and `.b2`) and channel 8 (`.b1` and `.b0`)
    * Side effects:
      * Modifies `r28`
  * `ADS_READ_ALL`: Read in all ADS131 registers in a single call
    * Arguments:
      * `ads_read_all_id (output, register)`: Register byte containing ADS131 register `ID`
      * `ads_read_all_config1 (output, register)`: Register byte containing ADS131 register `CONFIG1`
      * `ads_read_all_config2 (output, register)`: Register byte containing ADS131 register `CONFIG2`
      * `ads_read_all_config3 (output, register)`: Register byte containing ADS131 register `CONFIG3`
      * `ads_read_all_fault (output, register)`: Register byte containing ADS131 register `FAULT`
      * `ads_read_all_ch1set (output, register)`: Register byte containing ADS131 register `CH1SET`
      * `ads_read_all_ch2set (output, register)`: Register byte containing ADS131 register `CH2SET`
      * `ads_read_all_ch3set (output, register)`: Register byte containing ADS131 register `CH3SET`
      * `ads_read_all_ch4set (output, register)`: Register byte containing ADS131 register `CH4SET`
      * `ads_read_all_ch5set (output, register)`: Register byte containing ADS131 register `CH5SET`
      * `ads_read_all_ch6set (output, register)`: Register byte containing ADS131 register `CH6SET`
      * `ads_read_all_ch7set (output, register)`: Register byte containing ADS131 register `CH7SET`
      * `ads_read_all_ch8set (output, register)`: Register byte containing ADS131 register `CH8SET`
      * `ads_read_all_fault_statp (output, register)`: Register byte containing ADS131 register `STAT_P`
      * `ads_read_all_fault_statn (output, register)`: Register byte containing ADS131 register `STAT_N`
      * `ads_read_all_gpio (output, register)`: Register byte containing ADS131 register `GPIO`
    * Side effects:
      * Modifies `r28`


### ADS131E08 (INTERNAL)
  * `ADS_INIT`: Initializes the SPI driver interface. To save memory, the ADS131 driver uses jumps (JMP) to call `SPI_RX` and `SPI_TX` as if they were functions instead of using inline code. The input to `SPI_TX` and the output of `SPI_RX` must be saved in `r28.b0` and the return address must be in `r28.w2`
    * Arguments: none
    * Side effects:
      * Modifies `r28`
