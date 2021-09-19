
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "spi_drv.h"
#include "uart_drv.h"

extern TIM_HandleTypeDef htim1;


/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void){
  while (1)  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void){
  while (1)  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void){
  while (1)  {
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
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void){
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RCC global interrupt.
  */
void RCC_IRQHandler(void){
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void){
  HAL_TIM_IRQHandler(&htim1);
}

void SPI1_IRQHandler(void){
  HAL_SPI_IRQHandler(&SpiInstance[1].handle);
}

void UART7_IRQHandler(void){
  HAL_UART_IRQHandler(&huart[CLI_UART_NUM].uart_h);
}
