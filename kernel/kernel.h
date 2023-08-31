#ifndef _KERNEL_H_
#define _KERNEL_H_

typedef struct {
	void* sp;	// stack pointer
	/* other info */
} OSThread;

// typedef for pointer to a function of type: void xyz(void)
typedef void* OSThreadHandler();

void OS_init();

void OS_sched();

/*
 * stkSto: memory location of private stack 
 * stkSize: size of the stack
 * */
void OSThread_start(
	OSThread *this,
	OSThreadHandler threadHandler,
	void *stkSto, uint32_t stkSize);

#endif
