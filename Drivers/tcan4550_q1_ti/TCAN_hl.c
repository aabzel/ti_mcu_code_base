/*
 * Author: Texas Instruments
 * Date: 4/25/2019
 *
 * Description: A basic version of code to set up and receive a packet.
 *  - This is designed to work with the EVM the BOOSTXL-CANFD-LIN Rev 1.0 Boosterpack
 *  - It assumes TCAN4550 Oscillator of 40 MHz
 *  - Sets CAN arbitration rate at 500 kBaud
 *  - Sets CAN FD data phase for 2 MBaud
 *  - The interrupt pin is used for signal a received message, rather than polling
 *
 *   Pressing S1 will transmit a CAN message. S1 is on the MSP430FR6989 launchpad to the left.
 *
 *  Pinout
 *   - P1.4 SPI Clock / SCLK
 *   - P1.6 MOSI / SDI
 *   - P1.7 MISO / SDO
 *   - P2.5 SPI Chip Select / nCS
 *
 *   - P2.3 MCAN Interrupt 1 / nINT
 *   - Ground wire is important
 *
 *
 *
 * Copyright (c) 2019 Texas Instruments Incorporated.  All rights reserved.
 * Software License Agreement
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "TCAN_hl.h"

#include <stdbool.h>

#include "TCAN4550.h"
#include "tcan4550_drv.h"
#include "log.h"

volatile uint8_t TCAN_Int_Cnt = 0;					// A variable used to keep track of interrupts the MCAN Interrupt pin

/*
 * Configure the TCAN4550
 */
