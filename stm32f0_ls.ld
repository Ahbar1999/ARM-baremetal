/* ENTRY() commnad defines the entrypoint of the program */
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
	FLASH(rx): ORIGIN=0x08000000, LENGTH=64K
	/* SRAM memory is obviously readable, writable and executable */	
	SRAM(rwx): ORIGIN=0x20000000, LENGTH=8K
}

/*SECTIONS command is used to define various sections(their locations) in the memory*/
/*SECTIONS command maps various input sections to output sections(reordering) in the elf file */
/* this command allows us to place define the */
SECTIONS 
{
	/* FLASH memory starts with text section stored */
	.text : 
	{
		/* '*' refers to all the .xyz sections of all the files */
		*(.isr_vector)
		*(.text)
		/* data contained in .rodata section is part of text 
		 * we can choose to include it in the .text section or
		 * create a separate block for it
		 * */
		/* *(.rodata) */
		
		/* declare a symbol to track the ending address of .text section */ 
		_etext = .;	/* addr of the start of text section + size of data = _etext */ /* note the semicolon */
	}> FLASH
	/* > VMA AT> LMA */
	/* VMA= virutal memory address; LMA= load memory address */
	/* in this case VMA = LMA = FLASH */	
	
	/*
	 * .rodata section contains initialized global variables and constants
	 * */	
	.rodata : 
	{
		*(.rodata)
		_erodata = .;	/* end of .rodata section = start of .data section */
						/* this will be required for transefrring .data from FLASH to SRAM */
	}> FLASH

	/* data region is relocated since the code sits in FLASH initially bu
	 * but then is loaded into SRAM for execution
	 */

	/* COMPILE-TIME/USER INITIALIZED DATA 
	 * This data will need to be transferred...
	 * from FLASH(where it is initially loaded, hence LMA) to SRAM(during program execution, VMA) */
	.data :
	{
		_sdata = 0x20000000;	/* addr of start of .data section in SRAM... */
								/* since location counters track VMA not LMA */
		*(.data)
		_edata = .;	/* end of data section */
	}> SRAM AT> FLASH
	/*_edata - _sdata = size of data section */
	
	/* AT RUNTIME: 
	 * .data will need to be transferred FROM src addr: [_erodata to _erodata + (_edata - _sdata)] 
	 * TO start addr: [_sdata]
	 * */
	
	/* UNINITIALIZED/COMPILER INITIALIZED DATA/0-INITIALIZED DATA*/
	.bss :
	{
		*(.bss)
	}> SRAM
	/* since .bss is uninitialized data therefore
	 * .bss data is not loaded on flash but is directly initialized in SRAM 
	 * */
}

/* Location counters are provided by linkers which can be used to
 * track start and ending memory locations of various labels
 * Location counters always track VMA(final location) not LMA(initial location)
 *
 * -> Location counters can only be used inside the SECTIONS command
 * */






