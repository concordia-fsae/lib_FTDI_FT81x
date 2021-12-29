#pragma once

#ifndef ARDUINO
# error "EVE_target_Arduino.h should only be compiled for Arduino platforms"
#endif

#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>


#if defined(__AVR__)
//  #if defined (ARDUINO_AVR_UNO)
# include <avr/pgmspace.h>

# define EVE_CS  9
# define EVE_PDN 8

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
    while (!(SPSR & (1 << SPIF)))
        ;
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

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
# if defined(RAMPZ)
    return (pgm_read_byte_far(data));
# else
    return (pgm_read_byte_near(data));
# endif
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#elif defined(ARDUINO_METRO_M4)
# define EVE_CS  9
# define EVE_PDN 8

# define EVE_DMA

# if defined(EVE_DMA)
extern uint32_t          EVE_dma_buffer[1025];
extern volatile uint16_t EVE_dma_buffer_index;
extern volatile uint8_t  EVE_dma_busy;

void EVE_init_dma(void);
void EVE_start_dma_transfer(void);
# endif

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
# if defined(EVE_DMA)
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
# else
    spi_transmit_32(data);
# endif
}

static inline uint8_t spi_receive(uint8_t data)
{
    return SPI.transfer(data);
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#elif defined(ARDUINO_NUCLEO_F446RE)
# include "stm32f4xx_hal.h"
# include "stm32f4xx_ll_spi.h"

# define EVE_CS  9
# define EVE_PDN 8
# define EVE_SPI SPI1

void                     EVE_init_spi(void);

# define EVE_DMA

# if defined(EVE_DMA)
extern uint32_t          EVE_dma_buffer[1025];
extern volatile uint16_t EVE_dma_buffer_index;
extern volatile uint8_t  EVE_dma_busy;

void EVE_init_dma(void);
void EVE_start_dma_transfer(void);
# endif

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
    while (!LL_SPI_IsActiveFlag_TXE(EVE_SPI))
        ;
    while (!LL_SPI_IsActiveFlag_RXNE(EVE_SPI))
        ;
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
# if defined(EVE_DMA)
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
# else
    spi_transmit_32(data);
# endif
}

static inline uint8_t spi_receive(uint8_t data)
{
    //          return SPI.transfer(data);
    LL_SPI_TransmitData8(EVE_SPI, data);
    while (!LL_SPI_IsActiveFlag_TXE(EVE_SPI))
        ;
    while (!LL_SPI_IsActiveFlag_RXNE(EVE_SPI))
        ;
    return LL_SPI_ReceiveData8(EVE_SPI);
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#elif defined(ESP8266)
# define EVE_CS  D2 // D2 on D1 mini
# define EVE_PDN D1 // D1 on D1 mini

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
# if defined(EVE_DMA)
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
# else
    spi_transmit_32(data);
# endif
}

static inline uint8_t spi_receive(uint8_t data)
{
    return SPI.transfer(data);
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#elif defined(ESP32)
/* note: this is using the ESP-IDF driver as the Arduino class and driver does not allow DMA for SPI */
# include "driver/spi_master.h"

# define EVE_CS   13
# define EVE_PDN  12
# define EVE_SCK  18
# define EVE_MISO 19
# define EVE_MOSI 23

# define EVE_DMA

void EVE_init_spi(void);

extern spi_device_handle_t EVE_spi_device;
extern spi_device_handle_t EVE_spi_device_simple;

# if defined(EVE_DMA)
extern uint32_t            EVE_dma_buffer[1025];
extern volatile uint16_t   EVE_dma_buffer_index;
extern volatile uint8_t    EVE_dma_busy;

void EVE_init_dma(void);
void EVE_start_dma_transfer(void);
# endif

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
    spi_transaction_t trans = { 0 };
    trans.length            = 8;
    trans.rxlength          = 0;
    trans.flags             = SPI_TRANS_USE_TXDATA;
    trans.tx_data[0]        = data;
    spi_device_polling_transmit(EVE_spi_device_simple, &trans);
}

static inline void spi_transmit_32(uint32_t data)
{
    spi_transaction_t trans = { 0 };
    trans.length            = 32;
    trans.rxlength          = 0;
    trans.flags             = 0;
    trans.tx_buffer         = &data;
    spi_device_polling_transmit(EVE_spi_device_simple, &trans);
}

/* spi_transmit_burst() is only used for cmd-FIFO commands so it *always* has to transfer 4 bytes */
static inline void spi_transmit_burst(uint32_t data)
{
# if defined(EVE_DMA)
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
# else
    spi_transmit_32(data);
# endif
}

static inline uint8_t spi_receive(uint8_t data)
{
    spi_transaction_t trans = { 0 };
    trans.length            = 8;
    trans.rxlength          = 8;
    trans.flags             = (SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA);
    trans.tx_data[0]        = data;
    spi_device_polling_transmit(EVE_spi_device_simple, &trans);

    return trans.rx_data[0];
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#elif defined(ARDUINO_TEENSY41) || (ARDUINO_TEENSY35) /* note: this is mostly untested */

# define EVE_CS  8
# define EVE_PDN 9

# define EVE_DMA

# if defined(EVE_DMA)
extern uint32_t          EVE_dma_buffer[1025];
extern volatile uint16_t EVE_dma_buffer_index;
extern volatile uint8_t  EVE_dma_busy;

void EVE_init_dma(void);
void EVE_start_dma_transfer(void);
# endif

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
# if defined(EVE_DMA)
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
# else
    spi_transmit_32(data);
# endif
}

static inline uint8_t spi_receive(uint8_t data)
{
    return SPI.transfer(data);
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#elif defined(ARDUINO_BBC_MICROBIT_V2) /* note: this is mostly untested */

# define EVE_CS  12
# define EVE_PDN 9

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

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
#else /* generic functions for other Arduino architectures */
# define EVE_CS  9
# define EVE_PDN 8

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

static inline uint8_t fetch_flash_byte(const uint8_t* data)
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
