


  /********************************************************************************

  Main file for System clock configuration
  Author:   AARCtronics
  Updated:  25 Jul 2024

 ******************************************************************************
  Copyright (C) 2024 AARCtronics.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4 which gives APB1 peripheral clock 45000000 Hz and APB1 Timer clock 90000000 Hz
  *            APB2 Prescaler                 = 2 which gives APB1 peripheral clock 90000000 Hz and APB1 Timer clock 180000000 Hz
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4    
  *            PLL_N                          = 180
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5 :0101 These bits represent the ratio of the CPU clock period to the Flash memory access time.
  * @param  None
  * @retval None
  */

#include "RccConfig.h"

void SysClockConfig (void) //Writing a function for the system clock
{
		/*************>>>>>>> STEPS to be FOLLOWED <<<<<<<<************
	
	1. RCC_CR register :- ENABLE HSE (High speed external oscillator) and wait for the HSE to become Ready
	2. RCC_APB1ENR register :- Set the POWER ENABLE CLOCK and Power register PWR_CR :- VOLTAGE REGULATOR
	3. Configure the FLASH register Flash_CR :- PREFETCH (Prefetch enable), ICEN (Instruction cache enable),DCEN(Data cache Enable), and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. RCC_PLLCFGR registers :- Configure the MAIN PLL 
	6. Enable the PLL and wait for it to become ready
	7. RCC_CFGR registers :- Select the Clock Source and wait for it to be set
	
	********************************************************/
	
	/*RCC PLL configuration register (RCC_PLLCFGR) */
	#define PLL_M 	4 		//PLLM[5:0] Division factor for the main PLL (PLL) input clock (0x4 <<0)
												//000100: PLLM = 4
	#define PLL_N 	180 	// PLLN[8:0]: Main PLL (PLL) multiplication factor for VCO
												// (180<<6)
	#define PLL_P 	0  		// PLLP[1:0]: Main PLL (PLL) division factor for main system clock
												//00: PLLP = 2 ,      ~(3<<16)
	
	// 1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON;  // RCC -> CR |= (0x1<<16); 1: [HSE oscillator ON]
	while (!(RCC->CR & RCC_CR_HSERDY)); // while (!(RCC->CR & 1 << 17));[ 17 th bit high , 1: HSE oscillator ready]
	
	
	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; //Bit 28 PWREN: Power interface clock enable RCC->APB1ENR |= 1<<28;
	PWR->CR |= PWR_CR_VOS; // 11: Scale 1 mode (reset value) PWR->CR |=(0x3 << 14)
	
	
	// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	//	FLASH->ACR |=(0x1 << 9)  | (0x1 << 10) |(0x1 << 8) | (0x5 << 0);
	
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// AHB prescaler configuration
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //180 MHz , 0xx: AHB clock not divided  RCC->CFGR &=~(0xF << 4);

	// APB1 prescaler
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // 45 MHZ, 101: AHB clock divided by 4, RCC->CFGR |=(0x5 << 10);
	
	// APB2 prescaler
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // 45 MHZ, 101: AHB clock divided by 2, RCC->CFGR |=(0x4 << 13)
	
	
	// 5. Configure the MAIN PLL and clock source
	RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (RCC_PLLCFGR_PLLSRC_HSE);
	//RCC->PLLCFGR |= (4 <<0) | (180 << 6) > (0 <<16) | (1<< 22); 
	//RCC_PLLCFGR_PLLSRC_HSE is Bit 22 PLLSRC: Main PLL(PLL) and audio PLL (PLLI2S) entry clock source)

	// 6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON; //RCC ->CR |= (0x1<<24  Bit 24 MAIN PLLON: Main PLL (PLL) enable
	while (!(RCC->CR & RCC_CR_PLLRDY)); //RCC ->CR |= (0x1<<25  Bit 25 Wait for PLL ready
	
	
	// 7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL; //Bits 1:0 SW[1:0]: System clock switch  
	                              //10: PLL_P selected as system clock  (RCC->CFGR |=0x2<<0)
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); //Bits 3:2 SWS[1:0]: System clock switch status
	//while (!(RCC->CFGR |=0x2<<0)&(RCC->CFGR |=0x2<<2))
	//while (!(RCC->CFGR &0x2<<))  (or we can also write in this form.
}

