/*
 * There are a few different define domains:
 *   - REG_MCAN_x: MCAN register address defines
 *   - MCAN_DLC_x: DLC values for the RX and TX FIFO element defines
 *   - REG_SPI_x : SPI Controller register address defines
 *   - REG_DEV_x : TCAN4x5x Device-specific register address defines
 *   - REG_BITS_x: Register bit defines in a similar manner as above.
 *   EX: REG_BITS_MCAN_CCCR_INIT is the hex value corresponding to the REG_MCAN_CCCR register's INIT bit
 *
 */



#ifndef TCAN4550_REG_H
#define TCAN4550_REG_H

// Register Address Sections

#define ADDR_SPI_CONFIG								0x0000
#define ADDR_DEV_CONFIG								0x0800 //Modes of Operation and Pin Configurations
#define ADDR_MCAN									0x1000
#define ADDR_MRAM									0x8000

 
#define ADDR_DEVICE_ID0 0x0000
#define ADDR_DEVICE_ID1 0x0004
#define ADDR_READ_WRITE_TEST 0x0808
#define ADDR_SPI_2_REV 0x0008
#define ADDR_STATUS 0x000C
#define ADDR_CREL 0x1000
#define ADDR_FD_IR  0x1050 //Interrupt Register
#define ADDR_FD_IE 0x1054 //Interrupt Enable
#define ADDR_MRAM  0x8000 //MRAM start address,

/*
 * Device ID and SPI Registers: 	0x0000 Prefix
 */
#define ADDR_DEVICE_ID0							0x0000
#define ADDR_DEVICE_ID1							0x0004
#define ADDR_SPI_REVISION						0x0008
#define ADDR_SPI_STATUS					    	0x000C

/*
 * device configuration registers and Interrupt Flags: 	0x0800 Prefix
 */
//#define ADDR_MODES_AND_PINS						0x0800 //Modes of Operation and Pin Configurations
#define ADDR_TIMESTAMP_PRESCALER		        0x0804 //Timestamp Prescalar
#define ADDR_TEST_REGISTERS						0x0808 //Read and Write Test Registers
#define ADDR_IF									0x0820 //Interrupt Flags
#define ADDR_MIF  							    0x0824 //MCAN Interrupt Flags
#define ADDR_IE									0x0830 //Interrupt Enable


/*
 * MCAN Register Addresses CAN FD Register Set: 	0x1000 Prefix
 */
#define ADDR_MCAN_CREL								0x1000
#define ADDR_MCAN_ENDN								0x1004
#define ADDR_MCAN_CUST								0x1008
#define ADDR_MCAN_DBTP								0x100C //
#define ADDR_MCAN_TEST								0x1010
#define ADDR_MCAN_RWD								0x1014
#define ADDR_MCAN_CCCR								0x1018 // CC Control Register
#define ADDR_MCAN_NBTP								0x101C // Nominal Bit Timing & Prescaler Register
#define ADDR_MCAN_TSCC								0x1020
#define ADDR_MCAN_TSCV								0x1024
#define ADDR_MCAN_TOCC								0x1028
#define ADDR_MCAN_TOCV								0x102C
#define ADDR_MCAN_ECR								0x1040
#define ADDR_MCAN_PSR								0x1044
#define ADDR_MCAN_TDCR								0x1048
#define ADDR_MCAN_IR								0x1050 //Interrupt Register
#define ADDR_IR                                     0x1050
#define ADDR_MCAN_IE								0x1054 // Interrupt Enable
#define ADDR_MCAN_ILS								0x1058
#define ADDR_MCAN_ILE								0x105C // Interrupt Line Enable
#define ADDR_MCAN_GFC								0x1080 // Global Filter Configuration
#define ADDR_MCAN_SIDFC								0x1084 // Standard ID Filter Configuration
#define ADDR_MCAN_XIDFC								0x1088 // Extended ID Filter Configuration
#define ADDR_MCAN_XIDAM								0x1090
#define ADDR_MCAN_HPMS								0x1094
#define ADDR_MCAN_NDAT1								0x1098
#define ADDR_MCAN_NDAT2								0x109C
#define ADDR_MCAN_RXF0C								0x10A0 // Rx FIFO 0 Configuration
#define ADDR_MCAN_RXF0S								0x10A4
#define ADDR_MCAN_RXF0A								0x10A8
#define ADDR_MCAN_RXBC								0x10AC //
#define ADDR_MCAN_RXF1C								0x10B0 // Rx FIFO 1 Configuration
#define ADDR_MCAN_RXF1S								0x10B4
#define ADDR_MCAN_RXF1A								0x10B8
#define ADDR_MCAN_RXESC								0x10BC // Rx Buffer/FIFO Element Size Configuration
#define ADDR_MCAN_TXBC								0x10C0 // Tx Buffer Configuration
#define ADDR_MCAN_TXFQS								0x10C4
#define ADDR_MCAN_TXESC								0x10C8 // Tx Buffer Element Size Configuration
#define ADDR_MCAN_TXBRP								0x10CC
#define ADDR_MCAN_TXBAR								0x10D0 // Tx Buffer Add Request
#define ADDR_MCAN_TXBCR								0x10D4
#define ADDR_MCAN_TXBTO								0x10D8
#define ADDR_MCAN_TXBCF								0x10DC
#define ADDR_MCAN_TXBTIE							0x10E0
#define ADDR_MCAN_TXBCIE							0x10E4
#define ADDR_MCAN_TXEFC								0x10F0 // Tx Event FIFO Configuration
#define ADDR_MCAN_TXEFS								0x10F4
#define ADDR_MCAN_TXEFA								0x10F8

#endif /* TCAN4550_REG_H */
