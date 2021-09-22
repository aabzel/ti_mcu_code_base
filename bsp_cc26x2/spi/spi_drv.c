#include "spi_drv.h"

#include <ssi.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26X2DMA.h>

#include "bit_utils.h"
#include "clocks.h"
#include "gpio_drv.h"
#include "sys_config.h"

SpiInstance_t SpiInstance[SPI_CNT] = {{
                                          .SpiHandle = NULL,
                                          .rx_byte_cnt = 0,
                                          .tx_byte_cnt = 0,
                                          .base_addr = SSI0_BASE,
                                          .init_done = false,
                                      },
                                      {
                                          .SpiHandle = NULL,
                                          .rx_byte_cnt = 0,
                                          .tx_byte_cnt = 0,
                                          .base_addr = SSI1_BASE,
                                          .name = "unued",
                                          .init_done = false,
                                      }};

const uint_least8_t SPI_count = SPI_CNT;

SPICC26X2DMA_Object spiCC26X2DMAObjects[SPI_CNT];

UDMACC26XX_Object udmaObjects[1];

const UDMACC26XX_HWAttrs udmaHWAttrs[1] = {
    {.baseAddr = UDMA0_BASE, .powerMngrId = PowerCC26XX_PERIPH_UDMA, .intNum = INT_DMA_ERR, .intPriority = ~0}};
const UDMACC26XX_Config UDMACC26XX_config[1] = {
    {.object = &udmaObjects[0], .hwAttrs = &udmaHWAttrs[0]},
};

/*
 * NOTE: The SPI instances below can be used by the SD driver to communicate
 * with a SD card via SPI.  The 'defaultTxBufValue' fields below are set to 0xFF
 * to satisfy the SDSPI driver requirement.
 */
const SPICC26X2DMA_HWAttrs spiCC26X2DMAHWAttrs[SPI_CNT] = {{.baseAddr = SSI0_BASE,
                                                            .intNum = INT_SSI0_COMB,
                                                            .intPriority = ~0,
                                                            .swiPriority = 0,
                                                            .powerMngrId = PowerCC26XX_PERIPH_SSI0,
                                                            .defaultTxBufValue = 0xFF,
                                                            .rxChannelBitMask = 1 << UDMA_CHAN_SSI0_RX,
                                                            .txChannelBitMask = 1 << UDMA_CHAN_SSI0_TX,
                                                            .mosiPin = DIO_SPI0_MOSI,
                                                            .misoPin = DIO_SPI0_MISO,
                                                            .clkPin = DIO_SPI0_SCLK,
                                                            .csnPin = PIN_UNASSIGNED,
                                                            .minDmaTransferSize = 10},
                                                           {.baseAddr = SSI1_BASE,
                                                            .intNum = INT_SSI1_COMB,
                                                            .intPriority = ~0,
                                                            .swiPriority = 0,
                                                            .powerMngrId = PowerCC26XX_PERIPH_SSI1,
                                                            .defaultTxBufValue = 0xFF,
                                                            .rxChannelBitMask = 1 << UDMA_CHAN_SSI1_RX,
                                                            .txChannelBitMask = 1 << UDMA_CHAN_SSI1_TX,
                                                            .mosiPin = PIN_UNASSIGNED,
                                                            .misoPin = PIN_UNASSIGNED,
                                                            .clkPin = PIN_UNASSIGNED,
                                                            .csnPin = PIN_UNASSIGNED,
                                                            .minDmaTransferSize = 10}};

const SPI_Config SPI_config[SPI_CNT] = {
    {.fxnTablePtr = &SPICC26X2DMA_fxnTable,
     .object = &spiCC26X2DMAObjects[SPI0_INX],
     .hwAttrs = &spiCC26X2DMAHWAttrs[SPI0_INX]},
    {.fxnTablePtr = &SPICC26X2DMA_fxnTable,
     .object = &spiCC26X2DMAObjects[SPI1_INX],
     .hwAttrs = &spiCC26X2DMAHWAttrs[SPI1_INX]},
};

#ifdef HAS_SPI_INT
static void SPI0_CallBack(SPI_Handle handle, SPI_Transaction* objTransaction) { SpiInstance[0].it_cnt++; }
static void SPI1_CallBack(SPI_Handle handle, SPI_Transaction* objTransaction) { SpiInstance[1].it_cnt++; }

