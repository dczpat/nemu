#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

make_instr_func(lgdt)
{
    int len = 1;
    OPERAND mem;
    mem.data_size = data_size;
    mem.type = OPR_MEM;
    len += modrm_rm(eip+1, &mem);
    mem.sreg = SREG_CS;
    
    mem.data_size = 16;
    operand_read(&mem);
    cpu.gdtr.limit = mem.val;
    
    mem.data_size = 32;
    mem.addr += 2;
    operand_read(&mem);
    cpu.gdtr.base = mem.val;
    
    return len;
}






































































































     
     
     
     
     
