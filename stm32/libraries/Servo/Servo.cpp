
#include <Arduino.h> 
#include <Servo.h>

#define usToTicks(_us)    (( clockCyclesPerMicrosecond() * _us) / 50)     // converts microseconds to tick
#define ticksToUs(_ticks) (( (unsigned)_ticks * 32)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds


#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays

static servo_t servos[MAX_SERVOS];                          // static array of servo structures

uint8_t ServoCount = 0;                                     // the total number of attached servos

static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo 

/************ static functions common to all instances ***********************/


//timer16_Sequence_t timer;

//------------------------------------------------------------------------------
/// Interrupt handler for the TC0 channel 1.
//------------------------------------------------------------------------------
void Servo_Handler(timer16_Sequence_t timer, TIM_TypeDef* pTc, uint8_t channel);
#if defined (_useTimer2)
void HANDLER_FOR_TIM2(void) {
    	Servo_Handler(_timer2, TIM_FOR_TIMER2, CHANNEL_FOR_TIM2);
}
#endif

void Servo_Handler(timer16_Sequence_t timer, TIM_TypeDef* tc, uint8_t channel)
{
    	TIM_ClearITPendingBit(tc, TIM_IT_CC3);

    	if (Channel[timer] < 0) {
		tc->CNT = 0;
        } else {
        	if (SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true) {
            		digitalWrite(SERVO(timer,Channel[timer]).Pin.nbr, LOW); // pulse this channel low if activated
        	}
    	}

    	Channel[timer]++;    // increment to the next channel
    	if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
		tc->CCR3 = tc->CNT + SERVO(timer, Channel[timer]).ticks;
        	if(SERVO(timer,Channel[timer]).Pin.isActive == true) {    // check if activated
			digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high   
        	}
	}  
	else 
	{ 
        	// finished all channels so wait for the refresh period to expire before starting over 
        	if( (tc->CNT) + 4 < usToTicks(REFRESH_INTERVAL) ) { // allow a few ticks to ensure the next OCR1A not missed
            		tc->CCR3 = (unsigned int)usToTicks(REFRESH_INTERVAL);
        	}
        	else 
		{
            		tc->CCR3 = tc->CNT + 4;  // at least REFRESH_INTERVAL has elapsed
        	}
        	Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
	}
}

static void _initISR(TIM_TypeDef* tim, uint32_t channel, IRQn_Type irqn)
{
	uint16_t PrescalerValue = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
#if defined(_useTimer2)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = IRQn_FOR_TIM2;
#endif
  	
  	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  	/* Compute the prescaler value */
  	PrescalerValue = (uint16_t) (SystemCoreClock  / 1000000) - 1;

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period = 65535;
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);

  	/* Prescaler configuration */
  	TIM_PrescalerConfig(tim, PrescalerValue, TIM_PSCReloadMode_Immediate);

  	/* Output Compare Timing Mode configuration: Channel1 */
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  	/* Output Compare Timing Mode configuration: Channel3 */
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 230;

  	TIM_OC3Init(tim, &TIM_OCInitStructure);

  	TIM_OC3PreloadConfig(tim, TIM_OCPreload_Disable);
   
  	/* TIM Interrupts enable */
  	TIM_ITConfig(tim, TIM_IT_CC3, ENABLE);

  	/* TIMx enable counter */
  	TIM_Cmd(tim, ENABLE);
}

static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer2)
    if (timer == _timer2)
    	_initISR(TIM_FOR_TIMER2, CHANNEL_FOR_TIM2, IRQn_FOR_TIM2);
#endif  
} 

static void finISR(timer16_Sequence_t timer)
{
#if defined (_useTimer2)
    TIM_Cmd(TIM_FOR_TIMER2, ENABLE);
#endif       
}


static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
	servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values
  } else {
    this->servoIndex = INVALID_SERVO;  // too many servos 
  }
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  timer16_Sequence_t timer;

  if (this->servoIndex < MAX_SERVOS) {
    pinMode(pin, OUTPUT);                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;  
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128 
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4; 
    // initialize the timer if it has not already been initialized 
    timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false) {
      initISR(timer);    
    }
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex;
}

void Servo::detach()  
{
  timer16_Sequence_t timer;

  servos[this->servoIndex].Pin.isActive = false;  
  timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo::write(int value)
{  
  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {  
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {  
    if (value < SERVO_MIN())          // ensure pulse width is valid
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();   

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead
    servos[channel].ticks = value;  
  }
}

int Servo::read() // return the value as degrees
{
  return map(readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if (this->servoIndex != INVALID_SERVO)
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION;
  else 
    pulsewidth  = 0;

  return pulsewidth;   
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive;
}

