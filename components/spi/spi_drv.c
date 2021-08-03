#include "spi_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26X2DMA.h>
#endif

SPI_Handle spi0Handle = NULL;
SPI_Params SPI0_Params;

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
                                                            .mosiPin = CC26X2R1_LAUNCHXL_SPI0_MOSI,
                                                            .misoPin = CC26X2R1_LAUNCHXL_SPI0_MISO,
                                                            .clkPin = CC26X2R1_LAUNCHXL_SPI0_CLK,
                                                            .csnPin = CC26X2R1_LAUNCHXL_SPI0_CSN,
                                                            .minDmaTransferSize = 10},
                                                           {.baseAddr = SSI1_BASE,
                                                            .intNum = INT_SSI1_COMB,
                                                            .intPriority = ~0,
                                                            .swiPriority = 0,
                                                            .powerMngrId = PowerCC26XX_PERIPH_SSI1,
                                                            .defaultTxBufValue = 0xFF,
                                                            .rxChannelBitMask = 1 << UDMA_CHAN_SSI1_RX,
                                                            .txChannelBitMask = 1 << UDMA_CHAN_SSI1_TX,
                                                            .mosiPin = CC26X2R1_LAUNCHXL_SPI1_MOSI,
                                                            .misoPin = CC26X2R1_LAUNCHXL_SPI1_MISO,
                                                            .clkPin = CC26X2R1_LAUNCHXL_SPI1_CLK,
                                                            .csnPin = CC26X2R1_LAUNCHXL_SPI1_CSN,
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
uint32_t spi_tx_cnt = 0;
static void SPI0_CallBack(SPI_Handle handle, SPI_Transaction* objTransaction) { spi_tx_cnt++; }
#endif

static bool spi0_init(void) {
    bool res = false;
    SPI_init();
    uint8_t tx_buff[4] = {0x55, 0xaa, 0x55, 0xaa};
    SPI_Params_init(&SPI0_Params);

    SPI0_Params.bitRate = SPI0_BIT_RATE_HZ;
    SPI0_Params.dataSize = 8; //(bits)
    SPI0_Params.frameFormat = SPI_POL0_PHA0;
    SPI0_Params.mode = SPI_MASTER;
    SPI0_Params.transferMode = SPI_MODE_BLOCKING;
    SPI0_Params.transferCallbackFxn = NULL;
#ifdef HAS_SPI_INT
    SPI0_Params.transferCallbackFxn = SPI0_CallBack;
    SPI0_Params.transferMode = SPI_MODE_CALLBACK;
#endif
    SPI0_Params.transferTimeout = SPI_WAIT_FOREVER;

    spi0Handle = SPI_open(SPI0_INX, &SPI0_Params);
    if(spi0Handle) {
        res = true;
    }
    res = spi_write(SPI0_INX, tx_buff, 4) && res;
    return res;
}

bool spi_init(void) {
    bool res = true;
    res = spi0_init() && res;
    return res;
}

bool spi_write(uint8_t spi_num, uint8_t* tx_array, uint16_t array_len) {
    bool res = false;
    SPI_Transaction masterTransaction;

    masterTransaction.arg = NULL;
    masterTransaction.count = array_len;
    masterTransaction.rxBuf = NULL;
    masterTransaction.txBuf = (void*)tx_array;

    switch(spi_num) {
    case 0:
        res = SPI_transfer(spi0Handle, &masterTransaction);
        break;
    case 1:
        res = false;
        break;
    default:
        res = false;
        break;
    }
    return res;
}

bool spi_read(uint8_t spi_num, uint8_t* rx_array, uint16_t array_len) {
    bool res = false;
    SPI_Transaction masterTransaction;

    masterTransaction.arg = NULL;
    masterTransaction.count = array_len;
    masterTransaction.rxBuf = (void*)rx_array;
    masterTransaction.txBuf = NULL;

    switch(spi_num) {
    case SPI0_INX:
        res = SPI_transfer(spi0Handle, &masterTransaction);
        break;
    case SPI1_INX:
        res = false;
        break;
    default:
        res = false;
        break;
    }

    return res;
}
