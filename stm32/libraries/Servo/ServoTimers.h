
// For STM32:
#define _useTimer2


#if defined (_useTimer2)
#define TIM_FOR_TIMER2            TIM2
#define CHANNEL_FOR_TIM2          1
#define IRQn_FOR_TIM2             TIM2_IRQn
#define HANDLER_FOR_TIM2          TIM2_IRQHandler
#endif


typedef enum {
#if defined (_useTimer1)	
	_timer1,
#endif
#if defined (_useTimer2)	
	_timer2,
#endif
#if defined (_useTImer3)	
	_timer3,
#endif	
	_Nbr_16timers 
} timer16_Sequence_t ;

