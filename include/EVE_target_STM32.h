#pragma once


#ifndef STM32F1
# error "EVE_target_STM32 included for target other than STM32F1. Changes are necessary to make this work"
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_spi.h"

#define DELAY_MS(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#define EVE_SPI      SPI1
#define EVE_CS_PORT  SPI1_NSS_GPIO_Port
#define EVE_CS       SPI1_NSS_Pin

#if EVE_HAS_PDN
# define EVE_PDN_PORT FT_PDN_GPIO_Port
# define EVE_PDN      FT_PDN_Pin
#endif    // EVE_HAS_PDN

#if EVE_DMA
# define EVE_DMA_INSTANCE DMA2
# define EVE_DMA_CHANNEL  3
# define EVE_DMA_STREAM   3

extern uint32_t          EVE_dma_buffer[1025];
extern volatile uint16_t EVE_dma_buffer_index;
extern volatile uint8_t  EVE_dma_busy;

void EVE_init_dma(void);
void EVE_start_dma_transfer(void);
#endif    // EVE_DMA


#if EVE_HAS_PDN
static inline void EVE_pdn_clear(void)
{
    HAL_GPIO_WritePin(EVE_PDN_PORT, EVE_PDN, GPIO_PIN_SET);
}

static inline void EVE_pdn_set(void)
{
    HAL_GPIO_WritePin(EVE_PDN_PORT, EVE_PDN, GPIO_PIN_RESET);
}
#endif    // EVE_HAS_PDN

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
    LL_SPI_TransmitData8(EVE_SPI, data);
    while (!LL_SPI_IsActiveFlag_TXE(EVE_SPI));
    while (!LL_SPI_IsActiveFlag_RXNE(EVE_SPI));
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
#if EVE_DMA
    EVE_dma_buffer[EVE_dma_buffer_index++] = data;
#else
    spi_transmit_32(data);
#endif
}

static inline uint8_t spi_receive(uint8_t data)
{
    LL_SPI_TransmitData8(EVE_SPI, data);
    while (!LL_SPI_IsActiveFlag_TXE(EVE_SPI));
    while (!LL_SPI_IsActiveFlag_RXNE(EVE_SPI));
    return LL_SPI_ReceiveData8(EVE_SPI);
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}
