/* Tone.cpp
  Copyright (c) 2014 BestU.  All right reserved.
  
  A Tone Generator Library

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  */
  
#include "Arduino.h"


volatile long timer_toggle_count;

#define AVAILABLE_TONE_PINS 1
const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2 /*, 3, 4, 5, 1, 0 */ };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255 /*, 255, 255, 255, 255, 255 */ };

static int8_t toneBegin(uint8_t _pin)
{
  int i = 0;
  // if we're already using the pin, the timer should be configured.  
  for (i = 0; i < AVAILABLE_TONE_PINS; i++) {
    if (tone_pins[i] == _pin) {
      return -1;
    }
  }
  
  // search for an unused timer.
  for (i = 0; i < AVAILABLE_TONE_PINS; i++) {
    if (tone_pins[i] == 255) {
      tone_pins[i] = _pin;
      break;
    }
  }
  if (i >= AVAILABLE_TONE_PINS) return -1;
  
  uint16_t TimerPeriod = (uint16_t)(SystemCoreClock / 1000000) - 1;
  TIM_TypeDef* TIMx = TIM14;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Prescaler = TimerPeriod;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);



  NVIC_InitTypeDef   NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  return 1;
}


// frequency (in hertz) and duration (in milliseconds).

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
  uint8_t prescalarbits = 0b001;
  long toggle_count = 0;
  uint32_t ocr = 0;
  int8_t _timer;

  _timer = toneBegin(_pin);
  if (_timer >= 0)
  {
    // Set the pinMode as OUTPUT
    pinMode(_pin, OUTPUT);

    ocr = 1000000 / frequency / 2 - 1;
    if (duration > 0)
    {
      toggle_count = 2 * frequency * duration / 1000;
	  timer_toggle_count = toggle_count;
	  TIM_SetAutoreload(TIM14, ocr);
	  TIM_ARRPreloadConfig(TIM14, ENABLE);
          TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);	
	  /* TIMx counter enable */
	  TIM_Cmd(TIM14, ENABLE);	  
    }
    else
    {
      toggle_count = -1;
    }
	
  }
}

void disableTimer(void)
{
  TIM_ITConfig(TIM14, TIM_IT_Update, DISABLE);		
  /* TIMx counter enable */
  TIM_Cmd(TIM14, DISABLE);	
}

void noTone(uint8_t _pin)
{
  int8_t _timer = -1;
  
  for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
    if (tone_pins[i] == _pin) {
      tone_pins[i] = 255;
    }
  }
  
  disableTimer();

  digitalWrite(_pin, 0);
}

void TIM14_IRQHandler(void)
{
  if (timer_toggle_count != 0)
  {
    g_APinDescription[tone_pins[0]].pPort->ODR ^= 1 << (g_APinDescription[tone_pins[0]].ulPin & 0xF);
    if (timer_toggle_count > 0)
      timer_toggle_count--;
  }
  else
  {
    disableTimer();
    digitalWrite(tone_pins[0], LOW);
  }
}


