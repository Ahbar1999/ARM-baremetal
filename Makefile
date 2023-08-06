# variable to store cross compiler name
CC=arm-none-eabi-gcc
# machine or target name
MACH=cortex-m0
# compiler flags:
# 	-c: to only create objects files and not link them i.e. do not run the linker
# 	-O0: No optimizations
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0 
# Linker flags
# 	-Wl is a special flag that lets the compiler know that following is a linker specific command 
# 	-Map=final.map: allows linker to create a memory map file called final.map 
LDFLAGS= -nostdlib -T stm32f0_ls.ld -Wl,-Map=final.map

# command:<targets>
# 'make all' will build all the given targets
# currently we have only one target
all: main.o blink.o stm32f0_startup.o final.elf

# 	NOTES ON SYNTAX etc.
# 	target: dependency
# 	shorthands:
# 		$^: dependency
# 		$@: target
# 	syntax transformations: main.c -o main.o => $^ -o $@ => -o $@ $^ 

# run compiler

blink.o:blink.c
	$(CC) $(CFLAGS) -o $@ $^

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^

stm32f0_startup.o:stm32f0_startup.c
	$(CC) $(CFLAGS) -o $@ $^

# build target from linker
final.elf: main.o stm32f0_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

# load command to download target on board
load:
	openocd -f board_cfg_files/stm32f0discovery.cfg

clean:
	rm -rf *.o *.elf


