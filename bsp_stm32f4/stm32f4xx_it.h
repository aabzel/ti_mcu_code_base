
#ifndef STM32F4XX_IT_H
#define STM32F4XX_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

void BusFault_Handler(void);
void DebugMon_Handler(void);
void EXTI0_IRQHandler(void);
void HardFault_Handler(void);
void NMI_Handler(void);
void MemManage_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void SPI3_IRQHandler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void SDIO_IRQHandler(void);
void USART6_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32F4XX_IT_H */

