#include <stdint.h>
#include "usart.h"

#define USART1_START_ADDR 	0x40013800
#define USART2_START_ADDR	0x40004400
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

#define USART_CR1_RV 	0x00U
#define USART_CR2_RV 	0x00U
#define USART_CR3_RV 	0x00U
#define USART_BRR_RV 	0x00U
#define USART_GTPR_RV 	0x00U
#define USART_RTOR_RV 	0x00U
#define USART_RQR_RV 	0x00U
#define USART_ISR_RV 	0x020000C0
#define USART_ICR_RV 	0x00U
#define USART_RDR_RV 	0x00U
#define USART_TDR_RV 	0x00U

#define RXNE	5
#define RXNEIE	5
#define TXEIE	7
// framing error detection
#define FE		1
// idle line detection
#define IDLE	4
#define BUSY 	16
#define IDLEIE	4
// Rx timeout bit in ISR register
#define RTOF	11


// defined in gpio.c
// #define RCC_APB2ENR_RV	0x00000000	

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

volatile uint32_t* USART2_CR1 = (uint32_t*)(USART2_START_ADDR + USART_CR1_OFFSET); 
volatile uint32_t* USART2_CR2 = (uint32_t*)(USART2_START_ADDR + USART_CR2_OFFSET);
volatile uint32_t* USART2_CR3 = (uint32_t*)(USART2_START_ADDR + USART_CR3_OFFSET);
volatile uint32_t* USART2_BRR = (uint32_t*)(USART2_START_ADDR + USART_BRR_OFFSET);
volatile uint32_t* USART2_GTPR = (uint32_t*)(USART2_START_ADDR + USART_GTPR_OFFSET);
volatile uint32_t* USART2_RTOR = (uint32_t*)(USART2_START_ADDR + USART_RTOR_OFFSET);
volatile uint32_t* USART2_RQR  = (uint32_t*)(USART2_START_ADDR + USART_RQR_OFFSET);
volatile uint32_t* USART2_ISR  = (uint32_t*)(USART2_START_ADDR + USART_ISR_OFFSET);
volatile uint32_t* USART2_ICR  = (uint32_t*)(USART2_START_ADDR + USART_ICR_OFFSET);
volatile uint32_t* USART2_RDR  = (uint32_t*)(USART2_START_ADDR + USART_RDR_OFFSET);  	
volatile uint32_t* USART2_TDR  = (uint32_t*)(USART2_START_ADDR + USART_TDR_OFFSET);  		


// #define RCC_START 					0x40021000
// #define RCC_APB2ENR_OFFSET			0x18

// volatile uint32_t* RCC_APB2ENR = (uint32_t*)(RCC_START + RCC_APB2ENR_OFFSET);
extern volatile uint8_t usart1_configured = 0;
extern volatile uint8_t usart2_configured = 0;

// USARTDIV = baud rate = 0x0341H, for baud rate = 9600
void configure_usart1(uint16_t baud_rate) {
	// RESET REGISTERS
	// *RCC_APB2ENR 	= 	RCC_APB2ENR_RV;
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
	
	// for 8 + 1 codeword length M[1:0] = 00; (reset value)
	// *USART1_CR1 |= 1 << 28;
	// *USART1_CR1 |= 1 << 12;
	*USART1_BRR = baud_rate;	
	// stop bits = 00;
		
	*USART1_CR1 |= 1 << 0; 	// USART enable, UE = 1	
	usart1_configured = 1;
	*USART1_CR1 |= 1 << 3;	// Tx enable, TXE = 1, send idle frame
	*USART1_CR1 |= 1 << 2;	// Rx enable, RE = 1, start seaching for start bit		
}

