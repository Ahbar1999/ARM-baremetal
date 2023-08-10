#include <stdint.h>
#include "usart.h"

#define USART1_START_ADDR 	0x40013800
#define USART_CR1_OFFSET 	0x00
#define USART_CR2_OFFSET 	0x04
#define USART_CR3_OFFSET 	0x08
#define USART_BRR_OFFSET 	0x0C
#define USART_GTPR_OFFSET 	0x10
#define USART_RTOR_OFFSET 	0x14
#define USART_RQR_OFFSET 	0x18
#define USART_ISR_OFFSET 	0x1C
#define USART_ICR_OFFSET 	0x20
#define USART_RDR_OFFSET 	0x24
#define USART_TDR_OFFSET 	0x28

#define USART_CR1_RV 	0x00
#define USART_CR2_RV 	0x00
#define USART_CR3_RV 	0x00
#define USART_BRR_RV 	0x00
#define USART_GTPR_RV 	0x00
#define USART_RTOR_RV 	0x00
#define USART_RQR_RV 	0x00
#define USART_ISR_RV 	0x020000C0
#define USART_ICR_RV 	0x00
#define USART_RDR_RV 	0x00
#define USART_TDR_RV 	0x00

#define RCC_APB2ENR_RV	0x00000000	

volatile uint32_t* USART1_CR1 = (uint32_t*)(USART1_START_ADDR + USART_CR1_OFFSET); 
volatile uint32_t* USART1_CR2 = (uint32_t*)(USART1_START_ADDR + USART_CR2_OFFSET);
volatile uint32_t* USART1_CR3 = (uint32_t*)(USART1_START_ADDR + USART_CR3_OFFSET);
volatile uint32_t* USART1_BRR = (uint32_t*)(USART1_START_ADDR + USART_BRR_OFFSET);
volatile uint32_t* USART1_GTPR = (uint32_t*)(USART1_START_ADDR + USART_GTPR_OFFSET);
volatile uint32_t* USART1_RTOR = (uint32_t*)(USART1_START_ADDR + USART_RTOR_OFFSET);
volatile uint32_t* USART1_RQR  = (uint32_t*)(USART1_START_ADDR + USART_RQR_OFFSET);
volatile uint32_t* USART1_ISR  = (uint32_t*)(USART1_START_ADDR + USART_ISR_OFFSET);
volatile uint32_t* USART1_ICR  = (uint32_t*)(USART1_START_ADDR + USART_ICR_OFFSET);
volatile uint32_t* USART1_RDR  = (uint32_t*)(USART1_START_ADDR + USART_RDR_OFFSET);  	
volatile uint32_t* USART1_TDR  = (uint32_t*)(USART1_START_ADDR + USART_TDR_OFFSET);  		

#define RCC_START 					0x40021000
#define RCC_APB2ENR_OFFSET			0x18

volatile uint32_t* RCC_APB2ENR = (uint32_t*)(RCC_START + RCC_APB2ENR_OFFSET);
extern volatile uint8_t usart_configured = 0;

// USARTDIV = baud rate = 0x0341H, for baud rate = 9600
void configure_usart(uint16_t baud_rate) {
	// RESET REGISTERS
	*RCC_APB2ENR 	= 	RCC_APB2ENR_RV;
	*USART1_CR1 	=	USART_CR1_RV;	
	*USART1_CR2 	=	USART_CR2_RV;
	*USART1_CR3		= 	USART_CR3_RV;	
	*USART1_BRR		=	USART_BRR_RV;
	*USART1_GTPR	=	USART_GTPR_RV;
	*USART1_RTOR	=	USART_RTOR_RV;
	*USART1_RQR		=	USART_RQR_RV;
	*USART1_ISR		=	USART_ISR_RV;
	*USART1_ICR		=	USART_ICR_RV;
	*USART1_RDR		=	USART_RDR_RV;
	*USART1_TDR		=	USART_TDR_RV;

	// enable USART	
	*RCC_APB2ENR |= 1 << 14;
	
	// for 7 + 1 codeword length M[1:0] = 10;
	*USART1_CR1 |= 1 << 28;
	*USART1_CR1 |= 1 << 12;
	*USART1_BRR |= baud_rate;
	// stop bits = 00;
	
	*USART1_CR1 |= 1 << 0; 	// USART enable, UE = 1
	*USART1_CR1 |= 1 << 3;	// Tx enable, TXE = 1
	
	usart_configured = 1;
} 

// takes in pointer to data, we only need 8 bits because we are transmitting ascii + 1 start bit 
void send_message(uint8_t* msg, uint32_t size) {
	while (size > 0) {
		// check TC bit, if it is 1 or not  
		if ((*USART1_ISR & (1 << 6)) != 0) {
			size--;	
			// fill tdr with new data
			*USART1_TDR = *msg++;
		} 
	}
	// clear the tranmission complete clear flag, TCCF
	// to indicate that transmission is completed
	// this will set the TC flag in the ISR register 	
	*USART1_ICR |= (1 << 6);
	// wait until TC bit = 1 
	while ((*USART1_ISR & (1 << 6)) == 0);
}