bool init_tcan(void){
    bool res = true;
    set_log_level(CAN, LOG_LEVEL_NOTICE  ) ;
    CanPhy.cur.int_cnt = 0;
    tcan4550_reset();

    TCAN4x5x_Device_ClearSPIERR();                              // Clear any SPI ERR flags that might be set as a result of our pin mux changing during MCU startup

    /* Step one attempt to clear all interrupts */
	TCAN4x5x_Device_Interrupt_Enable dev_ie = {0};				// Initialize to 0 to all bits are set to 0.
	TCAN4x5x_Device_ConfigureInterruptEnable(&dev_ie);	        // Disable all non-MCAN related interrupts for simplicity

	TCAN4x5x_Device_Interrupts dev_ir = {0};					// Setup a new MCAN IR object for easy interrupt checking
	TCAN4x5x_Device_ReadInterrupts(&dev_ir);					// Request that the struct be updated with current DEVICE (not MCAN) interrupt values

	if (dev_ir.PWRON)                                           // If the Power On interrupt flag is set
		TCAN4x5x_Device_ClearInterrupts(&dev_ir);               // Clear it because if it's not cleared within ~4 minutes, it goes to sleep

	/* Configure the CAN bus speeds */
	TCAN4x5x_MCAN_Nominal_Timing_Simple TCANNomTiming = {0};	// 500k arbitration with a 40 MHz crystal ((40E6 / 2) / (32 + 8) = 500E3)
	TCANNomTiming.NominalBitRatePrescaler = 4;
	TCANNomTiming.NominalTqBeforeSamplePoint = 32;
	TCANNomTiming.NominalTqAfterSamplePoint = 8;

	TCAN4x5x_MCAN_Data_Timing_Simple TCANDataTiming = {0};		// 2 Mbps CAN FD with a 40 MHz crystal (40E6 / (15 + 5) = 2E6)
	TCANDataTiming.DataBitRatePrescaler = 1;
	TCANDataTiming.DataTqBeforeSamplePoint = 15;
	TCANDataTiming.DataTqAfterSamplePoint = 5;

	/* Configure the MCAN core settings */
	TCAN4x5x_MCAN_CCCR_Config cccrConfig = {0};					// Remember to initialize to 0, or you'll get random garbage!
	cccrConfig.FDOE = 0;										// CAN FD mode enable
	cccrConfig.BRSE = 0;										// CAN FD Bit rate switch enable
	cccrConfig.DAR = 1;                                         //Automatic Retransmission Disabled

	/* Configure the default CAN packet filtering settings */
	TCAN4x5x_MCAN_Global_Filter_Configuration gfc = {0};
	gfc.RRFE = 1;                                               // Reject remote frames (TCAN4x5x doesn't support this)
	gfc.RRFS = 1;                                               // Reject remote frames (TCAN4x5x doesn't support this)
	gfc.ANFE = TCAN4x5x_GFC_ACCEPT_INTO_RXFIFO0;                // Default behavior if incoming message doesn't match a filter is to accept into RXFIO0 for extended ID messages (29 bit IDs)
	gfc.ANFS = TCAN4x5x_GFC_ACCEPT_INTO_RXFIFO0;                // Default behavior if incoming message doesn't match a filter is to accept into RXFIO0 for standard ID messages (11 bit IDs)

	/* ************************************************************************
	 * In the next configuration block, we will set the MCAN core up to have:
	 *   - 1 SID filter element
	 *   - 1 XID Filter element
	 *   - 5 RX FIFO 0 elements
	 *   - RX FIFO 0 supports data payloads up to 64 bytes
	 *   - RX FIFO 1 and RX Buffer will not have any elements, but we still set their data payload sizes, even though it's not required
	 *   - No TX Event FIFOs
	 *   - 2 Transmit buffers supporting up to 64 bytes of data payload
	 */
	TCAN4x5x_MRAM_Config MRAMConfiguration = {0};
	MRAMConfiguration.SIDNumElements = 1;						// Standard ID number of elements, you MUST have a filter written to MRAM for each element defined
	MRAMConfiguration.XIDNumElements = 1;						// Extended ID number of elements, you MUST have a filter written to MRAM for each element defined
	MRAMConfiguration.Rx0NumElements = 5;						// RX0 Number of elements
	MRAMConfiguration.Rx0ElementSize = MRAM_64_Byte_Data;		// RX0 data payload size
	MRAMConfiguration.Rx1NumElements = 0;						// RX1 number of elements
	MRAMConfiguration.Rx1ElementSize = MRAM_64_Byte_Data;		// RX1 data payload size
	MRAMConfiguration.RxBufNumElements = 0;						// RX buffer number of elements
	MRAMConfiguration.RxBufElementSize = MRAM_64_Byte_Data;		// RX buffer data payload size
	MRAMConfiguration.TxEventFIFONumElements = 0;				// TX Event FIFO number of elements
	MRAMConfiguration.TxBufferNumElements = 2;					// TX buffer number of elements
	MRAMConfiguration.TxBufferElementSize = MRAM_64_Byte_Data;	// TX buffer data payload size


	/* Configure the MCAN core with the settings above, the changes in this block are write protected registers,      *
	 * so it makes the most sense to do them all at once, so we only unlock and lock once                             */

	TCAN4x5x_MCAN_EnableProtectedRegisters();					// Start by making protected registers accessible
	TCAN4x5x_MCAN_ConfigureCCCRRegister(&cccrConfig);			// Enable FD mode and Bit rate switching
	TCAN4x5x_MCAN_ConfigureGlobalFilter(&gfc);                  // Configure the global filter configuration (Default CAN message behavior)
	TCAN4x5x_MCAN_ConfigureNominalTiming_Simple(&TCANNomTiming);// Setup nominal/arbitration bit timing
	TCAN4x5x_MCAN_ConfigureDataTiming_Simple(&TCANDataTiming);	// Setup CAN FD timing
	TCAN4x5x_MRAM_Clear();										// Clear all of MRAM (Writes 0's to all of it)
	TCAN4x5x_MRAM_Configure(&MRAMConfiguration);				// Set up the applicable registers related to MRAM configuration
	TCAN4x5x_MCAN_DisableProtectedRegisters();					// Disable protected write and take device out of INIT mode


	/* Set the interrupts we want to enable for MCAN */
	TCAN4x5x_MCAN_Interrupt_Enable mcan_ie = {0};				// Remember to initialize to 0, or you'll get random garbage!
	mcan_ie.RF0NE = 1;											// RX FIFO 0 new message interrupt enable

	TCAN4x5x_MCAN_ConfigureInterruptEnable(&mcan_ie);			// Enable the appropriate registers


	/* Setup filters, this filter will mark any message with ID 0x055 as a priority message */
	TCAN4x5x_MCAN_SID_Filter SID_ID = {0};
	SID_ID.SFT = TCAN4x5x_SID_SFT_CLASSIC;						// SFT: Standard filter type. Configured as a classic filter
	SID_ID.SFEC = TCAN4x5x_SID_SFEC_PRIORITYSTORERX0;			// Standard filter element configuration, store it in RX fifo 0 as a priority message
	SID_ID.SFID1 = 0x055;										// SFID1 (Classic mode Filter)
	SID_ID.SFID2 = 0x7FF;										// SFID2 (Classic mode Mask)
	TCAN4x5x_MCAN_WriteSIDFilter(0, &SID_ID);					// Write to the MRAM


	/* Store ID 0x12345678 as a priority message */
	TCAN4x5x_MCAN_XID_Filter XID_ID = {0};
	XID_ID.EFT = TCAN4x5x_XID_EFT_CLASSIC;                      // EFT
	XID_ID.EFEC = TCAN4x5x_XID_EFEC_PRIORITYSTORERX0;           // EFEC
	XID_ID.EFID1 = 0x12345678;                                  // EFID1 (Classic mode filter)
	XID_ID.EFID2 = 0x1FFFFFFF;                                  // EFID2 (Classic mode mask)
	TCAN4x5x_MCAN_WriteXIDFilter(0, &XID_ID);                   // Write to the MRAM

	/* Configure the TCAN4550 Non-CAN-related functions */
	TCAN4x5x_DEV_CONFIG devConfig = {0};                        // Remember to initialize to 0, or you'll get random garbage!
	devConfig.SWE_DIS = 0;                                      // Keep Sleep Wake Error Enabled (it's a disable bit, not an enable)
	devConfig.DEVICE_RESET = 0;                                 // Not requesting a software reset
	devConfig.WD_EN = 0;                                        // Watchdog disabled
	devConfig.nWKRQ_CONFIG = 0;                                 // Mirror INH function (default)
	devConfig.INH_DIS = 0;                                      // INH enabled (default)
	devConfig.GPIO1_GPO_CONFIG = TCAN4x5x_DEV_CONFIG_GPO1_MCAN_INT1;    // MCAN nINT 1 (default)
	devConfig.FAIL_SAFE_EN = 0;                                 // Failsafe disabled (default)
	devConfig.GPIO1_CONFIG = TCAN4x5x_DEV_CONFIG_GPIO1_CONFIG_GPO;      // GPIO set as GPO (Default)
	devConfig.WD_ACTION = TCAN4x5x_DEV_CONFIG_WDT_ACTION_nINT;  // Watchdog set an interrupt (default)
	devConfig.WD_BIT_RESET = 0;                                 // Don't reset the watchdog
	devConfig.nWKRQ_VOLTAGE = 0;                                // Set nWKRQ to internal voltage rail (default)
	devConfig.GPO2_CONFIG = TCAN4x5x_DEV_CONFIG_GPO2_NO_ACTION; // GPO2 has no behavior (default)
	devConfig.CLK_REF = 1;                                      // Input crystal is a 40 MHz crystal (default)
	devConfig.WAKE_CONFIG = TCAN4x5x_DEV_CONFIG_WAKE_BOTH_EDGES;// Wake pin can be triggered by either edge (default)
	TCAN4x5x_Device_Configure(&devConfig);                      // Configure the device with the above configuration

	TCAN4x5x_Device_SetMode(TCAN4x5x_DEVICE_MODE_NORMAL);       // Set to normal mode, since configuration is done. This line turns on the transceiver

	TCAN4x5x_MCAN_ClearInterruptsAll();                         // Resets all MCAN interrupts (does NOT include any SPIERR interrupts)
    return res;
}