static SPI_CallbackFxn spiCallbackFunctions[SPI_CNT] = {
    SPI0_CallBack,
    SPI1_CallBack,
};

#endif

static bool spi_init_ll(SpiName_t spi_num, char* spi_name, uint32_t bit_rate, SPI_CallbackFxn transferCallbackFxn) {
    bool res = false;
    SpiInstance[spi_num].init_done = false;
    SPI_init();
    SPI_Params_init(&SpiInstance[spi_num].SpiParams);
    strncpy(SpiInstance[spi_num].name, spi_name, SPI_NAME_SZ_BYTE);
    SpiInstance[spi_num].rx_byte_cnt = 0;
    SpiInstance[spi_num].tx_byte_cnt = 0;

    SpiInstance[spi_num].SpiParams.bitRate = bit_rate;
    SpiInstance[spi_num].SpiParams.dataSize = 8; //(bits)
    SpiInstance[spi_num].SpiParams.frameFormat = SPI_POL0_PHA0;
    SpiInstance[spi_num].SpiParams.mode = SPI_MASTER;
    SpiInstance[spi_num].SpiParams.transferMode = SPI_MODE_BLOCKING;
    SpiInstance[spi_num].SpiParams.transferCallbackFxn = NULL;
#ifdef HAS_SPI_INT
    SpiInstance[spi_num].SpiParams.transferCallbackFxn = spiCallbackFunctions[spi_num];
    SpiInstance[spi_num].SpiParams.transferMode = SPI_MODE_CALLBACK;
#endif
    SpiInstance[spi_num].SpiParams.transferTimeout = SPI_WAIT_FOREVER;

    SpiInstance[spi_num].SpiHandle = SPI_open(spi_num, &SpiInstance[spi_num].SpiParams);
    if(SpiInstance[spi_num].SpiHandle) {
        res = true;
        SpiInstance[spi_num].init_done = true;
#ifdef INIT_SPI_SEND
        uint8_t tx_buff[4] = {0x55, 0xaa, 0x55, 0xaa};
        res = spi_write(index, tx_buff, 4) && res;
#endif
    }
    return res;
}

bool spi_init(void) {
    bool res = true;
#ifdef HAS_SPI0
    res = spi_init_ll(SPI0_INX, "sx1262", SPI0_BIT_RATE_HZ, NULL) && res;
#endif /*HAS_SPI1*/
#ifdef HAS_SPI1
    res = spi_init_ll(SPI1_INX, "unused", SPI1_BIT_RATE_HZ, NULL) && res;
#endif /*HAS_SPI1*/
    return res;
}

static bool spi_wait_tx(SpiName_t spi_num, uint32_t init_it_cnt) {
    bool res = false;
    uint32_t cnt = 0;
    while(1) {
        if(init_it_cnt < SpiInstance[spi_num].it_cnt) {
            res = true;
            break;
        }
        cnt++;
        if(100000000 < cnt) {
            res = false;
            break;
        }
    }
    return res;
}

bool spi_write(SpiName_t spi_num, const uint8_t* const tx_array, uint16_t tx_array_len) {
    bool res = false;
    SPI_Transaction masterTransaction;
    uint32_t init_it_cnt = 0;
    masterTransaction.arg = NULL;
    masterTransaction.count = tx_array_len;
    masterTransaction.rxBuf = NULL;
    masterTransaction.txBuf = (void*)tx_array;

    if(spi_num < SPI_CNT) {
        init_it_cnt = SpiInstance[spi_num].it_cnt;
    }
    switch(spi_num) {
    case 0:
        res = SPI_transfer(SpiInstance[0].SpiHandle, &masterTransaction);
        break;
    case 1:
        res = SPI_transfer(SpiInstance[1].SpiHandle, &masterTransaction);
        break;
    default:
        res = false;
        break;
    }
    if(res) {
        res = spi_wait_tx(spi_num, init_it_cnt);
        if(res) {
            SpiInstance[spi_num].tx_byte_cnt += tx_array_len;
        }
    }
    return res;
}

