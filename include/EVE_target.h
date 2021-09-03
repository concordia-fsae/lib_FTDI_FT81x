/*
@file    EVE_target.h
@brief   target specific includes, definitions and functions
@version 5.0
@date    2021-06-02
@author  Rudolph Riedel
*/

#ifndef EVE_TARGET_H_
#define EVE_TARGET_H_

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

#include "main.h"
#include "FeatureDefs.h"


#if !defined (ARDUINO)

    #if defined (__IMAGECRAFT__)
        #if defined (_AVR)
            #include <iccioavr.h>

            static inline void DELAY_MS(uint16_t val)
            {
                uint16_t counter;

                while(val > 0)
                {
                    for(counter=0; counter < 2000;counter++) // maybe ~1ms at 16MHz clock
                    {
                        __asm__ volatile ("nop");
                    }
                    val--;
                }
            }

            #define EVE_CS_PORT PORTB
            #define EVE_CS      (1<<PB5)
            #define EVE_PDN_PORT    PORTB
            #define EVE_PDN     (1<<PB4)

            static inline void EVE_pdn_set(void)
            {
                EVE_PDN_PORT &= ~EVE_PDN;   /* Power-Down low */
            }

            static inline void EVE_pdn_clear(void)
            {
                EVE_PDN_PORT |= EVE_PDN;    /* Power-Down high */
            }

            static inline void EVE_cs_set(void)
            {
                EVE_CS_PORT &= ~EVE_CS; /* cs low */
            }

            static inline void EVE_cs_clear(void)
            {
                EVE_CS_PORT |= EVE_CS;  /* cs high */
            }

            static inline void spi_transmit(uint8_t data)
            {
                SPDR = data; /* start transmission */
                while(!(SPSR & (1<<SPIF))); /* wait for transmission to complete - 1us @ 8MHz SPI-Clock */
            }

            static inline void spi_transmit_32(uint32_t data)
            {
                spi_transmit((uint8_t)(data));
                spi_transmit((uint8_t)(data >> 8));
                spi_transmit((uint8_t)(data >> 16));
                spi_transmit((uint8_t)(data >> 24));
            }

            /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
            static inline void spi_transmit_burst(uint32_t data)
            {
                spi_transmit_32(data);
            }

            static inline uint8_t spi_receive(uint8_t data)
            {
                SPDR = data; /* start transmission */
                while(!(SPSR & (1<<SPIF))); /* wait for transmission to complete - 1us @ 8MHz SPI-CLock */
                return SPDR;
            }

            static inline uint8_t fetch_flash_byte(const uint8_t *data)
            {
                return *data;
            }


        #endif
    #endif

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

    #if defined (__GNUC__)

        #if defined (__AVR_XMEGA__)

        #include <avr/io.h>
        #include <avr/pgmspace.h>
        #ifndef F_CPU
            #define F_CPU 32000000UL
        #endif
        #include <util/delay.h>

        #define DELAY_MS(ms) _delay_ms(ms)


        #define EVE_CS_PORT     PORTC
        #define EVE_CS_PIN      PIN4_bm
        #define EVE_PDN_PORT    PORTC
        #define EVE_PDN_PIN     PIN1_bm
        #define EVE_SPI         SPIC        // the used SPI port

        static inline void EVE_pdn_set(void)
        {
            EVE_PDN_PORT.OUTCLR = EVE_PDN_PIN;  /* Power-Down low */
        }

        static inline void EVE_pdn_clear(void)
        {
            EVE_PDN_PORT.OUTSET = EVE_PDN_PIN;  /* Power-Down high */
        }

        static inline void EVE_cs_set(void)
        {
            EVE_CS_PORT.OUTCLR = EVE_CS_PIN;        /* cs low */
        }

        static inline void EVE_cs_clear(void)
        {
            EVE_CS_PORT.OUTSET= EVE_CS_PIN; /* cs high */
        }

        static inline void spi_transmit(uint8_t data)
        {
            EVE_SPI.DATA = data;
            while(!(EVE_SPI.STATUS & 0x80)); // wait for transmit complete
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            spi_transmit_32(data);
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            EVE_SPI.DATA = data;
            while(!(EVE_SPI.STATUS & 0x80)); // wait for transmit complete
            return EVE_SPI.DATA;
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            #if defined (__AVR_HAVE_ELPM__) /* we have an AVR with more than 64kB FLASH memory */
            return(pgm_read_byte_far(data));
            #else
            return(pgm_read_byte_near(data));
            #endif
        }

        #endif /* XMEGA */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (__AVR__) && ! defined (__AVR_XMEGA__)

            #include <avr/io.h>
            #include <avr/pgmspace.h>
            #define F_CPU 16000000UL
            #include <util/delay.h>

            #define DELAY_MS(ms) _delay_ms(ms)

            #define EVE_CS_PORT PORTB
            #define EVE_CS      (1<<PB5)
            #define EVE_PDN_PORT    PORTB
            #define EVE_PDN     (1<<PB4)

            static inline void EVE_pdn_set(void)
            {
                EVE_PDN_PORT &= ~EVE_PDN;   /* Power-Down low */
            }

            static inline void EVE_pdn_clear(void)
            {
                EVE_PDN_PORT |= EVE_PDN;    /* Power-Down high */
            }

            static inline void EVE_cs_set(void)
            {
                EVE_CS_PORT &= ~EVE_CS; /* cs low */
            }

            static inline void EVE_cs_clear(void)
            {
                EVE_CS_PORT |= EVE_CS;  /* cs high */
            }

            static inline void spi_transmit(uint8_t data)
            {
#if 1
                SPDR = data; /* start transmission */
                while(!(SPSR & (1<<SPIF))); /* wait for transmission to complete - 1us @ 8MHz SPI-Clock */
#endif

#if 0
                uint8_t spiIndex  = 0x80;
                uint8_t k;

                for(k = 0; k <8; k++) // Output each bit of spiOutByte
                {
                    if(data & spiIndex) // Output MOSI Bit
                    {
                        PORTC |= (1<<PORTC1);
                    }
                    else
                    {
                        PORTC &= ~(1<<PORTC1);
                    }

                    PORTA |= (1<<PORTA1); // toggle SCK
                    PORTA &= ~(1<<PORTA1);

                    spiIndex >>= 1;
                }
#endif
            }

            static inline void spi_transmit_32(uint32_t data)
            {
                spi_transmit((uint8_t)(data));
                spi_transmit((uint8_t)(data >> 8));
                spi_transmit((uint8_t)(data >> 16));
                spi_transmit((uint8_t)(data >> 24));
            }

            /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
            static inline void spi_transmit_burst(uint32_t data)
            {
                spi_transmit_32(data);
            }

            static inline uint8_t spi_receive(uint8_t data)
            {
#if 1
                SPDR = data; /* start transmission */
                while(!(SPSR & (1<<SPIF))); /* wait for transmission to complete - 1us @ 8MHz SPI-CLock */
                return SPDR;
#endif

#if 0
                uint8_t spiIndex  = 0x80;
                uint8_t spiInByte = 0;
                uint8_t k;

                for(k = 0; k <8; k++) // Output each bit of spiOutByte
                {
                    if(data & spiIndex) // Output MOSI Bit
                    {
                        PORTC |= (1<<PORTC1);
                    }
                    else
                    {
                        PORTC &= ~(1<<PORTC1);
                    }

                    PORTA |= (1<<PORTA1); // toggle SCK
                    PORTA &= ~(1<<PORTA1);

                    if(PINC & (1<<PORTC0))
                    {
                        spiInByte |= spiIndex;
                    }

                    spiIndex >>= 1;
                }
                return spiInByte;
#endif

            }

            static inline uint8_t fetch_flash_byte(const uint8_t *data)
            {
                #if defined (__AVR_HAVE_ELPM__) /* we have an AVR with more than 64kB FLASH memory */
                    return(pgm_read_byte_far(data));
                #else
                    return(pgm_read_byte_near(data));
                #endif
            }

        #endif /* AVR */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (__v851__)

            #include <stdint.h>
            #include "rh850_regs.h"
            #include "os.h"

            #define DELAY_MS(ms)    OS_Wait(ms * 1000)

            static inline void EVE_pdn_set(void)
            {
                P0 &= ~(1u<<6);
            }

            static inline void EVE_pdn_clear(void)
            {
                P0 |= (1u<<6);
            }

            static inline void EVE_cs_set(void)
            {
                P8 &= ~(1u<<2); /* manually set chip-select to low */
            }

            static inline void EVE_cs_clear(void)
            {
                P8 |= (1u<<2);  /* manually set chip-select to high */
            }

            static inline void spi_transmit(uint8_t data)
            {
                CSIH0CTL0 = 0xC1; /* CSIH2PWR = 1;  CSIH2TXE=1; CSIH2RXE = 0; direct access mode  */
                CSIH0TX0H = data;   /* start transmission */
                while(CSIH0STR0 & 0x00080); /* wait for transmission to complete - 800ns @ 10MHz SPI-Clock */
            }

            static inline void spi_transmit_32(uint32_t data)
            {
                spi_transmit((uint8_t)(data));
                spi_transmit((uint8_t)(data >> 8));
                spi_transmit((uint8_t)(data >> 16));
                spi_transmit((uint8_t)(data >> 24));
            }

            /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
            static inline void spi_transmit_burst(uint32_t data)
            {
                spi_transmit_32(data);
            }

            static inline uint8_t spi_receive(uint8_t data)
            {
                CSIH0CTL0 = 0xE1; /* CSIH2PWR = 1;  CSIH2TXE=1; CSIH2RXE = 1; direct access mode  */
                CSIH0TX0H = data;   /* start transmission */
                while(CSIH0STR0 & 0x00080); /* wait for transmission to complete - 800ns @ 10MHz SPI-Clock */
                return (uint8_t) CSIH0RX0H;
            }

            static inline uint8_t fetch_flash_byte(const uint8_t *data)
            {
                return *data;
            }

        #endif /* RH850 */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (__TRICORE__)

            #include "types.h"
            #include "os.h"
            #include "dio.h"
            #include "spi.h"

            #define DELAY_MS(ms) OS_Wait(ms * 1000)

            static inline void EVE_pdn_set(void)
            {
                HW_DIO_SetSync(IO_DIO_DIGOUT_PD_TFT, 0);/* Power-Down low */
            }

            static inline void EVE_pdn_clear(void)
            {
                HW_DIO_SetSync(IO_DIO_DIGOUT_PD_TFT, 1);/* Power-Down high */
            }

            static inline void EVE_cs_set(void)
            {
                HW_DIO_SetSync(IO_DIO_DIGOUT_CS_TFT, 0); /* manually set chip-select to low */
            }

            static inline void EVE_cs_clear(void)
            {
                HW_DIO_SetSync(IO_DIO_DIGOUT_CS_TFT, 1);  /* manually set chip-select to high */
            }

            static inline void spi_transmit(uint8_t data)
            {
                SPI_ReceiveByte(data);
            }

            static inline void spi_transmit_32(uint32_t data)
            {
                spi_transmit((uint8_t)(data));
                spi_transmit((uint8_t)(data >> 8));
                spi_transmit((uint8_t)(data >> 16));
                spi_transmit((uint8_t)(data >> 24));
            }

            /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
            static inline void spi_transmit_burst(uint32_t data)
            {
                spi_transmit_32(data);
            }

            static inline uint8_t spi_receive(uint8_t data)
            {
                return SPI_ReceiveByte(data);
            }

            static inline uint8_t fetch_flash_byte(const uint8_t *data)
            {
                return *data;
            }

        #endif /* __TRICORE__ */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (__SAMC21E18A__) || (__SAMC21J18A__) || (__SAME51J19A__) || (__SAMD51P20A__) || (__SAMD51J19A__) || (__SAMD51G18A__)
        /* note: target as set by AtmelStudio, valid  are all from the same family, ATSAMC2x and ATSAMx5x use the same SERCOM units */

        #include "sam.h"

        #if defined (__SAMC21E18A__) || (__SAMC21J18A__)
        #define EVE_CS_PORT 0
        #define EVE_CS PORT_PA05
        #define EVE_PDN_PORT 0
        #define EVE_PDN PORT_PA03
        #define EVE_SPI SERCOM0
        #define EVE_SPI_DMA_TRIGGER SERCOM0_DMAC_ID_TX
        #define EVE_DMA_CHANNEL 0
        #define EVE_DMA
        #define EVE_DELAY_1MS 8000  /* ~1ms at 48MHz Core-Clock */
        #endif

        #if defined (__SAME51J19A__) || (__SAMD51P20A__) || (__SAMD51J19A__) || (__SAMD51G18A__)
        #define EVE_CS_PORT 1
        #define EVE_CS PORT_PB01
        #define EVE_PDN_PORT 1
        #define EVE_PDN PORT_PB31
        #define EVE_SPI SERCOM5
        #define EVE_SPI_DMA_TRIGGER SERCOM5_DMAC_ID_TX
        #define EVE_DMA_CHANNEL 0
        #define EVE_DMA
        #define EVE_DELAY_1MS 20000 /* ~1ms at 120MHz Core-Clock and activated cache, according to my Logic-Analyzer */
        #endif


        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        void DELAY_MS(uint16_t val);


        static inline void EVE_pdn_set(void)
        {
            PORT->Group[EVE_PDN_PORT].OUTCLR.reg = EVE_PDN;
        }

        static inline void EVE_pdn_clear(void)
        {
            PORT->Group[EVE_PDN_PORT].OUTSET.reg = EVE_PDN;
        }

        static inline void EVE_cs_set(void)
        {
            PORT->Group[EVE_CS_PORT].OUTCLR.reg = EVE_CS;
        }

        static inline void EVE_cs_clear(void)
        {
            PORT->Group[EVE_CS_PORT].OUTSET.reg = EVE_CS;
        }

        static inline void spi_transmit(uint8_t data)
        {
            EVE_SPI->SPI.DATA.reg = data;
            while((EVE_SPI->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_TXC) == 0);
            (void) EVE_SPI->SPI.DATA.reg; /* dummy read-access to clear SERCOM_SPI_INTFLAG_RXC */
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            EVE_SPI->SPI.DATA.reg = data;
            while((EVE_SPI->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_TXC) == 0);
            return EVE_SPI->SPI.DATA.reg;
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

        #endif /* SAMC2x / SAMx5x */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (__riscv)

//      #warning Compiling for GD32VF103CBT6

        #include "gd32vf103.h"

        static inline void DELAY_MS(uint16_t val)
        {
            uint16_t counter;

            while(val > 0)
            {
                for(counter=0; counter < 18000;counter++) /* ~1ms at 108MHz Core-Clock, according to my Logic-Analyzer */
                {
                    __asm__ volatile ("nop");
                }
                val--;
            }
        }

        static inline void EVE_pdn_set(void)
        {
            gpio_bit_reset(GPIOB,GPIO_PIN_1);
        }

        static inline void EVE_pdn_clear(void)
        {
            gpio_bit_set(GPIOB,GPIO_PIN_1);
        }

        static inline void EVE_cs_set(void)
        {
            gpio_bit_reset(GPIOB,GPIO_PIN_0);
        }

        static inline void EVE_cs_clear(void)
        {
            gpio_bit_set(GPIOB,GPIO_PIN_0);
        }

        static inline void spi_transmit(uint8_t data)
        {
                SPI_DATA(SPI0) = (uint32_t) data;
                while(SPI_STAT(SPI0) & SPI_STAT_TRANS);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            spi_transmit_32(data);
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
                SPI_DATA(SPI0) = (uint32_t) data;
                while(SPI_STAT(SPI0) & SPI_STAT_TRANS);
                return (uint8_t) SPI_DATA(SPI0);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

        #endif /* __riscv */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (STM32L073xx) || (STM32F1) || (STM32F207xx) || (STM32F3) || (STM32F4)

        #if defined (STM32L073xx) /* set by PlatformIO board definition file nucleo_l073z.json */
        #include "stm32l0xx.h"
        #include "stm32l0xx_hal.h"
        #include "stm32l0xx_ll_spi.h"
        #endif

        #if defined (STM32F1) /* set by PlatformIO board definition file genericSTM32F103C8.json */
        #include "stm32f1xx.h"
        #include "stm32f1xx_hal.h"
        #include "stm32f1xx_ll_spi.h"
        extern SPI_HandleTypeDef hspi1;
        #endif

        #if defined (STM32F207xx) /* set by PlatformIO board definition file nucleo_f207zg.json */
        #include "stm32f2xx.h"
        #include "stm32f2xx_hal.h"
        #include "stm32f2xx_ll_spi.h"
        #endif

        #if defined (STM32F3) /* set by PlatformIO board definition file genericSTM32F303CB.json */
        #include "stm32f3xx.h"
        #include "stm32f3xx_hal.h"
        #include "stm32f3xx_ll_spi.h"
        #endif

        #if defined (STM32F4) /* set by PlatformIO board definition file genericSTM32F407VET6.json */
        #include "stm32f4xx.h"
        #include "stm32f4xx_hal.h"
        #include "stm32f4xx_ll_spi.h"
        #endif


        #define EVE_SPI SPI1
        #define EVE_CS_PORT SPI1_NSS_GPIO_Port
        #define EVE_CS SPI1_NSS_Pin

#if FTR_HAS_PDN
        #define EVE_PDN_PORT GPIOD
        #define EVE_PDN GPIO_PIN_13
#endif // FTR_HAS_PDN

//      #define EVE_DMA     /* do not activate, it is not working yet */
#if defined (EVE_DMA)
        #define EVE_DMA_INSTANCE DMA2
        #define EVE_DMA_CHANNEL 3
        #define EVE_DMA_STREAM 3
#endif

        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        #define DELAY_MS(ms) HAL_Delay(ms)

#if FTR_HAS_PDN
        static inline void EVE_pdn_clear(void)
        {
            HAL_GPIO_WritePin(EVE_PDN_PORT, EVE_PDN, GPIO_PIN_SET);
        }

        static inline void EVE_pdn_set(void)
        {
            HAL_GPIO_WritePin(EVE_PDN_PORT, EVE_PDN, GPIO_PIN_RESET);
        }
#endif // FTR_HAS_PDN

        static inline void EVE_cs_clear(void)
        {
            HAL_GPIO_WritePin(EVE_CS_PORT, EVE_CS, GPIO_PIN_SET);
        }

        static inline void EVE_cs_set(void)
        {
            HAL_GPIO_WritePin(EVE_CS_PORT, EVE_CS, GPIO_PIN_RESET);
        }

        static inline void spi_transmit(uint8_t data)
        {
            // uint8_t tmp [1];
            // HAL_SPI_Transmit(&hspi1, &data, 1, 50);
            // HAL_SPI_Receive(&hspi1, tmp, 1, 50);
            LL_SPI_TransmitData8(EVE_SPI, data);
            while(!LL_SPI_IsActiveFlag_TXE(EVE_SPI));
            while(!LL_SPI_IsActiveFlag_RXNE(EVE_SPI));
            LL_SPI_ReceiveData8(EVE_SPI); /* dummy read-access to clear SPI_SR_RXNE */
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            // union
            // {
            //     uint8_t a [4];
            //     uint32_t d;
            // } convert;

            // uint8_t tmp [4];
            // HAL_SPI_Transmit(&hspi1, convert.a, 4, 50);
            // HAL_SPI_Receive(&hspi1, tmp, 4, 50);
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            // uint8_t received [1];
            // HAL_SPI_Transmit(&hspi1, &data, 1, 50);
            // HAL_SPI_Receive(&hspi1, received, 1, 50);
            // return received[0];
            LL_SPI_TransmitData8(EVE_SPI, data);
            while(!LL_SPI_IsActiveFlag_TXE(EVE_SPI));
            while(!LL_SPI_IsActiveFlag_RXNE(EVE_SPI));
            return LL_SPI_ReceiveData8(EVE_SPI);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

        #endif  /* STM32 */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (ESP_PLATFORM)

        #include "driver/spi_master.h"
        #include "driver/gpio.h"
        #include "freertos/task.h"

        #define EVE_CS      GPIO_NUM_13
        #define EVE_PDN     GPIO_NUM_12
        #define EVE_SCK     GPIO_NUM_18
        #define EVE_MISO    GPIO_NUM_19
        #define EVE_MOSI    GPIO_NUM_23

        extern spi_device_handle_t EVE_spi_device;
        extern spi_device_handle_t EVE_spi_device_simple;

        #define EVE_DMA

        void DELAY_MS(uint16_t ms);

        void EVE_init_spi(void);

        static inline void EVE_cs_set(void)
        {
            spi_device_acquire_bus(EVE_spi_device_simple, portMAX_DELAY);
            gpio_set_level(EVE_CS, 0);
        }

        static inline void EVE_cs_clear(void)
        {
            gpio_set_level(EVE_CS, 1);
            spi_device_release_bus(EVE_spi_device_simple);
        }

        static inline void EVE_pdn_set(void)
        {
            gpio_set_level(EVE_PDN, 0);
        }

        static inline void EVE_pdn_clear(void)
        {
            gpio_set_level(EVE_PDN, 1);
        }

        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        static inline void spi_transmit(uint8_t data)
        {
            spi_transaction_t trans = {0};
            trans.length = 8;
            trans.rxlength = 0;
            trans.flags = SPI_TRANS_USE_TXDATA;
            trans.tx_data[0] = data;
            spi_device_polling_transmit(EVE_spi_device_simple, &trans);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transaction_t trans = {0};
            trans.length = 32;
            trans.rxlength = 0;
            trans.flags = 0;
            trans.tx_buffer = &data;
            spi_device_polling_transmit(EVE_spi_device_simple, &trans);
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            spi_transaction_t trans = {0};
            trans.length = 8;
            trans.rxlength = 8;
            trans.flags = (SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA);
            trans.tx_data[0] = data;
            spi_device_polling_transmit(EVE_spi_device_simple, &trans);

            return trans.rx_data[0];
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

    #endif /* ESP_PLATFORM */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #if defined (RP2040)
        /* note: set in platformio.ini by "build_flags = -D RP2040" */

        #include <stdio.h>
        #include "pico/stdlib.h"
        #include "hardware/spi.h"

        #define EVE_CS      5
        #define EVE_PDN     6
        #define EVE_SCK     2
        #define EVE_MOSI    3
        #define EVE_MISO    4
        #define EVE_SPI spi0

//      #define EVE_DMA

        #define DELAY_MS(ms) sleep_ms(ms)

//      void EVE_init_spi(void);

        static inline void EVE_cs_set(void)
        {
            gpio_put(EVE_CS, 0);  // active low
        }

        static inline void EVE_cs_clear(void)
        {
            gpio_put(EVE_CS, 1);  // active high
        }

        static inline void EVE_pdn_set(void)
        {
            gpio_put(EVE_PDN, 0);
        }

        static inline void EVE_pdn_clear(void)
        {
            gpio_put(EVE_PDN, 1);
        }

        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        static inline void spi_transmit(uint8_t data)
        {
            spi_write_blocking(EVE_SPI, &data, 1);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_write_blocking(EVE_SPI, (uint8_t *) &data, 4);
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            uint8_t result;

            spi_write_read_blocking(EVE_SPI, &data, &result, 1);
            return result;
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

    #endif /* RP2040 */

    #endif /* __GNUC__ */

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

    #if defined (__TI_ARM__)

        #if defined (__MSP432P401R__)

        #include <ti/devices/msp432p4xx/inc/msp.h>
        #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
        #include <stdint.h>

        #define RIVERDI_PORT GPIO_PORT_P1
        #define RIVERDI_SIMO BIT6   // P1.6
        #define RIVERDI_SOMI BIT7   // P1.7
        #define RIVERDI_CLK BIT5    // P1.5
        #define EVE_CS_PORT         GPIO_PORT_P5
        #define EVE_CS              GPIO_PIN0           //P5.0
        #define EVE_PDN_PORT        GPIO_PORT_P5
        #define EVE_PDN             GPIO_PIN1           //P5.1

        void EVE_SPI_Init(void);

        static inline void DELAY_MS(uint16_t val)
        {
            uint16_t counter;

            while(val > 0)
            {
                for(counter=0; counter < 8000;counter++) // ~1ms at 48MHz Core-Clock
                {
                    __nop();
                }
                val--;
            }
        }

        static inline void EVE_pdn_set(void)
        {
//            GPIO_setOutputLowOnPin(EVE_PDN_PORT,EVE_PDN);   /* Power-Down low */
            P5OUT &= ~EVE_PDN;   /* Power-Down low */
        }

        static inline void EVE_pdn_clear(void)
        {
//            GPIO_setOutputHighOnPin(EVE_PDN_PORT,EVE_PDN);   /* Power-Down high */
            P5OUT |= EVE_PDN;    /* Power-Down high */
        }

        static inline void EVE_cs_set(void)
        {
//            GPIO_setOutputLowOnPin(EVE_CS_PORT,EVE_CS);   /* CS low */
            P5OUT &= ~EVE_CS;   /* CS low */
        }

        static inline void EVE_cs_clear(void)
        {
//            GPIO_setOutputHighOnPin(EVE_CS_PORT,EVE_CS);    /* CS high */
            P5OUT |= EVE_CS;    /* CS high */
        }

        static inline void spi_transmit(uint8_t data)
        {
//            SPI_transmitData(EUSCI_B0_BASE, data);
//            while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

            UCB0TXBUF_SPI = data;
            while(!(UCB0IFG_SPI & UCTXIFG)); /* wait for transmission to complete */
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
//            SPI_transmitData(EUSCI_B0_BASE, data);
//            while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
//            return EUSCI_B_CMSIS(EUSCI_B0_BASE)->RXBUF;

            UCB0TXBUF_SPI = data;
            while(!(UCB0IFG_SPI & UCTXIFG)); /* wait for transmission to complete */
            return UCB0RXBUF_SPI;
         }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

        #endif /* __MSP432P401R__ */

    #endif /* __TI_ARM */
#endif

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#if defined (ARDUINO)

    #include <Arduino.h>
    #include <stdio.h>
    #include <SPI.h>


    #if defined (__AVR__)
//  #if defined (ARDUINO_AVR_UNO)
        #include <avr/pgmspace.h>

        #define EVE_CS      9
        #define EVE_PDN     8

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
//          PORTB &=~(1<<PORTB1); /* directly use pin 9 */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
//          PORTB |=(1<<PORTB1); /* directly use pin 9 */
        }

        static inline void spi_transmit(uint8_t data)
        {
            SPDR = data;
            asm volatile("nop");
            while (!(SPSR & (1<<SPIF)));
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            spi_transmit_32(data);
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            return SPI.transfer(data);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            #if defined(RAMPZ)
                return(pgm_read_byte_far(data));
            #else
                return(pgm_read_byte_near(data));
            #endif
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

    #elif defined (ARDUINO_METRO_M4)
        #define EVE_CS      9
        #define EVE_PDN     8

        #define EVE_DMA

        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
        }

        static inline void spi_transmit(uint8_t data)
        {
            SPI.transfer(data);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            return SPI.transfer(data);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

    #elif defined (ARDUINO_NUCLEO_F446RE)
        #include "stm32f4xx_hal.h"
        #include "stm32f4xx_ll_spi.h"

        #define EVE_CS      9
        #define EVE_PDN     8
        #define EVE_SPI SPI1

        void EVE_init_spi(void);

        #define EVE_DMA

        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
        }

        static inline void spi_transmit(uint8_t data)
        {
//          SPI.transfer(data);
            LL_SPI_TransmitData8(EVE_SPI, data);
            while(!LL_SPI_IsActiveFlag_TXE(EVE_SPI));
            while(!LL_SPI_IsActiveFlag_RXNE(EVE_SPI));
            LL_SPI_ReceiveData8(EVE_SPI); /* dummy read-access to clear SPI_SR_RXNE */
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
//          return SPI.transfer(data);
            LL_SPI_TransmitData8(EVE_SPI, data);
            while(!LL_SPI_IsActiveFlag_TXE(EVE_SPI));
            while(!LL_SPI_IsActiveFlag_RXNE(EVE_SPI));
            return LL_SPI_ReceiveData8(EVE_SPI);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

    #elif defined (ESP8266)
        #define EVE_CS      D2  // D2 on D1 mini
        #define EVE_PDN     D1  // D1 on D1 mini

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
        }

        static inline void spi_transmit(uint8_t data)
        {
            SPI.write(data);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            SPI.write32(__builtin_bswap32(data));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            return SPI.transfer(data);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

    #elif defined (ESP32)
    /* note: this is using the ESP-IDF driver as the Arduino class and driver does not allow DMA for SPI */
        #include "driver/spi_master.h"

        #define EVE_CS      13
        #define EVE_PDN     12
        #define EVE_SCK     18
        #define EVE_MISO    19
        #define EVE_MOSI    23

        #define EVE_DMA

        void EVE_init_spi(void);

        extern spi_device_handle_t EVE_spi_device;
        extern spi_device_handle_t EVE_spi_device_simple;

        #if defined (EVE_DMA)
            extern uint32_t EVE_dma_buffer[1025];
            extern volatile uint16_t EVE_dma_buffer_index;
            extern volatile uint8_t EVE_dma_busy;

            void EVE_init_dma(void);
            void EVE_start_dma_transfer(void);
        #endif

        static inline void EVE_cs_set(void)
        {
            spi_device_acquire_bus(EVE_spi_device_simple, portMAX_DELAY);
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
            spi_device_release_bus(EVE_spi_device_simple);
        }

        static inline void spi_transmit(uint8_t data)
        {
            spi_transaction_t trans = {0};
            trans.length = 8;
            trans.rxlength = 0;
            trans.flags = SPI_TRANS_USE_TXDATA;
            trans.tx_data[0] = data;
            spi_device_polling_transmit(EVE_spi_device_simple, &trans);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transaction_t trans = {0};
            trans.length = 32;
            trans.rxlength = 0;
            trans.flags = 0;
            trans.tx_buffer = &data;
            spi_device_polling_transmit(EVE_spi_device_simple, &trans);
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
                EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
                spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            spi_transaction_t trans = {0};
            trans.length = 8;
            trans.rxlength = 8;
            trans.flags = (SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA);
            trans.tx_data[0] = data;
            spi_device_polling_transmit(EVE_spi_device_simple, &trans);

            return trans.rx_data[0];
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #elif defined (ARDUINO_TEENSY41) || (ARDUINO_TEENSY35) /* note: this is mostly untested */

        #define EVE_CS      8
        #define EVE_PDN     9

        #define EVE_DMA

        #if defined (EVE_DMA)
        extern uint32_t EVE_dma_buffer[1025];
        extern volatile uint16_t EVE_dma_buffer_index;
        extern volatile uint8_t EVE_dma_busy;

        void EVE_init_dma(void);
        void EVE_start_dma_transfer(void);
        #endif

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
        }

        static inline void spi_transmit(uint8_t data)
        {
            SPI.transfer(data);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            #if defined (EVE_DMA)
            EVE_dma_buffer[EVE_dma_buffer_index++] = data;
            #else
            spi_transmit_32(data);
            #endif
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            return SPI.transfer(data);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

        #elif defined (ARDUINO_BBC_MICROBIT_V2) /* note: this is mostly untested */

        #define EVE_CS      12
        #define EVE_PDN     9

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
        }

        static inline void spi_transmit(uint8_t data)
        {
            SPI.transfer(data);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            spi_transmit_32(data);
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            return SPI.transfer(data);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
    #else       /* generic functions for other Arduino architectures */
        #define EVE_CS      9
        #define EVE_PDN     8

        static inline void EVE_cs_set(void)
        {
            digitalWrite(EVE_CS, LOW); /* make EVE listen */
        }

        static inline void EVE_cs_clear(void)
        {
            digitalWrite(EVE_CS, HIGH); /* tell EVE to stop listen */
        }

        static inline void spi_transmit(uint8_t data)
        {
            SPI.transfer(data);
        }

        static inline void spi_transmit_32(uint32_t data)
        {
            spi_transmit((uint8_t)(data));
            spi_transmit((uint8_t)(data >> 8));
            spi_transmit((uint8_t)(data >> 16));
            spi_transmit((uint8_t)(data >> 24));
        }

        /* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
        static inline void spi_transmit_burst(uint32_t data)
        {
            spi_transmit_32(data);
        }

        static inline uint8_t spi_receive(uint8_t data)
        {
            return SPI.transfer(data);
        }

        static inline uint8_t fetch_flash_byte(const uint8_t *data)
        {
            return *data;
        }
    #endif


    /* functions that should be common across Arduino architectures */

    #define DELAY_MS(ms) delay(ms)

    static inline void EVE_pdn_set(void)
    {
        digitalWrite(EVE_PDN, LOW); /* go into power-down */
    }

    static inline void EVE_pdn_clear(void)
    {
        digitalWrite(EVE_PDN, HIGH); /* power up */
    }

#endif /* Arduino */

#endif /* EVE_TARGET_H_ */
