#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/

make_instr_func(leave)
{
    int len = 1;
    opr_src.type = OPR_MEM;
    opr_src.sreg = SREG_DS;
    opr_src.data_size = data_size;
    opr_src.addr = cpu.ebp;
    operand_read(&opr_src);
    cpu.esp = cpu.ebp + data_size / 8;
    cpu.ebp = opr_src.val;
    return len;
}