/*
@file    EVE_target.h
@brief   target specific includes, definitions and functions
@version 5.0
@date    2021-06-02
@author  Rudolph Riedel
*/

/* While the following lines make things a lot easier like automatically compiling the code for the target you are compiling for, */
/* a few things are expected to be taken care of beforehand. */
/* - setting the Chip-Select and Power-Down pins to Output, Chip-Select = 1 and Power-Down = 0 */
/* - setting up the SPI which may or not include things like
       - setting the pins for the SPI to output or some alternate I/O function or mapping that functionality to that pin
       - if that is an option with the controller your are using you probably should set the drive-strength for the SPI pins to high
       - setting the SS pin on AVRs to output in case it is not used for Chip-Select or Power-Down
       - setting SPI to mode 0
       - setting SPI to 8 bit with MSB first
       - setting SPI clock to no more than 11 MHz for the init - if the display-module works as high

  For the SPI transfers single 8-Bit transfers are used with busy-wait for completion.
  While this is okay for AVRs that run at 16MHz with the SPI at 8 MHz and therefore do one transfer in 16 clock-cycles,
  this is wasteful for any 32 bit controller even at higher SPI speeds.

  Check out the section for SAMC21E18A as it has code to transparently add DMA.

  If the define "EVE_DMA" is set the spi_transmit_async() is changed at compile time to write in a buffer instead directly to SPI.
  EVE_init() calls EVE_init_dma() which sets up the DMA channel and enables an IRQ for end of DMA.
  EVE_start_cmd_burst() resets the DMA buffer instead of transferring the first bytes by SPI.
  EVE_end_cmd_burst() just calls EVE_start_dma_transfer() which triggers the transfer of the SPI buffer by DMA.
  EVE_cmd_start() just instantly returns if there is an active DMA transfer.
  EVE_busy() does nothing but to report that EVE is busy if there is an active DMA transfer.
  At the end of the DMA transfer an IRQ is executed which clears the DMA active state, calls EVE_cs_clear() and EVE_cmd_start().
*/

#pragma once

#if defined(ARDUINO)
# include "EVE_target_Arduino.h"

#elif defined(__GNUC__)
# if defined(STM32L073xx) || (STM32F1) || (STM32F207xx) || (STM32F3) || (STM32F4)
#  include "EVE_target_STM32.h"
# endif /* STM32 */

# if defined(ESP_PLATFORM)
#  include "EVE_target_ESP.h"
# endif /* ESP_PLATFORM */
#else   // GNUC

# if defined(__TI_ARM__)
#  include "EVE_target_TI.h"
# endif /* __TI_ARM */

#endif // ARDUINO
