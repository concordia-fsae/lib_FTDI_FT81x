#pragma once

#ifndef __TI_ARM__
# error "EVE_target_TI.h should only be compiled for TI ARM platforms"
#endif

#if defined(__MSP432P401R__)

# include <stdint.h>
# include <ti/devices/msp432p4xx/driverlib/driverlib.h>
# include <ti/devices/msp432p4xx/inc/msp.h>

# define RIVERDI_PORT GPIO_PORT_P1
# define RIVERDI_SIMO BIT6 // P1.6
# define RIVERDI_SOMI BIT7 // P1.7
# define RIVERDI_CLK  BIT5 // P1.5
# define EVE_CS_PORT  GPIO_PORT_P5
# define EVE_CS       GPIO_PIN0 // P5.0
# define EVE_PDN_PORT GPIO_PORT_P5
# define EVE_PDN      GPIO_PIN1 // P5.1

void EVE_SPI_Init(void);

static inline void DELAY_MS(uint16_t val)
{
    uint16_t counter;

    while (val > 0)
    {
        for (counter = 0; counter < 8000; counter++) // ~1ms at 48MHz Core-Clock
        {
            __nop();
        }
        val--;
    }
}

static inline void EVE_pdn_set(void)
{
    //            GPIO_setOutputLowOnPin(EVE_PDN_PORT,EVE_PDN);   /* Power-Down low */
    P5OUT &= ~EVE_PDN; /* Power-Down low */
}

static inline void EVE_pdn_clear(void)
{
    //            GPIO_setOutputHighOnPin(EVE_PDN_PORT,EVE_PDN);   /* Power-Down high */
    P5OUT |= EVE_PDN; /* Power-Down high */
}

static inline void EVE_cs_set(void)
{
    //            GPIO_setOutputLowOnPin(EVE_CS_PORT,EVE_CS);   /* CS low */
    P5OUT &= ~EVE_CS; /* CS low */
}

static inline void EVE_cs_clear(void)
{
    //            GPIO_setOutputHighOnPin(EVE_CS_PORT,EVE_CS);    /* CS high */
    P5OUT |= EVE_CS; /* CS high */
}

static inline void spi_transmit(uint8_t data)
{
    //            SPI_transmitData(EUSCI_B0_BASE, data);
    //            while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

    UCB0TXBUF_SPI = data;
    while (!(UCB0IFG_SPI & UCTXIFG))
        ; /* wait for transmission to complete */
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
    //            SPI_transmitData(EUSCI_B0_BASE, data);
    //            while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
    //            return EUSCI_B_CMSIS(EUSCI_B0_BASE)->RXBUF;

    UCB0TXBUF_SPI = data;
    while (!(UCB0IFG_SPI & UCTXIFG))
        ; /* wait for transmission to complete */
    return UCB0RXBUF_SPI;
}

static inline uint8_t fetch_flash_byte(const uint8_t* data)
{
    return *data;
}

#endif /* __MSP432P401R__ */
