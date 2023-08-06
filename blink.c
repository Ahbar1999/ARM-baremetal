#ifndef _BLINK_
#define _BLINK_

#include <stdint.h>

#define GPIOA_START				0x48000000

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

#define RCC_START 					0x40021000
#define RCC_AHBENR_RESET_VALUE		0x00000014

enum GPIO_MODE {IP_MODE, GP_OP_MODE, AF_MODE, AN_MODE};
enum GPIO_OTYPE {OP_PUSH_PULL, OP_OPEN_DRAIN};
enum GPIO_OSPEED {LOW, MEDIUM, HIGH};
enum GPIO_PUPD {NONE, PULL_UP, PULL_DOWN, RESERVED};
enum GPIO_BS {NO_ACTION_BS, SET};
enum GPIO_BR {NO_ACTION_BR, RESET};

// To enable clock on PORTA
uint32_t* RCC_AHBENR 		= 	(uint32_t*)RCC_START + 0x14;

uint32_t* GPIOA_MODER 		=  	(uint32_t*)GPIOA_START + GPIO_MODER_OFFSET;  
uint32_t* GPIOA_OTYPER 	    = 	(uint32_t*)GPIOA_START + GPIO_OTYPER_OFFSET;
uint32_t* GPIOA_OSPEEDR 	=	(uint32_t*)GPIOA_START + GPIO_OSPEEDR_OFFSET;   	
uint32_t* GPIOA_PUPDR 		=  	(uint32_t*)GPIOA_START + GPIO_PUPDR_OFFSET;
uint32_t* GPIOA_IDR_READ	= 	(uint32_t*)GPIOA_START + GPIO_IDR_READ_OFFSET; 
uint32_t* GPIOA_IDR_WRITE	=	(uint32_t*)GPIOA_START + GPIO_IDR_WRITE_OFFSET; 
uint32_t* GPIOA_ODR	    =  		(uint32_t*)GPIOA_START + GPIO_ODR_OFFSET;
uint32_t* GPIOA_BSR 	=  		(uint32_t*)GPIOA_START + GPIO_BSR_OFFSET;
uint32_t* GPIOA_BRR 	=  		(uint32_t*)GPIOA_START + GPIO_BRR_OFFSET;
uint32_t* GPIOA_LCKR 	=  		(uint32_t*)GPIOA_START + GPIO_LCKR_OFFSET;
uint32_t* GPIOA_AFRL	=  		(uint32_t*)GPIOA_START + GPIO_AFRL_OFFSET; 
uint32_t* GPIOA_AFRH 	=  		(uint32_t*)GPIOA_START + GPIO_AFRH_OFFSET;

void reset(void) {
	// enable port A
	*RCC_AHBENR &= RCC_AHBENR_RESET_VALUE;   	
	*RCC_AHBENR |= 1 << 17;	
	
	*GPIOA_MODER 	&= 		GPIOA_MODER_RESET_VALUE;
	*GPIOA_OTYPER 	&=		GPIOA_OTYPER_RESET_VALUE;	
	*GPIOA_OSPEEDR 	&=		GPIOA_OSPEEDR_RESET_VALUE;  	
	*GPIOA_PUPDR 	&=		GPIOA_PUPDR_RESET_VALUE; 
	*GPIOA_IDR_WRITE	&= 	GPIOA_IDR_RESET_VALUE; 
	*GPIOA_ODR		&=  	GPIOA_ODR_RESET_VALUE; 
	*GPIOA_BSR 		&=  	GPIOA_BSR_RESET_VALUE;
	*GPIOA_BRR 		&=  	GPIOA_BRR_RESET_VALUE;
	*GPIOA_LCKR &=  		GPIOA_LCKR_RESET_VALUE; 
	*GPIOA_AFRL	&=  		GPIOA_AFRL_RESET_VALUE; 
	*GPIOA_AFRH &=  		GPIOA_AFRH_RESET_VALUE; 	
	*GPIOA_AFRL	&=  		GPIOA_AFRL_RESET_VALUE; 
}

void blink(uint8_t pin) {
	// reset the PORT registers
	// reset();
	
	*GPIOA_MODER 	|= GP_OP_MODE << pin;
	*GPIOA_OTYPER 	|= OP_PUSH_PULL << pin;	
	*GPIOA_OSPEEDR 	|= MEDIUM << pin;  	
	*GPIOA_PUPDR 	|= PULL_DOWN << pin;
	// set the given pin number bit in the bit set register which will write for the same pin in ODR 
	*GPIOA_BSR 		|= SET << pin; 	 
}

#endif