void configure_usart2(uint16_t baud_rate) {
	*USART2_CR1 	=	USART_CR1_RV;	
	*USART2_CR2 	=	USART_CR2_RV;
	*USART2_CR3		= 	USART_CR3_RV;	
	*USART2_BRR		=	USART_BRR_RV;
	*USART2_GTPR	=	USART_GTPR_RV;
	*USART2_RTOR	=	USART_RTOR_RV;
	*USART2_RQR		=	USART_RQR_RV;
	*USART2_ISR		=	USART_ISR_RV;
	*USART2_ICR		=	USART_ICR_RV;
	*USART2_RDR		=	USART_RDR_RV;
	*USART2_TDR		=	USART_TDR_RV;
	
	*USART2_BRR = baud_rate;
	// stop bits programmed to = 00(1);
	
	*USART2_CR1 |= 1 << 0; 			// USART enable, UE = 1	
	usart2_configured = 1;
	// enable receiver timeout(NOT SUPPORTED IN USART2 on stm32f05)		
	// *USART2_CR2 |= 1 << 23;			// RTOEN = 1 	 
	// *USART2_RTOR =	1000U;			// timeout bits programming	
	
	*USART2_CR1 |= 1 << 3;			// TE = 1 	 
	*USART2_CR1 |= 1 << 2;			// Rx enable, RE = 1, start seaching for start bit	
} 

/*
void recv_message() {
	// wait while data not recvd
	while (((*USART1_ISR) & (1 << RXNE)) == 0);
	
	// read the contents of RDR register 
	// recv_buf = (uint8_t)(*USART1_RDR);	
	// send_message(&recv_buf, sizeof(recv_buf));
} 
*/
void uart_enable_interrupt(uint8_t uart_x, uint8_t recv) {
	if (recv) {
		// enable reciever interrupt
		*USART1_CR1 |= 1 << RXNEIE; 	
	} else {
		// enable tx interrupt
		*USART1_CR1 |= 1 << TXEIE; 	
	}
}

/*
void UART1_Handler() {
	// __asm volatile("BKPT");
		
}
*/
/*
void UART2_Handler() {
	__asm volatile("BKPT");
}
*/

void write_usart(volatile uint32_t* USARTx_ISR, volatile uint32_t* USARTx_TDR, volatile uint32_t* USARTx_ICR, uint8_t* buf, uint8_t size) {
		// now transmit data
		uint8_t i = 0;
		while (i < size) {
			// check TC bit, if it is 1 or not
			if ((*USARTx_ISR & (1 << 6))) {	
				*USARTx_TDR = buf[i++];	
			}
		}

		// set TCCF
		*USARTx_ICR |= (1 << 6);
}
// we will have to uart interrupts to process data, there is no other way
uint8_t recv_esp(volatile uint32_t* USARTx_ISR, volatile uint32_t* USARTx_RDR, volatile uint8_t* buf) {
		// clear buffer
		uint8_t i = 0;
		// recieve until timeout
		while(1) {
			if (*USARTx_ISR & (1 << RXNE)) {	
				buf[i++] = (uint8_t)(*USARTx_RDR);
			}		
		}

		return i;		
}

uint8_t get_input(volatile uint32_t* USARTx_ISR, volatile uint32_t* USARTx_RDR, uint8_t* buf){
	uint8_t i = 0;	
	while(i == 0 || buf[i - 1] != '\n') {
		if (*USARTx_ISR & (1 << RXNE)) {	
			buf[i++] = (uint8_t)(*USARTx_RDR);	
		}
	}
	return i;
}

// null byte is implicit in c strings
// flush terminal characters(for putty like tools)
uint8_t flush[] = "\033\143";
uint8_t prompt[] = "> ";
// buffer for receiving and sending data
uint8_t recv_buf[100];
uint8_t bytes_recvd;

// echo from uartx to uarty
void echo() {		
	while(1) {
			
		// show prompt; 
		write_usart(USART1_ISR, USART1_TDR, USART1_ICR, prompt, (sizeof(prompt) / sizeof(prompt[0])));	
		// listen to pc	
		bytes_recvd = get_input(USART1_ISR, USART1_RDR, recv_buf);		
		// echo back message	
		write_usart(USART1_ISR, USART1_TDR, USART1_ICR, recv_buf, bytes_recvd);	
		
		// send to esp
		write_usart(USART2_ISR, USART2_TDR, USART2_ICR, recv_buf, bytes_recvd);	
				
		// listen to esp
		bytes_recvd = recv_esp(USART2_ISR, USART2_RDR, recv_buf);		
		// __asm volatile("BKPT");		
		// send to pc 
		write_usart(USART1_ISR, USART1_TDR, USART1_ICR, recv_buf, bytes_recvd);
				
		*USART1_RQR |= (1 << 3);	// Receive data flush request, discard data in the rdr	
		*USART2_RQR |= (1 << 3);	// Receive data flush request, discard data in the rdr
	}
}
