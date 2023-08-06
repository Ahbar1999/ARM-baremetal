#include "blink.h"
#include <stdint.h>

void reset(void) {
	// enable port A
	RCC_AHBENR &= RCC_AHBENR_RESET_VALUE;   	
	RCC_AHBENR |= 1 << 17;	
	
	*GPIOA_MODER &= 		GPIOA_MODER_RESET_VALUE;
	*GPIOA_OTYPER &=		GPIOA_OTYPER_RESET_VALUE;	
	*GPIOA_OSPEEDR &=		GPIO_OSPEEDR_RESET_VALUE;  	
	*GPIOA_PUPDR &=			GPIO_PUPDR_RESET_VALUE; 
	*GPIOA_IDR_WRITE &= 	GPIOA_IDR_RESET_VALUE; 
	*GPIOA_ODR	&=  		GPIO_ODR_RESET_VALUE; 
	*GPIOA_BSRR &=  		GPIO_BSRR_RESET_VALUE; 
	*GPIOA_LCKR &=  		GPIO_LCKR_RESET_VALUE; 
	*GPIOA_AFRL	&=  		GPIO_AFRL_RESET_VALUE; 
	*GPIOA_AFRH &=  		GPIO_AFRH_RESET_VALUE; 	
	*GPIOA_AFRL	&=  		GPIO_AFRL_RESET_VALUE; 
}

void blink(uint8_t pin) {
	reset();
	
	*GPIOA_MODER 	|= GP_OP_MODE << pin;
	*GPIOA_OTYPER 	|= OP_PUSH_PULL << pin;	
	*GPIOA_OSPEEDR 	|= MEDIUM << pin;  	
	*GPIOA_PUPDR 	|= PULL_DOWN << pin; 
	*GPIOA_BS 		|= SET << pin; 	 
}
