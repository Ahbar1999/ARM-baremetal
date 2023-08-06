#include <stdint.h>

#define GPIOA_MODER_OFFSET 		0x00
#define GPIOA_OTYPER_OFFSET 	0x04
#define GPIOA_OSPEEDR_OFFSET 	0x08
#define GPIOA_PUPDR_OFFSET 		0x0C
#define GPIOA_IDR_WRITE_OFFSET	0x10 + 0x10 /* the reset value is for first 16 bits only, rest are RO bits */ 
#define GPIOA_IDR_READ_OFFSET	0x10		 
#define GPIOA_ODR_OFFSET 		0x14
#define GPIOA_BSRR_OFFSET 		0x18
#define GPIOA_LCKR_OFFSET 		0x1C
#define	GPIOA_AFRL_OFFSET		0x20
#define GPIOA_AFRH_OFFSET 		0x24
#define GPIOA_AFRL_OFFSET		0x28

#define GPIOA_MODER_RESET_VALUE 	0x28000000 
#define GPIOA_OTYPER_RESET_VALUE 	0x00000000
#define GPIOA_OSPEEDR_RESET_VALUE 	0x0C000000
#define GPIOA_PUPDR_RESET_VALUE 	0x24000000
#define GPIOA_IDR_RESET_VALUE 		0x0000
#define GPIOA_ODR_RESET_VALUE 		0x00000000
#define GPIOA_BSRR_RESET_VALUE 		0x00000000
#define GPIOA_LCKR_RESET_VALUE 		0x00000000
#define	GPIOA_AFRL_RESET_VALUE		0x00000000
#define GPIOA_AFRH_RESET_VALUE 		0x00000000	
#define GPIOA_AFRL_RESET_VALUE		0x00000000

#define RCC_AHBENR_RESET_VALUE = 0x00000014;

enum GPIO_MODE {IP_MODE, GP_OP_MODE, AF_MODE, AN_MODE};
enum GPIO_OTYPE {OP_PUSH_PULL, OP_OPEN_DRAIN};
enum GPIO_OSPEED {LOW, MEDIUM, HIGH};
enum GPIO_PUPD {NONE, PULL_UP, PULL_DOWN, RESERVED};
enum GPIO_OTYPE {OP_PUSH_PULL, OP_OPEN_DRAIN};
enum GPIO_BS {NO_ACTION, SET};
enum GPIO_BR {NO_ACTION, RESET};

// To enable clock on PORTA
const uint32_t* RCC = 0x40021000;
const uint32_t* RCC_AHBENR = RCC + 0x14;

const uint32_t* GPIOA_START 	=  	0x48000000
const uint32_t* GPIOA_MODER 	=  	GPIOA_START + GPIO_MODER_OFFSET  
const uint32_t* GPIOA_OTYPER 	= 	GPIOA_START + GPIO_OTYPER_OFFSET
const uint32_t* GPIOA_OSPEEDR 	=	GPIOA_START + GPIO_OSPEEDR_OFFSET   	
const uint32_t* GPIOA_PUPDR 	=  	GPIOA_START + GPIO_PUPDR_OFFSET 
const uint32_t* GPIOA_IDR_READ	= 	GPIOA_START + GPIO_IDR_READ_OFFSET 
const uint32_t* GPIOA_IDR_WRITE	=	GPIOA_START + GPIO_IDR_WRITE_OFFSET 
const uint32_t* GPIOA_ODR	=  		GPIOA_START + GPIO_ODR_OFFSET 
const uint32_t* GPIOA_BSRR 	=  		GPIOA_START + GPIO_BSRR_OFFSET 
const uint32_t* GPIOA_LCKR 	=  		GPIOA_START + GPIO_LCKR_OFFSET 
const uint32_t*	GPIOA_AFRL	=  		GPIOA_START + GPIO_AFRL_OFFSET 
const uint32_t* GPIOA_AFRH 	=  		GPIOA_START + GPIO_AFRH_OFFSET 	
const uint32_t* GPIOA_AFRL	=  		GPIOA_START + GPIO_AFRL_OFFSET 

void reset(void);
void blink(void);
