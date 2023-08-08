#include <stdint.h>
#include "systick.h"

#define STK_CSR_ADDR 	0xE000E010U
#define STK_RVR_ADDR	0xE000E014U // SysTick reload value register (STK_RVR)
#define STK_CVR_ADDR	0xE000E018U // SysTick current value register (STK_CVR)
#define STK_CALIB_ADDR	0xE000E01CU

// reset values, missing values denote that reset value is kept unknown
#define STK_CSR_RV		0x00000004U
#define STK_RVR_RV		16000000U 	/*24 bit timer, set countdown to approx 2 seconds*/  
#define STK_CVR_RV		0x00000000U /* clear current value register */   
#define STK_CALIB_RV 	0x00000000U

// pointers to volatile data
extern uint32_t volatile *STK_CSR = (uint32_t*)STK_CSR_ADDR;
extern uint32_t volatile *STK_RVR = (uint32_t*)STK_RVR_ADDR; 
extern uint32_t volatile *STK_CVR = (uint32_t*)STK_CVR_ADDR;
extern uint32_t volatile *STK_CALIB = (uint32_t*)STK_CALIB_ADDR;
extern uint8_t systick_configured = 0;

void configure_systick() {
	if (systick_configured) return;

	systick_configured = 1; 
	*(STK_CSR) 	= STK_CSR_RV; 	
	*(STK_RVR) 	= STK_RVR_RV;	
	*(STK_CVR) 	= STK_CVR_RV;	
	*(STK_CALIB)= STK_CALIB_RV;	
	
	// configure, ENABLE CSR, rest of the values are default, clock: system	
	*(STK_CSR) 	|= 1 << 0;	
} 
