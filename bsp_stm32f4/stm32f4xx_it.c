#include "stm32f4xx_it.h"

#include "main.h"
#include "spi_common.h"
#include "sys_tick.h"
#include "uart_common.h"

extern SD_HandleTypeDef hsd;

void NMI_Handler(void){
  while (1)  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void){
  while (1)
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void){
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void){
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void){
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void){

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void){
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void){
}

#ifdef HAS_SPI1
void SPI1_IRQHandler(void) {
    HAL_SPI_IRQHandler(&SpiInstance[0].handle);
}
#endif

#ifdef HAS_SPI2
void SPI2_IRQHandler(void) {
    HAL_SPI_IRQHandler(&SpiInstance[1].handle);
}
#endif

#ifdef HAS_SPI3
void SPI3_IRQHandler(void) {
    HAL_SPI_IRQHandler(&SpiInstance[2].handle);
}
#endif

void EXTI0_IRQHandler(void){
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void){
  HAL_IncTick();
  SysTickIntHandler();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles SDIO global interrupt.
  */
void SDIO_IRQHandler(void){
  HAL_SD_IRQHandler(&hsd);
}

/**
  * @brief This function handles USART6 global interrupt.
  */
#ifdef HAS_USART6
void USART6_IRQHandler(void){
  HAL_UART_IRQHandler(&huart[6-1].uart_h);
}
#endif

