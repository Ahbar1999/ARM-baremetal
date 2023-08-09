#include <stdint>
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

volatile uint32_t* USART_CR1 = (uint32_t*)(USART1_START_ADDR + USART_CR1_OFFSET); 
volatile uint32_t* USART_CR2 = (uint32_t*)(USART1_START_ADDR + USART_CR2_OFFSET);
volatile uint32_t* USART_CR3 = (uint32_t*)(USART1_START_ADDR + USART_CR3_OFFSET);
volatile uint32_t* USART_BRR = (uint32_t*)(USART1_START_ADDR + USART_BRR_OFFSET);
volatile uint32_t* USART_GTPR = (uint32_t*)(USART1_START_ADDR + USART_GTPR_OFFSET);
volatile uint32_t* USART_RTOR = (uint32_t*)(USART1_START_ADDR + USART_RTOR_OFFSET);
volatile uint32_t* USART_RQR  = (uint32_t*)(USART1_START_ADDR + USART_RQR_OFFSET);
volatile uint32_t* USART_ISR  = (uint32_t*)(USART1_START_ADDR + USART_ISR_OFFSET);
volatile uint32_t* USART_ICR  = (uint32_t*)(USART1_START_ADDR + USART_ICR_OFFSET);
volatile uint32_t* USART_RDR  = (uint32_t*)(USART1_START_ADDR + USART_RDR_OFFSET);  	
volatile uint32_t* USART_TDR  = (uint32_t*)(USART1_START_ADDR + USART_TDR_OFFSET);  		

#define RCC_START 					0x40021000
#define RCC_APB2ENR_OFFSET			0x18

uint32_t volatile uint32_t* RCC_APB2ENR = (uint32_t*)(RCC_START + RCC_APB2ENR_OFFSET)

extern void configure_usart(uint16_t baud_rate) {
	*RCC_APB2ENR = 0x00000000;
	*RCC_APB2ENR |= 1 << 14;
	
	// for 7 + 1 codeword length M[1:0] = 10;
	*USART_CR1 |= 1 << 28;
	*USART_CR1 |= 1 << 12;
	*((*uint16_t)USART_BRR) = baud_rate;
	// stop bits = 00;
	*USART_CR1 |= 1 << 0; 	// USART enable
	*USART_CR1 |= 1 << 3;	// Tx enable
} 

// takes in pointer to data, we only need 8 bits because we are transmitting ascii + 1 start bit 
extern void send_messsage(uint8_t* msg, uint32_t size) {
	while (size > 0) {
		*USART_TDR = *msg++;
	}

	// wait until TC bit = 1 
	while ((*USART_ISR | (1 << 6)) == 0);
}
