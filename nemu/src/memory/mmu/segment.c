#include "cpu/cpu.h"
#include "memory/memory.h"
#include <stdio.h>

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	 //printf("--------\n");
	 SegDesc* psd = (void*)(hw_mem + cpu.gdtr.base + 3 * cpu.segReg[sreg].index);
	 uint32_t base = psd->base_15_0 + (psd->base_23_16 << 16) + (psd->base_31_24 << 24);
	 uint32_t limit = psd->limit_15_0 + (psd->limit_19_16 << 16);
	 assert(psd->present == 1);
	 assert(psd->granularity == 1);
	 //printf("---limit=%x\n", limit);
	 assert(base == 0);
	 cpu.segReg[sreg].base = base;
	 cpu.segReg[sreg].limit = limit;
	 cpu.segReg[sreg].privilege_level = psd->privilege_level;
}

