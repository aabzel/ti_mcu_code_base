#include "board_layout.h"

#include "data_utils.h"
#include "gpio_drv.h"
#include "stm32f4xx_hal.h"
#include "sys_config.h"

#ifndef USE_HAL_DRIVER
#error "that wile only for STM32 MCUs"
#endif /*USE_HAL_DRIVER*/

Pin_t PinTable[]={
    {{PORT_A,0},"WKUP",00},
    {{PORT_A,1},"PA1",00},
    {{PORT_A,2},"CON3.2",00},
    {{PORT_A,3},"CON3.1",00},
    {{PORT_A,4},"",00},
    {{PORT_A,5},"",00},
    {{PORT_A,6},"",00},
    {{PORT_A,7},"",00},
    {{PORT_A,8},"",00},
    {{PORT_A,9},"",00},
    {{PORT_A,10},"",00},
    {{PORT_A,11},"",00},
    {{PORT_A,12},"",00},
    {{PORT_A,13},"",00},
    {{PORT_A,14},"",00},
    {{PORT_A,15},"",00},

    {{PORT_B,0},"",00},
    {{PORT_B,1},"",00},
    {{PORT_B,2},"",00},
    {{PORT_B,3},"",00},
    {{PORT_B,4},"",00},
    {{PORT_B,5},"",00},
    {{PORT_B,6},"",00},
    {{PORT_B,7},"",00},
    {{PORT_B,8},"",00},
    {{PORT_B,9},"",00},
    {{PORT_B,10},"",00},
    {{PORT_B,11},"",00},
    {{PORT_B,12},"",00},
    {{PORT_B,13},"",00},
    {{PORT_B,14},"",00},
    {{PORT_B,15},"",00},

    {{PORT_C,0},"",00},
    {{PORT_C,1},"",00},
    {{PORT_C,2},"",00},
    {{PORT_C,3},"",00},
    {{PORT_C,4},"",00},
    {{PORT_C,5},"",00},
    {{PORT_C,6},"USART6_TX",96,GPIO_MODE_AF_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF8_USART6, GPIO_PIN_SET},
    {{PORT_C,7},"USART6_RX",97,GPIO_MODE_AF_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF8_USART6, GPIO_PIN_SET},
    {{PORT_C,8},"",00},
    {{PORT_C,9},"",00},
    {{PORT_C,10},"",00},
    {{PORT_C,11},"",00},
    {{PORT_C,12},"",00},
    {{PORT_C,13},"LED_G",7,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_LOW, 0, GPIO_PIN_RESET},
    {{PORT_C,14},"",00},
    {{PORT_C,15},"",00},

    {{PORT_D,0},"",00},
    {{PORT_D,1},"",00},
    {{PORT_D,2},"",00},
    {{PORT_D,3},"",00},
    {{PORT_D,4},"",00},
    {{PORT_D,5},"",00},
    {{PORT_D,6},"",00},
    {{PORT_D,7},"",00},
    {{PORT_D,8},"",00},
    {{PORT_D,9},"",00},
    {{PORT_D,10},"",00},
    {{PORT_D,11},"",00},
    {{PORT_D,12},"",00},
    {{PORT_D,13},"",00},
    {{PORT_D,14},"",00},
    {{PORT_D,15},"",00},

    {{PORT_E,0},"",00},
    {{PORT_E,1},"",00},
    {{PORT_E,2},"",00},
    {{PORT_E,3},"",00},
    {{PORT_E,4},"",00},
    {{PORT_E,5},"",00},
    {{PORT_E,6},"",00},
    {{PORT_E,7},"",00},
    {{PORT_E,8},"",00},
    {{PORT_E,9},"",00},
    {{PORT_E,10},"",00},
    {{PORT_E,11},"",00},
    {{PORT_E,12},"",00},
    {{PORT_E,13},"",00},
    {{PORT_E,14},"",00},
    {{PORT_E,15},"",00},  

    {{PORT_F,0},"",00},
    {{PORT_F,1},"",00},
    {{PORT_F,2},"",00},
    {{PORT_F,3},"",00},
    {{PORT_F,4},"",00},
    {{PORT_F,5},"",00},
    {{PORT_F,6},"",00},
    {{PORT_F,7},"",00},
    {{PORT_F,8},"",00},
    {{PORT_F,9},"",00},
    {{PORT_F,10},"",00},
    {{PORT_F,11},"",00},
    {{PORT_F,12},"",00},
    {{PORT_F,13},"",00},
    {{PORT_F,14},"",00},
    {{PORT_F,15},"",00},  

    {{PORT_G,0},"",00},
    {{PORT_G,1},"",00},
    {{PORT_G,2},"",00},
    {{PORT_G,3},"",00},
    {{PORT_G,4},"",00},
    {{PORT_G,5},"",00},
    {{PORT_G,6},"",00},
    {{PORT_G,7},"",00},
    {{PORT_G,8},"",00},
    {{PORT_G,9},"",00},
    {{PORT_G,10},"",00},
    {{PORT_G,11},"",00},
    {{PORT_G,12},"",00},
    {{PORT_G,13},"",00},
    {{PORT_G,14},"",00},
    {{PORT_G,15},"",00},  

    {{PORT_H,0},"",00},
    {{PORT_H,1},"",00},

};


static GPIO_TypeDef *Port2PortPtr(uint8_t port_num){
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

static uint32_t PinNum2PinMask(uint8_t pin_num){
    uint32_t pin_mask=0;
    pin_mask |= (1 << pin_num);
    return pin_mask;
}

static bool gpio_init_one(Pin_t *pinInstance){
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

uint32_t gpio_get_cnt(void){
    return ARRAY_SIZE(PinTable);
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