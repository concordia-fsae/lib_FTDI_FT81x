#pragma once

#ifndef ESP_PLATFORM
# error "EVE_target_ESP.h should only be compiled for ESP platforms"
#endif

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/task.h"

#define EVE_CS   GPIO_NUM_13
#define EVE_PDN  GPIO_NUM_12
#define EVE_SCK  GPIO_NUM_18
#define EVE_MISO GPIO_NUM_19
#define EVE_MOSI GPIO_NUM_23

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

#if defined(EVE_DMA)
extern uint32_t          EVE_dma_buffer[1025];
extern volatile uint16_t EVE_dma_buffer_index;
extern volatile uint8_t  EVE_dma_busy;

void EVE_init_dma(void);
void EVE_start_dma_transfer(void);
#endif

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
#if defined(EVE_DMA)
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
#else
    spi_transmit_32(data);
#endif
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
