#include "kernel.h"

// system handler priority register 3 
#define SHPR3 0xE000ED20
// interrupt control and state register
#define ICSR 0xE000ED04
#define PENDSVSET 28

/* volatile pointer meaning the value of the pointer variable itself is subject to change */
OSThread* volatile OS_curr;
OSThread* volatile OS_next;

// array of threads
OSThread* threads[32 + 1];
// count of total threads active 
uint8_t OS_thread_num; 
// index of the thread to run
uint8_t i_thread;

void OS_init(void) {
	// set PendSV to lowest priority so that it is the last interrupt to be serviced
	// Refer: Page No. 61: STM32F0 Cortex-M programming manual 
	*((uint32_t volatile *)SHPR3) = 0x00000000 | 0xF << 20;
}

void OS_sched(void) {
	// OSThread const *next = OS_next;
	
	OSThread const *next = threads[i_thread];
	// update counter
	i_thread = (i_thread + 1) % OS_thread_num;
	if (next != OS_curr) {
		// set PendSV pending for context switching
		*(uint32_t volatile *)ICSR |= 1 << PENDSVSET;
	}
}

void OSThread_start (
	OSThread *this,
	OSThreadHandler threadHandler,
	void *stkSto, uint32_t stkSize)
{
	// stack grows from the top so we add the stack size to the stack start addr to get the addr of the top
	uint32_t *sp = (uint32_t)((((uint32_t)stkSto + stkSize) / 8) * 8);
	uint32_t *stk_limit;	
	
	/* only PSR(thumb bit = 1) and PC(return addr) register have special value
	 * rest of the registers are set arbitrary values, we have chosen to set them
	 * to their(register) serial numbers
	 * 	xPSR
	 *	PC
	 *	LR
	 *	R12
	 *	R3
	 *	R2
	 *	R1
	 *	R0
	 */
	// pre decrement because sp always points to the last used address rather than the first unused addr 
	*(--sp) = (1U << 24);	/* xPSR */
	*(--sp) = (uint32_t)threadHandler;	/* PC */
	*(--sp) = 0x0000000EU;	/* LR(arbitrary value), LR is used by the CPU only to handle exception returns */
	*(--sp) = 0x0000000CU;
	*(--sp) = 0x00000003U;
	*(--sp) = 0x00000002U;
	*(--sp) = 0x00000001U;
	*(--sp) = 0x00000000U;
	// additionally save R11-R4
	*(--sp) = 0x0000000BU;
	*(--sp) = 0x0000000AU;
	*(--sp) = 0x00000009U;
	*(--sp) = 0x00000008U;
	*(--sp) = 0x00000007U;
	*(--sp) = 0x00000006U;
	*(--sp) = 0x00000005U;
	*(--sp) = 0x00000004U;	
	
	// save stack pointer(top) of this stack frame
	this->sp = sp;
	
	// round up the stack boundary to 8 byte
	stk_limit = (uint32_t*)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);

	// pad unused part of the stack
	for (sp = sp - 1U; sp >= skt_limit; --sp) {
		*sp = 0x00ABCDEF;
	}

	// save the thread
	thread[OS_thread_num++] = this;
}

// optimize attribute is used to particularly optimize marked piece of code
__attribute__ ((naked, optimize("-fno-stack-protector")))
void PendSV_Handler() {
	__asm volatile(
		// disable IRQs
		"CPSID 	I					\n"
		// following 2 lines are equivalent of: r1 = OS_curr->sp
		// first load pointer: OS_curr, then load sp member of the struct pointed by OS_curr
		"LDR	r1,=OS_curr 		\n"
		"LDR	r1,[r1, #0x00] 		\n"
			
		// branch if OS_curr == NULL, meaning no process is active, 
		// so restore a process
		"CBZ	r1,PendSV_restore 	\n"
		// else : process is active
		// start saving current state to prep for context switch 	
		"PUSH	{r4-r11}			\n"
		// save current thread's stack pointer to the top of stack 
		"LDR	r1,=OS_curr			\n"
		"LDR	r1,[r1,#0x00] 		\n"
		// OS_curr->sp = sp
		"STR	sp,[r1,#0x00]		\n" 

		// load next thread	
		"PendSV_restore:			\n"
		// load sp from OS_next
		"LDR 	r1,=OS_next			\n"
		"LDR	r1,[r1,#0x00]		\n"
		// sp = OS_next->sp 
		"LDR	sp,[r1,#0x00]		\n"

		// OS_curr = OS_next, update pointer
		// remember that OS_Thread struct is only characterized by the 
		// 'sp'
		"LDR	r1,=OS_next			\n"
		"LDR	r1,[r1,#0x00]		\n"
		"LDR	r2,=OS_curr			\n"
		"STR	r1,[r2,#0x00]		\n"
		
		// restore the extra registers
		// pop stack 7 times and load values in registerst successively 
		// lowest numbered register(eg r4 here) uses the lowest memory addr 
		// and the highest register(eg. r11) using the highest memory addr
		"POP	{r4-r11}			\n"
		
		// renable interrupts 
		"CPSIE	I					\n"
		// LR is loaded with a sepcial value that, when loaded in PC, tells the cpu that
		// the exception is completed
		"BX		LR					\n"
	);
}
