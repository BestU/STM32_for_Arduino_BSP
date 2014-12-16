/*
  Copyright (c) 2014 BestU. All right reserved.
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#include "WInterrupts.h"
#include "chip.h"

const uint8_t GPIO_IRQn[] = {
	EXTI0_1_IRQn,	//0
	EXTI0_1_IRQn,	//1
	EXTI2_3_IRQn,	//2
	EXTI2_3_IRQn,	//3
	EXTI4_15_IRQn,	//4
	EXTI4_15_IRQn,	//5
	EXTI4_15_IRQn,	//6
	EXTI4_15_IRQn,	//7
	EXTI4_15_IRQn,	//8
	EXTI4_15_IRQn,	//9
	EXTI4_15_IRQn, 	//10
	EXTI4_15_IRQn, 	//11
	EXTI4_15_IRQn, 	//12
	EXTI4_15_IRQn, 	//13
	EXTI4_15_IRQn, 	//14
	EXTI4_15_IRQn  	//15
 };
typedef void (*interruptCB)(void);

static interruptCB callbacksEXTI[16];//EXTI line:0~15


/* Configure PIO interrupt sources */
static void __initialize() {
	int i;
	for (i = 0; i < 16; i++) {
		callbacksEXTI[i] = NULL;
	}

}


void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	static int enabled = 0;
	if (!enabled) {
		__initialize();
		enabled = 1;
	}
	if ( g_APinDescription[pin].pPort == NULL )
	{
		return;
	}
	GPIO_TypeDef *gpio_port = g_APinDescription[pin].pPort;
	uint8_t GPIO_PortSource = 0;
	uint16_t gpio_pin = 1 << (g_APinDescription[pin].ulPin & 0xF);

	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(g_APinDescription[pin].ulPeripheral,ENABLE);

	GPIO_InitStructure.GPIO_Pin = gpio_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(gpio_port, &GPIO_InitStructure);
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	if (gpio_port == GPIOA )
	{
		GPIO_PortSource = EXTI_PortSourceGPIOA;
	}
	else if (gpio_port == GPIOB )
	{
		GPIO_PortSource = EXTI_PortSourceGPIOB;
	}
	else if (gpio_port == GPIOC )
	{
		GPIO_PortSource = EXTI_PortSourceGPIOC;
	}
	else if (gpio_port == GPIOD )
	{
		GPIO_PortSource = EXTI_PortSourceGPIOD;
	}
	else if (gpio_port == GPIOE )
	{
		GPIO_PortSource = EXTI_PortSourceGPIOE;
	}

	/* Connect EXTI Line to GPIO pin */
	SYSCFG_EXTILineConfig(GPIO_PortSource, (g_APinDescription[pin].ulPin & 0xF));
	
	// Register the handler for the user function name
	callbacksEXTI[(g_APinDescription[pin].ulPin & 0xF)] = callback;

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = gpio_pin;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	switch (mode)
	{
		case CHANGE:
			//generate interrupt on rising or falling edge
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			break;
		case RISING:
			//generate interrupt on rising edge
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
			break;
		case FALLING:
			//generate interrupt on falling edge
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
			break;
	}
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn[(g_APinDescription[pin].ulPin & 0xF)];
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void detachInterrupt(uint32_t pin)
{
	uint8_t GPIO_PinSource = 0;	

	uint16_t gpio_pin = 1 << (g_APinDescription[pin].ulPin & 0xF);

	//Clear the pending interrupt flag for that interrupt pin
	EXTI_ClearITPendingBit(gpio_pin);

	//EXTI structure to init EXTI
	EXTI_InitTypeDef EXTI_InitStructure;

	//Select the appropriate EXTI line
	EXTI_InitStructure.EXTI_Line = gpio_pin;
	//disable that EXTI line
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	//send values to registers
	EXTI_Init(&EXTI_InitStructure);

	//unregister the user's handler
	callbacksEXTI[(g_APinDescription[pin].ulPin & 0xF)] = NULL;
}

#ifdef __cplusplus
extern "C" {
#endif



void EXTI0_1_IRQHandler(void)
{
	uint8_t i;
	
	for (i = 0; i < 2; i++)
	{
		if (EXTI_GetITStatus(1 << i) != RESET)
		{
			/* Clear the EXTI line pending bit */
			EXTI_ClearITPendingBit(1 << i);

			if (NULL != callbacksEXTI[i])
			{
				callbacksEXTI[i]();
			}	
		}
	}
}
void EXTI2_3_IRQHandler(void)
{
	uint8_t i;
	
	for (i = 2; i < 4; i++)
	{
		if (EXTI_GetITStatus(1 << i) != RESET)
		{
			/* Clear the EXTI line pending bit */
			EXTI_ClearITPendingBit(1 << i);

			if (NULL != callbacksEXTI[i])
			{
				callbacksEXTI[i]();
			}	
		}
	}
}
void EXTI4_15_IRQHandler(void)
{
	uint8_t i;
	
	for (i = 4; i < 16; i++)
	{
		if (EXTI_GetITStatus(1 << i) != RESET)
		{
			/* Clear the EXTI line pending bit */
			EXTI_ClearITPendingBit(1 << i);

			if (NULL != callbacksEXTI[i])
			{
				callbacksEXTI[i]();
			}	
		}
	}
}


#ifdef __cplusplus
}
#endif
