/* ENTRY() commnad defines the entrypoint of the program */
/* We will define Reset_Handler in the startup file */
ENTRY(Reset_Handler)

/* MEMORY command defines the available memories with their locations and sizes */
MEMORY 
{
	/* syntax: */
	/* name(attr): ORIGIN = origin, LENGTH = len */
	/* following are the attributes given by 'attr':
	 * R or r: Read-only sections
	 * W or w: read/write sections
	 * X: sections containing executable code
	 * I: initialized sections
	 * L: same as I
	 * !: Invert the sense of any of the above attributes
	 * above attributes that can be combined like linux file ownerships 
	 */

	/* We have 2 memory regions in stm32f0: FLASH: 64KB AND SRAM: 8KB */
	/* FLASH memory is readable and executable */	
	FLASH(rx): ORIGIN =0x08000000, LENGTH =64K
	/* SRAM memory is obviously readable, writable and executable */	
	SRAM(rwx): ORIGIN =0x20000000, LENGTH =8K
}

/*SECTIONS command is used to define various sections(their locations) in the memory*/
/*SECTIONS command maps various input sections to output sections(reordering) in the elf file */
/* this command allows us to place define the */
SECTIONS 
{
	/* FLASH memory starts with text section stored */
	/* .text section contains code related data like function definitions etc.,
	 * data that does not change and cannot written to */
	_stext = 0x08000000;
	/* output section */
	.text : 
	{
		/* input sections */
		/* interrupt vector table is required to be present at the start of FLASH */
		*(.isr_vector)

		/* put all .text section of input files here together in output file */
		*(.text)
		*(.text.*)
		/* data contained in .rodata section is part of text 
		 * we can choose to include it in the .text section or
		 * create a separate block for it for clarity
		 */
		/* *(.rodata) */
	}> FLASH
	/* declare a symbol to track the ending address of .text section */ 
	_etext = .;	/* addr of the start of text section + size of data = _etext */ /* note the semicolon */
	
	/* > VMA AT> LMA */
	/* VMA= virutal memory address; LMA= load memory address */
	/* in this case VMA = LMA = FLASH */	
	
	/* Read-only data
	 * .rodata section contains initialized global variables and constants
	 */	
	.rodata : 
	{
		/* put all .rodata section of input files here together in output file */
		*(.rodata)
		*(.rodata.*)
		/* SET LOCATION COUNTER TO NEXT WORD BOUNDARY */
		/* SO THAT ENDING OF .rodata SECTION IS WORD ALIGNED */
		. = ALIGN(4);
	}> FLASH
	_erodata = .;	/* end of .rodata section = start of .data section */
					/* this will be required for transefrring .data from FLASH to SRAM */
	
	/* data region is relocated since the code sits in FLASH initially bu
	 * but then is loaded into SRAM for execution
	 */

	/* COMPILE-TIME/USER INITIALIZED DATA 
	 * This data will need to be transferred...
	 * from FLASH(where it is initially loaded, hence LMA) to SRAM(during program execution, VMA) */
	
	.data :
	{
		_sdata = .;	/* addr of start of .data section in SRAM... 0x20000000 */
		*(.data)
		. = ALIGN(4);
		_edata = .;	/* end of data section, which would an addr in the SRAM which is VMA for .data section */
	}> SRAM AT> FLASH
	/* rememeber that location counters track VMA not LMA */
	/*_edata - _sdata = size of data section */
	
	/* AT RUNTIME:
	 * Through the startup script
	 * .data will need to be transferred FROM src addr: [_erodata to _erodata + (_edata - _sdata)] 
	 * TO start addr: [_sdata]
	 */
	
	.bss :
	{
		/* UNINITIALIZED/COMPILER INITIALIZED DATA/0-INITIALIZED DATA */
		_sbss = .;	/* start of bss section in SRAM */
					/* usually comes after .data section */
					/* therefore _sbss = _edata */
		*(.bss)
		*(COMMON)
		. = ALIGN(4); 	/* align the start of next section to a word boundary */
		_ebss = .; /* end of bss section */
	}> SRAM
	/* since .bss is uninitialized data... 
	 * therefore .bss data is not loaded on flash but is directly initialized in SRAM 
	 * */
}

/* Location counters are provided by linkers which can be used to
 * track start and ending memory locations of various labels
 * Location counters always track VMA(final location) not LMA(initial location)
 * -> Location counters can only be used inside the SECTIONS command 
 * -> Location counter symbols are exported by the linker so they can be used and referenced to
 *    in your .c files.
 */






