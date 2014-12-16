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

#ifndef _VARIANT_MAKERLAB_SUN_
#define _VARIANT_MAKERLAB_SUN_

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		8000000

/** Master clock frequency */
#define VARIANT_MCK			48000000

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "Arduino.h"
#ifdef __cplusplus
#include "USARTClass.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/**
 * Libc porting layers
 */
#if defined (  __GNUC__  ) /* GCC CS3 */
#    include <syscalls.h> /** RedHat Newlib minimal stub */
#endif

#define NONE ((uint8_t)0xFF)

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
 
// Number of pins defined in PinDescription array
#define PINS_COUNT           (22u)
// LEDs
#define PIN_LED_13           (13u)
#define PIN_LED_RXL          -1
#define PIN_LED_TXL          -1
#define PIN_LED              PIN_LED_13
#define PIN_LED2             PIN_LED_RXL
#define PIN_LED3             PIN_LED_TXL

static const uint8_t SS   = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 14;

static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

#define LED_BUILTIN 13

static const uint8_t A0 = 15;
static const uint8_t A1 = 16;
static const uint8_t A2 = 17;
static const uint8_t A3 = 18;
static const uint8_t A4 = 19;
static const uint8_t A5 = 20;


static const uint8_t RX = 0;
static const uint8_t TX = 1;

extern const unsigned int pin_ADC_Channel[];
extern const TIM_TypeDef* pin_TIM[];
extern const uint16_t pin_TIM_Channel[];


#define ADC_RESOLUTION		12


#define PWM_FREQUENCY		1000
#define PWM_RESOLUTION		8

#define WIRE_ISR_HANDLER	I2C1_IRQHandler




#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

extern USARTClass Serial;

#endif

#endif /* _VARIANT_MAKERLAB_SUN_ */

