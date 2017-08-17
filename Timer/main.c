/*
 * File name            : main.c
 * Compiler             : MDK-Lite
 * IDE                  : Keil uVision V5
 * Author               : Etiq Technologies
 * Processor            : LPC1768
 * Created on           : June 17, 2017, 9:44 AM
 * Description          : TIMER0 Test Program
 */
 
#include <lpc17xx.h>

#define		Power_Control_Reg					LPC_SC->PCONP
#define		Match_Control_Reg					LPC_TIM0->MCR
#define		Peripheral_Clk0						LPC_SC->PCLKSEL0
#define		Peripheral_Clk1						LPC_SC->PCLKSEL1
#define		Prescalar_Reg							LPC_TIM0->PR
#define		Timer0_Match_Reg					LPC_TIM0->MR0
#define		Timer0_Timer_Counter			LPC_TIM0->TCR
#define		Interrupt_Reg							LPC_TIM0->IR

#define		MatchReg0_Int									0
#define		MatchReg0_Rst									1

#define		Timer0_Power			 						1	
#define		Timer1_Power			 						2
#define		Timer2_Power								 22
#define		Timer3_Power								 23

#define		Timer0_Peripheral							2
#define		Timer1_Peripheral							4
#define		Timer2_Peripheral						 12
#define		Timer3_Peripheral						 14

#define		Timer_Counter_Enable					0

#define		BlinkDir										LPC_GPIO2->FIODIR
#define		BlinkPin										LPC_GPIO2->FIOPIN
#define		Led														0

int main()
{
	unsigned int pclk, PreScale, ReqCntsPerSec;
	SystemInit();
	
	Power_Control_Reg |= (1<<Timer0_Power);
	Match_Control_Reg	=	(1<<MatchReg0_Int) | (1<<MatchReg0_Rst);
	Timer0_Match_Reg = 1000000;			// 1 sec delay

	pclk = (Peripheral_Clk0 >> Timer0_Peripheral) & 0x03;
	
	ReqCntsPerSec	= 1000000;

	switch(pclk)
	{
    case 0x00: pclk = SystemFrequency/4;
							 break;

    case 0x01: pclk = SystemFrequency;
							 break; 

    case 0x02: pclk = SystemFrequency/2;
							 break; 

    case 0x03: pclk = SystemFrequency/8;
							 break;		
	}

	PreScale = pclk/ReqCntsPerSec;
	Prescalar_Reg = PreScale - 1;
	
	Timer0_Timer_Counter = (1<<Timer_Counter_Enable);
	
	BlinkDir = (1<<Led);
	NVIC_EnableIRQ(TIMER0_IRQn);

	while(1)
	{
		
	}
}

void TIMER0_IRQHandler(void)
{
    uint8_t interrupt_bit;
    interrupt_bit = Interrupt_Reg; 				// When MR0 match occurs, flag is set. Bit 0 of T0 interrupt reg
    Interrupt_Reg = interrupt_bit;				// Writing logic 1 clears the flag
    BlinkPin ^= (1<<Led); 								// Toggle the LED
}
