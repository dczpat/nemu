#include "cpu/intr.h"
#include "cpu/instr.h"

/*
Put the implementations of `int' instructions here.

Special note for `int': please use the instruction name `int_' instead of `int'.
*/

make_instr_func(int_)
{
    OPERAND rel;
    rel.data_size = 8;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.addr = eip + 1;
    operand_read(&rel);
    
    print_asm_1("int_", "", 2, &rel);
    raise_sw_intr(rel.val);
    
    return 0;
}