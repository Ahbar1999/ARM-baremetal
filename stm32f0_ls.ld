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
	.text : 
	{
		/* '*' refers to all the .xyz sections of all the files */
		*(.isr_vector)
		*(.text)
		*(.rodata)
	}> FLASH
	/* > VMA AT> LMA */
	/* VMA= virutal memory address; LMA= load memory address */
	/* in this case VMA = LMA = FLASH */	
	
	/* data region is relocated since the code sits in FLASH initially bu
	 * but then is loaded into SRAM for execution
	 */
	
	/* COMPILE-TIME/USER INITIALIZED DATA */
	.data
	{
		*(.data)

	}> SRAM AT> FLASH
	/* .data data is first loaded into flash then is put into SRAM for use */
	
	/* UNINITIALIZED/COMPILER INITIALIZED DATA/0-INITIALIZED DATA*/
	.bss
	{
		*(.bss)
	}> SRAM
	/* since .bss is uninitialized data therefore
	 * .bss data is not loaded on flash but is directly initialized in SRAM 
	 * */
}





