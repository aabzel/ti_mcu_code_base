#include "board_layout.h"

#include <stdlib.h>

#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[DIO_CNT] = {
		{.dir=GPIO_DIR_IN,.dio=-1, .mcu_pin=1, .name="3V3"},
		{.dir=GPIO_DIR_INOUT, .dio=-1, .mcu_pin=2, .name="LNA_IN"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=3, .name="VDD3P3"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=4, .name="VDD3P3"},
		{.dir=GPIO_DIR_IN, .dio=36, .mcu_pin=5, .name="IO36"},
		{.dir=GPIO_DIR_IN, .dio=37, .mcu_pin=6, .name="IO37"},
		{.dir=GPIO_DIR_IN, .dio=38, .mcu_pin=7, .name="IO38"},
		{.dir=GPIO_DIR_IN, .dio=39, .mcu_pin=8, .name="IO39"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=9, .name="RST/EN"},
		{.dir=GPIO_DIR_IN, .dio=34, .mcu_pin=10,.name="TXD1"},
		{.dir=GPIO_DIR_IN, .dio=35, .mcu_pin=11,.name="IO35"},
		{.dir=GPIO_DIR_IN, .dio=32, .mcu_pin=12,.name="LoRaIo2"},
		{.dir=GPIO_DIR_IN, .dio=33, .mcu_pin=13,.name="LoRaIo1"},
		{.dir=GPIO_DIR_INOUT, .dio=25, .mcu_pin=14,.name="IO25"},
		{.dir=GPIO_DIR_IN, .dio=26, .mcu_pin=15,.name="LoRaIo0"},
		{.dir=GPIO_DIR_OUT, .dio=27, .mcu_pin=16,.name="MOSI/SDI"},
		{.dir=GPIO_DIR_INOUT, .dio=14, .mcu_pin=17,.name="IO14"},
		{.dir=GPIO_DIR_OUT, .dio=12, .mcu_pin=18,.name="RXD1"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=19,.name="3V3"},
		{.dir=GPIO_DIR_INOUT, .dio=13, .mcu_pin=20,.name="IO13"},
		{.dir=GPIO_DIR_INOUT, .dio=15, .mcu_pin=21,.name="IO15"},
		{.dir=GPIO_DIR_INOUT, .dio=2,  .mcu_pin=22,.name="IO2"},
		{.dir=GPIO_DIR_INOUT, .dio=0,  .mcu_pin=23,.name="IO0"},
		{.dir=GPIO_DIR_OUT, .dio=4,  .mcu_pin=24,.name="IO4"},/*?*/
		{.dir=GPIO_DIR_OUT, .dio=16, .mcu_pin=25,.name="GPIO16"},
		{.dir=GPIO_DIR_OUT, .dio=-1, .mcu_pin=26,.name="VDD_SDIO"},
		{.dir=GPIO_DIR_OUT, .dio=17, .mcu_pin=27,.name="GPIO17"}, /**/
		{.dir=GPIO_DIR_INOUT, .dio=9,  .mcu_pin=28,.name="SHD/SD2"},
		{.dir=GPIO_DIR_INOUT, .dio=10, .mcu_pin=29,.name="SWP/SD3"},
		{.dir=GPIO_DIR_OUT, .dio=11, .mcu_pin=30,.name="SCS/CMD"},
		{.dir=GPIO_DIR_OUT, .dio=6,  .mcu_pin=31,.name="SCK/CLK"},
		{.dir=GPIO_DIR_INOUT, .dio=7,  .mcu_pin=32,.name="SDO/SD0"},
		{.dir=GPIO_DIR_INOUT, .dio=8,  .mcu_pin=33,.name="SDI/SD1"},
		{.dir=GPIO_DIR_OUT, .dio=5,  .mcu_pin=34,.name="SCK"},
		{.dir=GPIO_DIR_OUT, .dio=18, .mcu_pin=35,.name="LoRaSS"},
		{.dir=GPIO_DIR_OUT, .dio=23, .mcu_pin=36,.name="LoRaRst"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=37,.name="3V3"},
		{.dir=GPIO_DIR_IN, .dio=19, .mcu_pin=38,.name="MISO/SDO"},
		{.dir=GPIO_DIR_OUT, .dio=22, .mcu_pin=39,.name="IO22"},
		{.dir=GPIO_DIR_IN, .dio=3,  .mcu_pin=40,.name="U0RXD"},
		{.dir=GPIO_DIR_OUT, .dio=1,  .mcu_pin=41, .name="U0TXD"},
		{.dir=GPIO_DIR_INOUT, .dio=21, .mcu_pin=42,.name="IO21"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=43,.name="3V3"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=44,.name="XTAL_N"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=45,.name="XTAL_P"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=46,.name="3V3"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=47,.name="CAP2"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=48,.name="CAP1"},
		{.dir=GPIO_DIR_IN, .dio=-1, .mcu_pin=49,.name="GND"},
};









































