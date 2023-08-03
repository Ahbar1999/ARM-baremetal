/* ENTRY() commnad defines the entrypoint of the program */
ENTRY(Reset_Handler)

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

