#include <stdint.h>
#include "../systick.h"
#include "gpio.h"

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
#define RCC_APB2ENR_RESET_VALUE		0x00000000

#define AF0 0b0000
#define AF1 0b0001
#define AF2 0b0011
#define AF3 0b0100

enum GPIO_MODE {GP_IP_MODE, GP_OP_MODE, GP_AF_MODE, GP_AN_MODE};
enum GPIO_OTYPE {OT_PUSH_PULL, OT_OPEN_DRAIN};
enum GPIO_OSPEED {LOW, MEDIUM, HIGH};
enum GPIO_PUPD {NONE, PULL_UP, PULL_DOWN, RESERVED};
enum GPIO_BS {NO_ACTION_BS, SET};
enum GPIO_BR {NO_ACTION_BR, RESET};

// To enable clock on PORTA
uint32_t volatile *RCC_AHBENR 		= 	(uint32_t*)(RCC_START + 0x14);
uint32_t volatile *RCC_APB2ENR 		= 	(uint32_t*)(RCC_START + 0x18);

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

// GPIO clock enable bit positions 
#define RCC_AHBENR_IOPAEN 		17
#define RCC_AHBENR_IOPCEN 		19
#define RCC_APB2ENR_USART1EN	14

extern volatile uint32_t* STK_CVR;
extern volatile uint32_t* STK_CSR;
extern uint8_t systick_configured;

volatile uint32_t* ICSR = (uint32_t*)(0xE000ED00 + 0x04);

void reset_gpio() {
	*RCC_AHBENR = RCC_AHBENR_RESET_VALUE;
	*RCC_APB2ENR = RCC_APB2ENR_RESET_VALUE;	// has same reset values as ahbenr 0x00000000
	
	// enable ports and peripherals
	*RCC_AHBENR |= 1 << RCC_AHBENR_IOPAEN;		// enable port A	
	*RCC_AHBENR |= 1 << RCC_AHBENR_IOPCEN;		// enable port B
	*RCC_APB2ENR |= 1 << RCC_APB2ENR_USART1EN;	// enable peripheral usart1
	// wait until its enabled	
	// while(!(*RCC_AHBENR & ((1 << RCC_RCC_AHBENR_IOPCEN) != (1 << RCC_RCC_AHBENR_IOPCEN))));

	*GPIOA_MODER 	= 		GPIOA_MODER_RESET_VALUE;
	*GPIOA_OTYPER 	=		GPIOA_OTYPER_RESET_VALUE;	
	*GPIOA_OSPEEDR 	=		GPIOA_OSPEEDR_RESET_VALUE;  	
	*GPIOA_PUPDR 	=		GPIOA_PUPDR_RESET_VALUE; 
	*GPIOA_IDR_WRITE=		GPIOA_IDR_RESET_VALUE; 
	*GPIOA_ODR		=		GPIOA_ODR_RESET_VALUE; 
	*GPIOA_BSR 		=		GPIOA_BSR_RESET_VALUE;
	*GPIOA_BRR 		=		GPIOA_BRR_RESET_VALUE;
	*GPIOA_LCKR 	=		GPIOA_LCKR_RESET_VALUE; 
	*GPIOA_AFRL		=		GPIOA_AFRL_RESET_VALUE; 
	*GPIOA_AFRH 	=		GPIOA_AFRH_RESET_VALUE; 	
	*GPIOA_AFRL		=		GPIOA_AFRL_RESET_VALUE; 
	

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
	
	// call enable_systick from systick.h
	if (!systick_configured) {
		configure_systick();
	}
}

// private
void set_pin_mode(volatile uint32_t* GPIOX_MODER, uint8_t pin,enum GPIO_MODE mode) {
	// this only works because initially all bits in MODE register are zero
	// otherwise you would need to set individuals to 0 and 1 because OR wouldnt work in the case 
	// there are bits which are not 0 that you are trying to change
	*GPIOX_MODER |= mode << pin * 2;
}

// private
void set_pin_otype(volatile uint32_t* GPIOX_OTYPER, uint8_t pin,enum GPIO_OTYPE otype) {
	*GPIOX_OTYPER |= otype << pin;	
}

// private
void set_pin_ospeed(volatile uint32_t* GPIOX_OSPEEDR, uint8_t pin,enum GPIO_OSPEED speed) {
	*GPIOX_OSPEEDR 	|= speed << pin * 2;  	
}

// private
void set_pin_pupd(volatile uint32_t* GPIOX_PUPDR, uint8_t pin,enum GPIO_PUPD pupd) {
	*GPIOX_PUPDR |= pupd << pin * 2;
}

void configure_pin(uint8_t pin, enum GPIO_MODE mode, uint8_t port) {
	// some are being shifted twice because some registers accept 2 bit value for each pin
	switch (port) {
		case 0:
			set_pin_mode(GPIOA_MODER, pin, mode);	
			// AF function
			if (mode == GP_AF_MODE) {
				// HARDCODED: for pin 9, configuration bits start form index 4
				*GPIOA_AFRH |= AF1 << 4; 
			}	
			set_pin_otype(GPIOA_OTYPER, pin, OT_PUSH_PULL);	
			// set_pin_ospeed(GPIOA_MODER, pin, MEDIUM);
			set_pin_pupd(GPIOA_PUPDR, pin, PULL_DOWN);
			break;
		case 1:
			break;
		case 2:
			// __asm volatile("BKPT");
			set_pin_mode(GPIOC_MODER, pin, mode);
			// AF function
			if (mode == GP_AF_MODE) {
				// for pin 9, configuration bits start form index 4
				*GPIOC_AFRH |= AF1 << 4; 
			}	
			set_pin_otype(GPIOC_OTYPER, pin, OT_PUSH_PULL);	// let it be default rest value
			// set_pin_ospeed(GPIOC_MODER, pin, LOW);		// dont set it to HIGH, LOW is default so need to set
			set_pin_pupd(GPIOC_PUPDR, pin, PULL_DOWN);
			break;
	}	
	// set the given pin number bit in the bit set register which will write for the same pin in ODR 
	// *GPIOC_BSR 		|= SET << pin; 	 
}

void toggle_pin(uint8_t pin, uint8_t port) {	
	switch (port) {
		case 0:
			*GPIOA_ODR ^= (1 << pin);
			break;
		case 1:
			// GPIOB not implemented yet
			// *GPIOB_ODR ^= (1 << pin);
			break;
		case 2:
			// __asm volatile("BKPT");
			*GPIOC_ODR ^= (1 << pin);
			break;
	}
}

