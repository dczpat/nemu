#include "cpu/instr.h"
#include <stdio.h>
/*
Put the implementations of `ret' instructions here.
*/

//c3
make_instr_func(ret_near)
{
    OPERAND mem;
    mem.type = OPR_MEM;
    mem.data_size = data_size;
    mem.sreg = SREG_DS;
    mem.addr = cpu.esp;
    operand_read(&mem);
    //printf("eip=%d\n",mem.val);
    cpu.eip = mem.val;
    cpu.esp += data_size / 8;
    return 0;
}

//c2
make_instr_func(ret_near_imm16)
{
    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_DS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    operand_read(&mem);
    cpu.eip = mem.val;
    
    OPERAND imm;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.data_size = 16;
    imm.addr = eip + 1;
    operand_read(&imm);
    int offset = sign_ext(imm.val, 16);
    cpu.esp += offset + data_size / 8;
    
    return 0;
}