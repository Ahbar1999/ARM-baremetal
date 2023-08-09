#include <stdint.h>
#include "blink.h"
#include "systick.h"

#define GPIOA_START				0x48000000
// on board led is connected to PORT C
#define GPIOC_START 			0x48000800

#define GPIO_MODER_OFFSET 		0x00
#define GPIO_OTYPER_OFFSET 		0x04
#define GPIO_OSPEEDR_OFFSET 	0x08
#define GPIO_PUPDR_OFFSET 		0x0C
#define GPIO_IDR_WRITE_OFFSET	0x10 + 0x10             /* the reset value is for last 16 bits only, first 16 are RO bits */ 
#define GPIO_IDR_READ_OFFSET	0x10		 
#define GPIO_ODR_OFFSET 		0x14
#define GPIO_BSR_OFFSET 		0x18                    /* starting first bits are for setting ODR bits */
#define GPIO_BRR_OFFSET         GPIO_BSR_OFFSET + 0x10  /* rest are for resetting */
#define GPIO_LCKR_OFFSET 		0x1C
#define	GPIO_AFRL_OFFSET		0x20
#define GPIO_AFRH_OFFSET 		0x24
// There is another bit reset register in the datasheet i dont think we need it  
// #define GPIO_BRR_OFFSET		0x28

#define GPIOA_MODER_RESET_VALUE 	0x28000000 
#define GPIOA_OTYPER_RESET_VALUE 	0x00000000
#define GPIOA_OSPEEDR_RESET_VALUE 	0x0C000000
#define GPIOA_PUPDR_RESET_VALUE 	0x24000000
#define GPIOA_IDR_RESET_VALUE 		0x0000
#define GPIOA_ODR_RESET_VALUE 		0x00000000
#define GPIOA_BSR_RESET_VALUE 		0x0000
#define GPIOA_BRR_RESET_VALUE 		0x0000
#define GPIOA_LCKR_RESET_VALUE 		0x00000000
#define	GPIOA_AFRL_RESET_VALUE		0x00000000
#define GPIOA_AFRH_RESET_VALUE 		0x00000000	

#define GPIOC_MODER_RESET_VALUE 	0x00000000 
#define GPIOC_OTYPER_RESET_VALUE 	0x00000000
#define GPIOC_OSPEEDR_RESET_VALUE 	0x00000000
#define GPIOC_PUPDR_RESET_VALUE 	0x00000000
#define GPIOC_IDR_RESET_VALUE 		0x0000
#define GPIOC_ODR_RESET_VALUE 		0x00000000
#define GPIOC_BSR_RESET_VALUE 		0x0000
#define GPIOC_BRR_RESET_VALUE 		0x0000
#define GPIOC_LCKR_RESET_VALUE 		0x00000000
#define	GPIOC_AFRL_RESET_VALUE		0x00000000
#define GPIOC_AFRH_RESET_VALUE 		0x00000000	

#define RCC_START 					0x40021000
#define RCC_AHBENR_RESET_VALUE		0x00000014

enum GPIO_MODE {IP_MODE, GP_OP_MODE, AF_MODE, AN_MODE};
enum GPIO_OTYPE {OP_PUSH_PULL, OP_OPEN_DRAIN};
enum GPIO_OSPEED {LOW, MEDIUM, HIGH};
enum GPIO_PUPD {NONE, PULL_UP, PULL_DOWN, RESERVED};
enum GPIO_BS {NO_ACTION_BS, SET};
enum GPIO_BR {NO_ACTION_BR, RESET};

// To enable clock on PORTA
uint32_t volatile *RCC_AHBENR 		= 	(uint32_t*)(RCC_START + 0x14);

uint32_t volatile *GPIOA_MODER 		=  	(uint32_t*)(GPIOA_START + GPIO_MODER_OFFSET);  
uint32_t volatile *GPIOA_OTYPER 	= 	(uint32_t*)(GPIOA_START + GPIO_OTYPER_OFFSET);
uint32_t volatile *GPIOA_OSPEEDR 	=	(uint32_t*)(GPIOA_START + GPIO_OSPEEDR_OFFSET);   	
uint32_t volatile *GPIOA_PUPDR 		=  	(uint32_t*)(GPIOA_START + GPIO_PUPDR_OFFSET);
uint32_t volatile *GPIOA_IDR_READ	= 	(uint32_t*)(GPIOA_START + GPIO_IDR_READ_OFFSET); 
uint32_t volatile *GPIOA_IDR_WRITE	=	(uint32_t*)(GPIOA_START + GPIO_IDR_WRITE_OFFSET); 
uint32_t volatile *GPIOA_ODR	    =  	(uint32_t*)(GPIOA_START + GPIO_ODR_OFFSET);
uint32_t volatile *GPIOA_BSR 	=  		(uint32_t*)(GPIOA_START + GPIO_BSR_OFFSET);
uint32_t volatile *GPIOA_BRR 	=  		(uint32_t*)(GPIOA_START + GPIO_BRR_OFFSET);
uint32_t volatile *GPIOA_LCKR 	=  		(uint32_t*)(GPIOA_START + GPIO_LCKR_OFFSET);
uint32_t volatile *GPIOA_AFRL	=  		(uint32_t*)(GPIOA_START + GPIO_AFRL_OFFSET); 
uint32_t volatile *GPIOA_AFRH 	=  		(uint32_t*)(GPIOA_START + GPIO_AFRH_OFFSET);

