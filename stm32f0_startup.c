// create vector table
// for cortex-m0 there are exactly 6 system interrupt vectors
// word size is 32 byte
// smallest addressable unit is 1 byte

/*
 * While creating the vector table(which would be represented by an array of pointers in c)
 * Keep in mind that we cant allow vector table to be put in the default .data section of the code
 * Vector table needs to be put in a separate .isr_vector section that is used by the processor which comes
 * before the .data section
 * */

#include <stdint.h>

/* IMPORTANT: 	Note that linker symbols are always memory locations and not 'values'. 
 * 				You can get the address of a symbol but you cant access its value
 * 				because it has no value. 
 * 				And the address of the symbol would be the addr that you assigned to
 * 				it in the linker script.
 * https://stackoverflow.com/questions/8398755/access-symbols-defined-in-the-linker-script-by-application
 */

// import linker symbols
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _erodata;

extern uint32_t _sbss;
extern uint32_t _ebss;

// addresses are represented in bytes so we write all values in bytes
#define SRAM_START 0x20000000U	// U: unsigned int
#define SRAM_SIZE (64 * 1024) 	// 64KB
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))
#define STACK_START SRAM_END 	// stack grows from the top

// test variables
const uint32_t constant = 32;
int uninit_var;

// 0x00000000 -> 0x00000004: 32 bits Reserved for Stack Ptr
// 0x00000004 onwards Interrupt vector pointers live, each ptr takes 32 bits or 4 bytes
// 0x00000004: Reset
// 0x00000008: NMI 
// 0x0000000C: HardFault 
// and so on...

// main prototype
int main(void);

// using variable __attribute__ allows us to put specific part of the code
// into special/customized sections in the memory
void Reset_Handler(void);
// fn attribute(weak, alis("xyz")) means that in case the implementation for 'fn'
// if provided the execution will go to that implementation instead of the alias
// thats why its marked 'weak' 
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* System interrupt handlers have been written
 * Leaving rest of the interrupt handlers to be written later
 * */
uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
	STACK_START,
	(uint32_t)&Reset_Handler,
	(uint32_t)&NMI_Handler,		/* Here the addr of Default_Handler will be stored */
								/* and same for all functionas aliasing Default_Handler */
	(uint32_t)&HardFault_Handler,
	(uint32_t)&SVCall_Handler,
	(uint32_t)&PendSV_Handler,
	(uint32_t)&SysTick_Handler,
};

void Default_Handler(void) {
	// just hang 
	while(1);
}

void Reset_Handler(void) {
	// copy .data section to SRAM
	// init the .bss section to zero in SRAM
	// call main()
	/* call init functions of std library if being used */
	
	uint32_t dotdata_size = (uint32_t)&_edata - (uint32_t)&_sdata;
	// copy .data from FLASH to SRAM 
	uint32_t* dest = (uint32_t*)&_sdata;	//	addr in SRAM
	uint32_t* src = (uint32_t*)&_erodata; 	//	addr in FLASH
	
	// traverse bytewise
	for (uint32_t i = 0; i < dotdata_size; i++, dest++, src++) {
		*dest = *src;
	}
	
	uint32_t sbss_size = (uint32_t)&_sbss - (uint32_t)&_ebss;
	uint32_t* ptr = (uint32_t*)&_sbss; 	
	for (uint32_t i = 0; i < sbss_size; i++, ptr++){
		*ptr = 0;	
	}
	
	// call main
	main();
}

int main(void) {
	int a = 0;
	a += 1;
	
	return 0;
}





