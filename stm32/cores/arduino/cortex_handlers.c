/*
  Copyright (c) 2014 BestU. All right reserved.
  Copyright (c) 2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "Reset.h"

#ifdef __cplusplus
extern "C" {
#endif

static void __halt() {
	// Halts
	while (1)
		;
}

extern void svcHook(void);
extern void pendSVHook(void);
extern int sysTickHook(void);

/* Cortex-M3 core handlers */
void NMI_Handler       (void) __attribute__ ((weak, alias("__halt")));
void HardFault_Handler (void) __attribute__ ((weak, alias("__halt")));
void MemManage_Handler (void) __attribute__ ((weak, alias("__halt")));
void BusFault_Handler  (void) __attribute__ ((weak, alias("__halt")));
void UsageFault_Handler(void) __attribute__ ((weak, alias("__halt")));
void DebugMon_Handler  (void) __attribute__ ((weak, alias("__halt")));
void SVC_Handler       (void) { svcHook(); }
void PendSV_Handler    (void) {	pendSVHook(); }

void SysTick_Handler(void)
{
	if (sysTickHook())
		return;

//	tickReset();

	// Increment tick count each ms
	TimeTick_Increment();
}

/* Peripherals handlers */
void WWDG_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void PVD_IRQHandler                  (void) __attribute__ ((weak, alias("__halt")));
void RTC_IRQHandler                  (void) __attribute__ ((weak, alias("__halt")));
void FLASH_IRQHandler                (void) __attribute__ ((weak, alias("__halt")));
void RCC_IRQHandler                  (void) __attribute__ ((weak, alias("__halt")));
void EXTI0_1_IRQHandler              (void) __attribute__ ((weak, alias("__halt")));
void EXTI2_3_IRQHandler              (void) __attribute__ ((weak, alias("__halt")));
void EXTI4_15_IRQHandler             (void) __attribute__ ((weak, alias("__halt")));
void TS_IRQHandler                   (void) __attribute__ ((weak, alias("__halt")));
void DMA1_Channel1_IRQHandler        (void) __attribute__ ((weak, alias("__halt")));
void DMA1_Channel2_3_IRQHandler      (void) __attribute__ ((weak, alias("__halt")));
void DMA1_Channel4_5_IRQHandler      (void) __attribute__ ((weak, alias("__halt")));
void ADC1_COMP_IRQHandler            (void) __attribute__ ((weak, alias("__halt")));
void TIM1_BRK_UP_TRG_COM_IRQHandler  (void) __attribute__ ((weak, alias("__halt")));
void TIM1_CC_IRQHandler              (void) __attribute__ ((weak, alias("__halt")));
void TIM2_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void TIM3_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void TIM6_DAC_IRQHandler             (void) __attribute__ ((weak, alias("__halt")));
void TIM14_IRQHandler                (void) __attribute__ ((weak, alias("__halt")));
void TIM15_IRQHandler                (void) __attribute__ ((weak, alias("__halt")));
void TIM16_IRQHandler                (void) __attribute__ ((weak, alias("__halt")));
void TIM17_IRQHandler                (void) __attribute__ ((weak, alias("__halt")));
void I2C1_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void I2C2_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void SPI1_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void SPI2_IRQHandler                 (void) __attribute__ ((weak, alias("__halt")));
void USART1_IRQHandler               (void) __attribute__ ((weak, alias("__halt")));
void USART2_IRQHandler               (void) __attribute__ ((weak, alias("__halt")));
void CEC_IRQHandler                  (void) __attribute__ ((weak, alias("__halt")));

#ifdef __cplusplus
}
#endif

