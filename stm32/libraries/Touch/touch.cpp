

#include "touch.h"

void TOUCH::begin(void)
{
	GPIO_InitTypeDef gpio_config;
	uint32_t tmp_value = 0xFFFFFFFF;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	gpio_config.GPIO_Mode = GPIO_Mode_AF;
	gpio_config.GPIO_OType = GPIO_OType_OD;
	gpio_config.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_config.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &gpio_config);
	gpio_config.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &gpio_config);

	gpio_config.GPIO_OType = GPIO_OType_PP;
	gpio_config.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &gpio_config);

	gpio_config.GPIO_OType = GPIO_OType_PP;
	gpio_config.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &gpio_config);

	GPIO_PinAFConfig(GPIOA, 11, GPIO_AF_3);
	GPIO_PinAFConfig(GPIOA, 12, GPIO_AF_3);	
	GPIO_PinAFConfig(GPIOB, 0, GPIO_AF_3);
	GPIO_PinAFConfig(GPIOB, 1, GPIO_AF_3);	
	GPIO_PinAFConfig(GPIOB, 2, GPIO_AF_3);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_TS, ENABLE);
	// CTPH = 1x tPGCLK, CTPL = 1x tPGCLK, PGPSC = 5(fHCLK/32), MCV = 6, TSCE = ENABLE
	TSC->CR = (0 << 28) | (0 << 24) | (5 << 12) | (6 << 5) | (1 << 0);
	
	tmp_value &= (uint32_t)~((uint32_t)1 << 9);
	tmp_value &= (uint32_t)~((uint32_t)1 << 10);
	tmp_value &= (uint32_t)~((uint32_t)1 << 11);
	tmp_value &= (uint32_t)~((uint32_t)1 << 14);
	tmp_value &= (uint32_t)~((uint32_t)1 << 15);
	TSC->IOHCR &= tmp_value;

	// Sampling enabled
	tmp_value = 0;
	tmp_value |= (uint32_t)((uint32_t)1 << 11);
	tmp_value |= (uint32_t)((uint32_t)1 << 15);
	TSC->IOSCR |= tmp_value;

	tmp_value = 0;
	tmp_value |= (uint32_t)((uint32_t)1 << 9);
	tmp_value |= (uint32_t)((uint32_t)1 << 10);
	tmp_value |= (uint32_t)((uint32_t)1 << 14);
	TSC->IOCCR |= tmp_value;
	// Enable group, G3 and G4
	//TSC->IOGCSR |= (1 << 2);

	_trace.index = 0;
	_trace.key[0].fixed_value = 0xD64;
	_trace.key[1].fixed_value = 0xDED;
	_trace.key[2].fixed_value = 0x1023;
	_trace.key[0].current_value = 0;
	_trace.key[1].current_value = 0;
	_trace.key[2].current_value = 0;
	_trace.key[0].status = TOUCH_RELEASE;
	_trace.key[1].status = TOUCH_RELEASE;
	_trace.key[2].status = TOUCH_RELEASE;
	_trace.key[0].enable_mask = 1 << 9;
	_trace.key[1].enable_mask = 1 << 10;
	_trace.key[2].enable_mask = 1 << 14;
	
}
void TOUCH::scan(void)
{
  scan_channel(_trace.index++);
  _trace.index = (_trace.index >= 3) ? 0 : _trace.index;
}
TouchStatus TOUCH::getstatus(ToutchKey key)
{
	return _trace.key[key].status;
}
void TOUCH::scan_channel(int index)
{
	int delay_cnt = 0;
	short diff;

	if (index == 2) {
		TSC->IOGCSR = (1 << 3);
	} else {
		TSC->IOGCSR = (1 << 2);
	}
	TSC->IOCCR = _trace.key[index].enable_mask;
	TSC->ICR |= 0x03;
	TSC->CR |= 0x02;
  
	while(wait_eoc() == 0) {
		if(++delay_cnt > 10000) {
			_trace.key[index].status = TOUCH_RELEASE;
			return;
		}
	}
	if (index == 2) {
		_trace.key[index].current_value = TSC->IOGXCR[3];
	} else {
		_trace.key[index].current_value = TSC->IOGXCR[2];
	}
	diff = abs(_trace.key[index].current_value - _trace.key[index].fixed_value);
	if(diff > THRESHOLD)
	{
		_trace.key[index].status = TOUCH_PRESSED;
	} else {
		_trace.key[index].status = TOUCH_RELEASE;
	}
}
int TOUCH::wait_eoc(void)
{
	int ret = 0;
	uint32_t status = TSC->ISR;

	if (status & 0x01) {
		// Check MCEF flag
		if (status & 0x02) {
			ret = 0;
		} else {
			ret = 1;
		}
	}
	TSC->ICR = status;

	return ret;
}