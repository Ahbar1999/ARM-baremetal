# variable to store cross compiler name
CC=arm-none-eabi-gcc
# machine or target name
MACH=cortex-m0
# compiler flags:
# 	-c: to only create objects files and not link them i.e. do not run the linker
# 	-O0: No optimizations
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0 

# command:<targets>
# 'make all' will build all the given targets
all:main.o led.o stm32f0_startup.o

# target: dependency
# shorthands
# $^: dependency
# $@: target
# main.c -o main.o => $^ -o $@ => -o $@ $^ 

led.o:led.c
	$(CC) $(CFLAGS) -o $@ $^

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^

stm32f0_startup.o:stm32f0_startup.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf

















