#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
make_instr_func(call_near)
{
    OPERAND rel, mem;
    
    rel.data_size = data_size;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.addr = eip + 1;
    operand_read(&rel);
    //push eip/ip
    mem.data_size = data_size;
    mem.type = OPR_MEM;
    mem.sreg = SREG_DS;
    mem.val = eip + data_size / 8 + 1;
    cpu.esp -= data_size / 8;
    mem.addr = cpu.esp;
    operand_write(&mem);
    //call_near
    int offset = sign_ext(rel.val, data_size);
    cpu.eip += offset + data_size / 8 + 1;
    return 0;
}

//ff
make_instr_func(call_near_indirect)
{
    OPERAND rel, mem;
    int len = 1;
    rel.data_size = data_size;
    len += modrm_rm(eip + 1, &rel);
    operand_read(&rel);
    
    cpu.esp -= data_size / 8;
    mem.data_size = data_size;
    mem.type = OPR_MEM;
    mem.sreg = SREG_DS;
    mem.addr = cpu.esp;
    mem.val = cpu.eip + len;
    operand_write(&mem);
    
    cpu.eip = rel.val;
    if(data_size == 16)
        cpu.eip &= 0xffff;
    return 0;
}
