/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "arduino.h"
#include "SPI.h"

SPIClass SPI;

void SPIClass::begin() {

	// When the SS pin is set as OUTPUT, it can be used as
	// a general purpose output port (it doesn't influence
	// SPI operations).
	pinMode(SS, OUTPUT);
  
	// Set SS to high so a connected chip will be "deselected" by default
	digitalWrite(SS, HIGH);
  
	pinMode(SCK,  ALTERNATE);
	pinMode(MOSI, ALTERNATE);
	pinMode(MISO, ALTERNATE);
  
	spi = SPI1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_CLOCK_DIV4;	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(spi,&SPI_InitStructure);
	SPI_RxFIFOThresholdConfig(spi,SPI_RxFIFOThreshold_QF);
	SPI_Cmd(spi,ENABLE);
}


void SPIClass::end() {
	SPI_Cmd(spi,DISABLE);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
	SPI_Cmd(spi,DISABLE);
	if(bitOrder == LSBFIRST)
	{
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	}
	else
	{
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	}
	SPI_Init(spi,&SPI_InitStructure);
	SPI_Cmd(spi,ENABLE);
}

void SPIClass::setDataMode(uint8_t mode)
{
	SPI_Cmd(spi,DISABLE);
	if (mode == SPI_MODE0)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} 
	else if(mode == SPI_MODE1)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	} 
	else if(mode == SPI_MODE2)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} 
	else if(mode == SPI_MODE3)
	{
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	}

	SPI_Init(spi,&SPI_InitStructure);
	SPI_Cmd(spi,ENABLE);
}

void SPIClass::setClockDivider(uint8_t rate)
{
	SPI_Cmd(spi,DISABLE);
	SPI_InitStructure.SPI_BaudRatePrescaler = rate;

	SPI_Init(spi,&SPI_InitStructure);
	SPI_Cmd(spi,ENABLE);
}

byte SPIClass::transfer(byte _data) {
	uint8_t d = _data;

    while (SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(spi,d);

    while (SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_RXNE) == RESET);
    d = SPI_ReceiveData8(spi);
    return d ;
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}