bool spi_read(SpiName_t spi_num, uint8_t* rx_array, uint16_t rx_array_len) {
    bool res = false;
    SPI_Transaction masterTransaction;
    uint32_t init_it_cnt = 0;

    masterTransaction.arg = NULL;
    masterTransaction.count = rx_array_len;
    masterTransaction.rxBuf = (void*)rx_array;
    masterTransaction.txBuf = NULL;
    if(spi_num < SPI_CNT) {
        init_it_cnt = SpiInstance[spi_num].it_cnt;
    }

    switch(spi_num) {
    case SPI0_INX:
        res = SPI_transfer(SpiInstance[0].SpiHandle, &masterTransaction);
        break;
    case SPI1_INX:
        res = SPI_transfer(SpiInstance[1].SpiHandle, &masterTransaction);
        break;
    default:
        res = false;
        break;
    }
    if(res) {
        res = spi_wait_tx(spi_num, init_it_cnt);
        if(res) {
            SpiInstance[spi_num].rx_byte_cnt += rx_array_len;
        }
    }

    return res;
}

uint32_t spi_get_clock(SpiName_t spi_num) {
    uint32_t spi_bit_rate = 0xFF;
    if(SpiInstance[spi_num].init_done) {
        uint32_t control_0_reg = 0;
        uint32_t clock_prescale_reg = 0;
        clock_prescale_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_CPSR);
        control_0_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_CR0);
        uint32_t scr = extract_subval_from_32bit(control_0_reg, 15, 8);
        clock_prescale_reg = MASK_8BIT & clock_prescale_reg;
        spi_bit_rate = SYS_FREQ / (clock_prescale_reg * (1 + scr));
    }
    return spi_bit_rate;
}

uint8_t spi_get_phase(SpiName_t spi_num) {
    uint32_t phase = 9;
    if(SpiInstance[spi_num].init_done) {
        uint32_t control_0_reg = 0;
        control_0_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_CR0);
        phase = extract_subval_from_32bit(control_0_reg, 7, 7);
    }
    return phase;
}

uint8_t spi_get_polarity(SpiName_t spi_num) {
    uint32_t polarity = 9;
    if(SpiInstance[spi_num].init_done) {
        uint32_t control_0_reg = 0;
        control_0_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_CR0);
        polarity = extract_subval_from_32bit(control_0_reg, 6, 6);
    }
    return polarity;
}

uint8_t spi_get_data_size(SpiName_t spi_num) {
    uint32_t data_size = 0xFF;
    if(SpiInstance[spi_num].init_done) {
        uint32_t control_0_reg = 0;
        control_0_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_CR0);
        data_size = extract_subval_from_32bit(control_0_reg, 3, 0) + 1;
    }
    return data_size;
}

uint8_t spi_get_transmit_int(SpiName_t spi_num) {
    uint8_t val = 0xFF;

    if(SpiInstance[spi_num].init_done) {
        uint32_t masked_interrupt_status_reg;
        masked_interrupt_status_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_RIS);
        val = GET_BIT_NUM(masked_interrupt_status_reg, SSI_IMSC_TXIM_BITN);
    }
    return val;
}

uint8_t spi_get_receive_int(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    if(SpiInstance[spi_num].init_done) {
        uint32_t masked_interrupt_status_reg;
        masked_interrupt_status_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_RIS);
        val = GET_BIT_NUM(masked_interrupt_status_reg, SSI_IMSC_RXIM_BITN);
    }
    return val;
}

uint8_t spi_get_receive_timeout_interrupt(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    if(SpiInstance[spi_num].init_done) {
        uint32_t masked_interrupt_status_reg;
        masked_interrupt_status_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_RIS);
        val = GET_BIT_NUM(masked_interrupt_status_reg, SSI_IMSC_RTIM_BITN);
    }
    return val;
}

uint8_t spi_get_receive_overrun_interrupt(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    if(SpiInstance[spi_num].init_done) {
        uint32_t masked_interrupt_status_reg;
        masked_interrupt_status_reg = HWREG(SpiInstance[spi_num].base_addr + SSI_O_RIS);
        val = GET_BIT_NUM(masked_interrupt_status_reg, SSI_IMSC_RORIM_BITN);
    }
    return val;
}
