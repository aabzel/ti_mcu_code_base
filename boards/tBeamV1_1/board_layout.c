#include "board_layout.h"

#include <stdlib.h>

#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[DIO_CNT] = {
		{.dio=-1, .mcu_pin=1, .name="3V3"},
		{.dio=-1, .mcu_pin=2, .name="LNA_IN"},
		{.dio=-1, .mcu_pin=3, .name="VDD3P3"},
		{.dio=-1, .mcu_pin=4, .name="VDD3P3"},
		{.dio=36, .mcu_pin=5, .name="IO36"},
		{.dio=37, .mcu_pin=6, .name="IO37"},
		{.dio=38, .mcu_pin=7, .name="IO38"},
		{.dio=39, .mcu_pin=8, .name="IO39"},
		{.dio=-1, .mcu_pin=9, .name="RST/EN"},
		{.dio=34, .mcu_pin=10,.name="TXD1"},
		{.dio=35, .mcu_pin=11,.name="IO35"},
		{.dio=32, .mcu_pin=12,.name="LoRaIo2"},
		{.dio=33, .mcu_pin=13,.name="LoRaIo1"},
		{.dio=25, .mcu_pin=14,.name="IO25"},
		{.dio=26, .mcu_pin=15,.name="LoRaIo0"},
		{.dio=27, .mcu_pin=16,.name="MOSI/SDI"},
		{.dio=14, .mcu_pin=17,.name="IO14"},
		{.dio=12, .mcu_pin=18,.name="RXD1"},
		{.dio=-1, .mcu_pin=19,.name="3V3"},
		{.dio=13, .mcu_pin=20,.name="IO13"},
		{.dio=15, .mcu_pin=21,.name="IO15"},
		{.dio=2,  .mcu_pin=22,.name="IO2"},
		{.dio=0,  .mcu_pin=23,.name="IO0"},
		{.dio=4,  .mcu_pin=24,.name="IO4"},
		{.dio=16, .mcu_pin=25,.name="GPIO16"},
		{.dio=-1, .mcu_pin=26,.name="VDD_SDIO"},
		{.dio=17, .mcu_pin=27,.name="GPIO17"},
		{.dio=9,  .mcu_pin=28,.name="SHD/SD2"},
		{.dio=10, .mcu_pin=29,.name="SWP/SD3"},
		{.dio=11, .mcu_pin=30,.name="SCS/CMD"},
		{.dio=6,  .mcu_pin=31,.name="SCK/CLK"},
		{.dio=7,  .mcu_pin=32,.name="SDO/SD0"},
		{.dio=8,  .mcu_pin=33,.name="SDI/SD1"},
		{.dio=5,  .mcu_pin=34,.name="SCK"},
		{.dio=18, .mcu_pin=35,.name="LoRaSS"},
		{.dio=23, .mcu_pin=36,.name="LoRaRst"},
		{.dio=-1, .mcu_pin=37,.name="3V3"},
		{.dio=19, .mcu_pin=38,.name="MISO/SDO"},
		{.dio=22, .mcu_pin=39,.name="IO22"},
		{.dio=3,  .mcu_pin=40,.name="U0RXD"},
		{.dio=1,  .mcu_pin=41,.name="U0TXD"},
		{.dio=21, .mcu_pin=42,.name="IO21"},
		{.dio=-1, .mcu_pin=43,.name="3V3"},
		{.dio=-1, .mcu_pin=44,.name="XTAL_N"},
		{.dio=-1, .mcu_pin=45,.name="XTAL_P"},
		{.dio=-1, .mcu_pin=46,.name="3V3"},
		{.dio=-1, .mcu_pin=47,.name="CAP2"},
		{.dio=-1, .mcu_pin=48,.name="CAP1"},
		{.dio=-1, .mcu_pin=49,.name="GND"},
};









































