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

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

static int _readResolution = 10;
static int _writeResolution = 8;

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return value >> (from-to);
	else
		return value << (to-from);
}

eAnalogReference analog_reference = AR_DEFAULT;

void analogReference(eAnalogReference ulMode)
{
	analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
	uint32_t ulValue = 0;
	uint32_t ulChannel;
	ADC_InitTypeDef     ADC_InitStructure;

	if (ulPin < A0)
	{
		ulPin += A0;
	}

	ulChannel = pin_ADC_Channel[ulPin] ;

	if (g_APinDescription[ulPin].pPort == NULL || ulChannel == NONE )
	{
		return -1;
	}
	pinMode(ulPin, ANALOG);
	/* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
	ADC_ChannelConfig(ADC1, ulChannel , ADC_SampleTime_55_5Cycles);
	//Start ADC1 Software Conversion
	ADC_StartOfConversion(ADC1);

	// Wait until conversion completion
	// while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	// Read the value
	ulValue = ADC_GetConversionValue(ADC1);
	ulValue = mapResolution(ulValue, ADC_RESOLUTION, _readResolution);

	return ulValue;
}


static uint8_t pinEnabled[PINS_COUNT];

void analogOutputInit(void) {
	uint8_t i;
	for (i=0; i<PINS_COUNT; i++)
		pinEnabled[i] = 0;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t ulPin, uint32_t ulValue) {

	if ( g_APinDescription[ulPin].pPort == NULL )
	{
		return;
	}
	if (pin_TIM[ulPin] == NULL)
	{
	    // Defaults to digital write
		pinMode(ulPin, OUTPUT);
		ulValue = mapResolution(ulValue, _writeResolution, 8);
		if (ulValue < 128)
		{
			digitalWrite(ulPin, LOW);
		}
		else
		{
			digitalWrite(ulPin, HIGH);
		}

		return;
	}
	TIM_TypeDef* TIMx = pin_TIM[ulPin];
	
	ulValue = mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
	uint16_t TimerPeriod = (uint16_t)(SystemCoreClock / 1000000) - 1;
	uint16_t TimerARP = (uint16_t)(1000000 / PWM_FREQUENCY) - 1;
	uint16_t Duty_Cycle = (uint16_t)((ulValue * 100) / 255);
	uint16_t ChannelPulse = (uint16_t)((Duty_Cycle * (TimerARP + 1)) / 100);
	
	pinMode(ulPin, ALTERNATE);

	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	if (!pinEnabled[ulPin])
	{
		if (TIMx == TIM1)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		}
		else if (TIMx == TIM2)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		}
		else if (TIMx == TIM3)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		}
		else if (TIMx == TIM15)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
		}
		
		TIM_TimeBaseStructure.TIM_Prescaler = TimerPeriod;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = TimerARP;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		pinEnabled[ulPin] = 1;
	}
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	
	if (pin_TIM_Channel[ulPin] == TIM_Channel_1)
	{
		// PWM1 Mode configuration: Channel1
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);	
	}
	else if (pin_TIM_Channel[ulPin] == TIM_Channel_2)
	{
		// PWM2 Mode configuration: Channel1
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);		
	}
	else if (pin_TIM_Channel[ulPin] == TIM_Channel_3)
	{
		// PWM3 Mode configuration: Channel1
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);		
	}
	else if (pin_TIM_Channel[ulPin] == TIM_Channel_4)
	{
		// PWM4 Mode configuration: Channel1
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);		
	}
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	
	/* TIMx counter enable */
	TIM_Cmd(TIMx, ENABLE);

	/* TIMx Main Output Enable */
	TIM_CtrlPWMOutputs(TIMx, ENABLE);
}

#ifdef __cplusplus
}
#endif
