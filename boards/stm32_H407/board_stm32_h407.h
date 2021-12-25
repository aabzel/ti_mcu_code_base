#ifndef BOARD_HARVESTER_V1_H
#define BOARD_HARVESTER_V1_H

#define BOARD_NAME "h407"
#define MCU_NAME "STM32F407ZGT6"
#define UART_COUNT 6
#define SPI_COUNT 3

#define APB2_CLOCK_HZ 84000000
#define APB1_CLOCK_HZ 42000000

#define DIO_SX1262_SS (gpio_port_pin2pad(PORT_B,7))
#define DIO_SX1262_BUSY (gpio_port_pin2pad(PORT_B,9))
#define DIO_SX1262_RST (gpio_port_pin2pad(PORT_B,8))
#define DIO_SX1262_INT (gpio_port_pin2pad(PORT_D,0))

#define SYS_FREQ 168000000U
#define RAM_SIZE (192 * K_BYTES)
#define ROM_SIZE ( 1024 * K_BYTES)

#endif /* BOARD_HARVESTER_V1_H  */