uint32_t volatile *GPIOC_MODER 		=  	(uint32_t*)(GPIOC_START + GPIO_MODER_OFFSET);  
uint32_t volatile *GPIOC_OTYPER 	= 	(uint32_t*)(GPIOC_START + GPIO_OTYPER_OFFSET);
uint32_t volatile *GPIOC_OSPEEDR 	=	(uint32_t*)(GPIOC_START + GPIO_OSPEEDR_OFFSET);   	
uint32_t volatile *GPIOC_PUPDR 		=  	(uint32_t*)(GPIOC_START + GPIO_PUPDR_OFFSET);
uint32_t volatile *GPIOC_IDR_READ	= 	(uint32_t*)(GPIOC_START + GPIO_IDR_READ_OFFSET); 
uint32_t volatile *GPIOC_IDR_WRITE	=	(uint32_t*)(GPIOC_START + GPIO_IDR_WRITE_OFFSET); 
uint32_t volatile *GPIOC_ODR	    =  	(uint32_t*)(GPIOC_START + GPIO_ODR_OFFSET);
uint32_t volatile *GPIOC_BSR 	=  		(uint32_t*)(GPIOC_START + GPIO_BSR_OFFSET);
uint32_t volatile *GPIOC_BRR 	=  		(uint32_t*)(GPIOC_START + GPIO_BRR_OFFSET);
uint32_t volatile *GPIOC_LCKR 	=  		(uint32_t*)(GPIOC_START + GPIO_LCKR_OFFSET);
uint32_t volatile *GPIOC_AFRL	=  		(uint32_t*)(GPIOC_START + GPIO_AFRL_OFFSET); 
uint32_t volatile *GPIOC_AFRH 	=  		(uint32_t*)(GPIOC_START + GPIO_AFRH_OFFSET);

// PORT clock enable bit positions 
#define RCC_RCC_AHBENR_IOPAEN 17
#define RCC_RCC_AHBENR_IOPCEN 19

extern volatile uint32_t* STK_CVR;
extern volatile uint32_t* STK_CSR;
extern uint8_t systick_configured;

/*
void SysTick_Handler(void) {
	// toggle operation
	*GPIOC_ODR ^= (1 << 9);
}
*/

volatile uint32_t* ICSR = (uint32_t*)(0xE000ED00 + 0x04);

void reset(void) {
	// enable port A
	/*	
	*RCC_AHBENR &= RCC_AHBENR_RESET_VALUE;   	
	*RCC_AHBENR |= 1 << RCC_RCC_AHBENR_IOPAEN;	
	*/
	*RCC_AHBENR = RCC_AHBENR_RESET_VALUE;   	
	*RCC_AHBENR |= 1 << RCC_RCC_AHBENR_IOPCEN;	
	// wait until its enabled	
	// while(!(*RCC_AHBENR & ((1 << RCC_RCC_AHBENR_IOPCEN) != (1 << RCC_RCC_AHBENR_IOPCEN))));

	*GPIOC_MODER 	= 		GPIOC_MODER_RESET_VALUE;
	*GPIOC_OTYPER 	=		GPIOC_OTYPER_RESET_VALUE;	
	*GPIOC_OSPEEDR 	=		GPIOC_OSPEEDR_RESET_VALUE;  	
	*GPIOC_PUPDR 	=		GPIOC_PUPDR_RESET_VALUE; 
	*GPIOC_IDR_WRITE=		GPIOC_IDR_RESET_VALUE; 
	*GPIOC_ODR		=		GPIOC_ODR_RESET_VALUE; 
	*GPIOC_BSR 		=		GPIOC_BSR_RESET_VALUE;
	*GPIOC_BRR 		=		GPIOC_BRR_RESET_VALUE;
	*GPIOC_LCKR 	=		GPIOC_LCKR_RESET_VALUE; 
	*GPIOC_AFRL		=		GPIOC_AFRL_RESET_VALUE; 
	*GPIOC_AFRH 	=		GPIOC_AFRH_RESET_VALUE; 	
	*GPIOC_AFRL		=		GPIOC_AFRL_RESET_VALUE; 
	
	*ICSR			=		0x00000000;
}

void blink(uint8_t pin) {
	// reset the PORT registers
	// reset();
	// some are being shifted twice because some registers accept 2 bit value for each pin 	
	*GPIOC_MODER 	|= GP_OP_MODE << pin * 2;
	*GPIOC_OTYPER 	|= OP_PUSH_PULL << pin;	
	*GPIOC_OSPEEDR 	|= MEDIUM << pin * 2;  	
	*GPIOC_PUPDR 	|= PULL_DOWN << pin * 2;
	// set the given pin number bit in the bit set register which will write for the same pin in ODR 
	*GPIOC_BSR 		|= SET << pin; 	 
	
	// call enable_systick from systick.h
	configure_systick();
	while(systick_configured) {
		// default clk speed is 8MHz(HSI) => 125,000 cycles per 1 second 
		// systick set to 250,000		
		// check if counter was reset since we last checked
		if ((*STK_CSR) & (1 << 16)) {
			// toggle operation
			*GPIOC_ODR ^= (1 << pin);
		}
	}
	/*
	while(1) {
		if ((*ICSR & 1 << 26) != 0) {
			*ICSR |= 1 << 25;
		}
	}
	*/
}

