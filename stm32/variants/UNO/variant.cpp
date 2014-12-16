/*
  Copyright (c) 2014 BestU.  All right reserved.
  Copyright (c) 2011 Arduino.  All right reserved.

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

#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
 * Pins descriptions
 */
extern const PinDescription g_APinDescription[]=
{
	// 0/1 - UART (Serial)
	{ GPIOA, 10,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIO_AF_1 }, // 0
	{ GPIOA,  9,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIO_AF_1 }, // 1
	{ GPIOA,  2,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_0 }, // 2
	{ GPIOB,  3,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_2 }, // 3
	{ GPIOB,  5,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_1 }, // 4
	{ GPIOB,  4,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_1 }, // 5
	{ GPIOB, 10,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_2 }, // 6
	{ GPIOA,  8,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_2 }, // 7
	{ GPIOB,  7,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 8
	{ GPIOB,  2,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 9
	{ GPIOB,  6,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_UP    , GPIO_Speed_50MHz,         0 }, // 10
	{ GPIOA,  7,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_DOWN,   GPIO_Speed_50MHz, GPIO_AF_0 }, // 11
	{ GPIOA,  6,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_DOWN,   GPIO_Speed_50MHz, GPIO_AF_0 }, // 12
	{ GPIOA, 11,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 13 
	{ GPIOA,  5,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_DOWN,   GPIO_Speed_50MHz, GPIO_AF_0 }, // 14
	{ GPIOA,  0,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 15
	{ GPIOA,  1,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 16
	{ GPIOA,  4,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 17
	{ GPIOB,  0,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 18
	{ GPIOB,  1,  RCC_AHBPeriph_GPIOB, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 19
	{ GPIOA,  3,  RCC_AHBPeriph_GPIOA, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 20
	{ GPIOB,  9,  RCC_AHBPeriph_GPIOB, GPIO_OType_OD, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_1 }, // 21
	{ GPIOB,  8,  RCC_AHBPeriph_GPIOB, GPIO_OType_OD, GPIO_PuPd_UP,     GPIO_Speed_50MHz, GPIO_AF_1 }, // 22
	{ GPIOC, 13,  RCC_AHBPeriph_GPIOC, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz,         0 }, // 23
	// END
	{ NULL,   0,                 NULL,          NULL,             NULL,             NULL, 0 }
} ;

/*
 * ADC Channel
 */
extern const unsigned int pin_ADC_Channel[]=
{
	NONE,          // 0
	NONE,          // 1
	NONE,          // 2
	NONE,          // 3
	NONE,          // 4
	NONE,          // 5
	NONE,          // 6
	NONE,          // 7
	NONE,          // 8
	NONE,          // 9
	NONE,          // 10
	NONE,          // 11
	NONE,          // 12
	NONE,          // 13
	NONE,          // 14
	ADC_Channel_0, // 15
	ADC_Channel_1, // 16
	ADC_Channel_4, // 17
	ADC_Channel_8, // 18
	ADC_Channel_9, // 19
	ADC_Channel_3, // 20
	NONE,          // 21
	NONE,          // 22
	NONE,          // 23
	NONE           
};

/*
 * TIMER PWM
 */
extern const TIM_TypeDef* pin_TIM[]=
{
	NULL,          // 0
	NULL,          // 1
	TIM15,         // 2
	TIM2,          // 3
	TIM3,          // 4
	TIM3,          // 5
	TIM2,          // 6
	TIM1,          // 7
	NULL,          // 8
	NULL,          // 9
	NULL,          // 10
	NULL,          // 11
	NULL,          // 12
	NULL,          // 13
	NULL,          // 14
	NULL,          // 15
	NULL,          // 16
	NULL,          // 17
	NULL,          // 18
	NULL,          // 19
	NULL,          // 20
	NULL,          // 21
	NULL,          // 22
	NULL,          // 23
	NULL
};

extern const uint16_t pin_TIM_Channel[]=
{
	NONE,          // 0
	NONE,          // 1
	TIM_Channel_1, // 2
	TIM_Channel_2, // 3
	TIM_Channel_2, // 4
	TIM_Channel_1, // 5
	TIM_Channel_3, // 6
	TIM_Channel_1, // 7
	NONE,          // 8
	NONE,          // 9
	NONE,          // 10
	NONE,          // 11
	NONE,          // 12
	NONE,          // 13
	NONE,          // 14
	NONE,          // 15
	NONE,          // 16
	NONE,          // 17
	NONE,          // 18
	NONE,          // 19
	NONE,          // 20
	NONE,          // 21
	NONE,          // 22
	NONE,          // 23
	NONE
};

#ifdef __cplusplus
}
#endif


/*
 * USART objects
 */
RingBuffer rx_buffer1;

USARTClass Serial(USART1, USART1_IRQn, id_serial, &rx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }


// IT handlers
void USART1_IRQHandler(void) 
{
	Serial.IrqHandler();
}



// ----------------------------------------------------------------------------

void serialEventRun(void)
{
	if (Serial.available()) serialEvent();
}
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void __libc_init_array(void);

void init( void )
{
	SystemInit();

	// Set Systick to 1ms interval, common to all SAM3 variants
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		// Capture error
		while (true);
	}
	/* Configure the SysTick Handler Priority: Preemption priority and subpriority */
	NVIC_SetPriority(SysTick_IRQn, 15);	

	
	// Initialize C library
	__libc_init_array();

/************************************ ADC Configuration *************************************/	
	// Initialize Analog Controller
	ADC_InitTypeDef     ADC_InitStructure;
		
	/* ADC1 DeInit */  
	ADC_DeInit(ADC1);
  
	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
  
	/* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);
  
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);     
  
	/* Wait the ADCEN falg */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));	

}

#ifdef __cplusplus
}
#endif

