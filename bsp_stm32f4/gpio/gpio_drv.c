#include "gpio_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "board_layout.h"
#include "data_utils.h"
#include "stm32f4xx_hal.h"

uint32_t gpio_int_cnt=0;

bool gpios_init(void) {
      bool res = false;
     GPIO_InitTypeDef GPIO_InitStruct = {0};
   
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
   
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
   
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
   
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
   
    /*Configure GPIO pin : PG6 */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
   
    /*Configure GPIO pins : PD13 PD14 PD15 */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
   
    /*Configure GPIO pin : PB0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      return res;
}

uint32_t PinNum2PinMask(uint8_t pin_num){
    uint32_t pin_mask=0;
    pin_mask |= (1 << pin_num);
    return pin_mask;
}

GPIO_TypeDef *Port2PortPtr(uint8_t port_num){
    GPIO_TypeDef *GpioPort=NULL;
    switch(port_num){
        case  PORT_A:GpioPort=GPIOA; break;
        case PORT_B:GpioPort=GPIOB; break;
        case PORT_C:GpioPort=GPIOC; break;
        case PORT_D:GpioPort=GPIOD; break;
        case PORT_E:GpioPort=GPIOE; break;
        case PORT_F:GpioPort=GPIOF; break;
        case PORT_G:GpioPort=GPIOG; break;
        case PORT_H:GpioPort=GPIOH; break;
        default: break;
    }
    return GpioPort;
}

Port_t PortLetter2PortNum(char port){
    Port_t port_num=PORT_UNDEF;
    switch (port) {
        case 'A':port_num=PORT_A; break;
        case 'B':port_num=PORT_B; break;
        case 'C':port_num=PORT_C; break;
        case 'D':port_num=PORT_D; break;
        case 'E':port_num=PORT_E; break;
        case 'F':port_num=PORT_F; break;
        case 'G':port_num=PORT_G; break;
        case 'H':port_num=PORT_H; break;
    }
    return port_num;
}

bool gpio_get_state(uint8_t pad_num, uint8_t* logic_level) {
    Pad_t pad;
    pad.byte = pad_num;
    GPIO_PinState value = HAL_GPIO_ReadPin(Port2PortPtr(pad.port), 1<<pad.pin);
    (*logic_level) = (uint8_t)value;
    return true;
}

uint32_t gpio_read(uint8_t pad_num) {
    uint32_t ret=0;
    Pad_t pad;
    pad.byte = pad_num;
    GPIO_PinState value = HAL_GPIO_ReadPin(Port2PortPtr(pad.port), 1<<pad.pin);
    ret = (uint32_t)value;
    return ret;
}

uint8_t gpio_port_pin2pad(Port_t port, uint8_t pin){
    Pad_t pad={0};
    pad.port = port;
    pad.pin= pin;
    return pad.byte;
}

bool gpio_set_state(uint8_t pad_num, uint8_t logic_level) {
    Pad_t pad;
    pad.byte = pad_num;
    HAL_GPIO_WritePin(Port2PortPtr(pad.port), 1<<pad.pin, (GPIO_PinState) logic_level);
    return true;
}

bool is_edge_irq_en(uint8_t dio_pin) {
    bool res = false;

    return res;
}

uint8_t get_mcu_pin(uint8_t io_pin) {
    uint8_t mcu_pin = 0;

    return mcu_pin;
}

uint8_t get_aux_num(uint8_t io_pin) {
    uint8_t aux_pin = 0;

    return aux_pin;
}

bool gpio_toggle(uint8_t pad_num) {
    return true;
}

bool gpio_init_one(Pin_t *pinInstance){
    bool res = false;
    if(pinInstance){
        if(0!=pinInstance->mcu_pin){
            GPIO_InitTypeDef GPIO_InitStruct = {0};
            GPIO_TypeDef *GpioPort = Port2PortPtr(pinInstance->pad.port);
            /*Configure GPIO pin Output Level */
            HAL_GPIO_WritePin(GpioPort, PinNum2PinMask(pinInstance->pad.pin), pinInstance->pin_state);

            /*Configure GPIO pin : PtPin */
            GPIO_InitStruct.Pin = PinNum2PinMask(pinInstance->pad.pin);
            GPIO_InitStruct.Mode = pinInstance->mode;
            GPIO_InitStruct.Pull = pinInstance->pull;
            GPIO_InitStruct.Speed = pinInstance->speed;
            GPIO_InitStruct.Alternate = pinInstance->alternate;
            HAL_GPIO_Init(GpioPort, &GPIO_InitStruct);
        }
        res = true;
    }
    return res;
}


static bool gpio_init_clock(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    return true;
}

bool gpio_init(void){
    bool res = false;
    gpio_init_clock();
    gpio_int_cnt=0;
    uint32_t i=0, cnt_ok=0;
    for(i=0;i<gpio_get_cnt();i++){
        res = gpio_init_one(&PinTable[i]);
        if(res){
            cnt_ok++;
        }
    }
    if(gpio_get_cnt()==cnt_ok){
        res = true;
    }
    return res;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(1==GPIO_Pin){
      gpio_int_cnt++;
  }
}
