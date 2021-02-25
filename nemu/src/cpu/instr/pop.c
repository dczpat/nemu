#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op()
{
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_DS;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    operand_read(&opr_dest);
    opr_src.val = opr_dest.val;
    operand_write(&opr_src);
    cpu.esp += data_size / 8;
}

//58+r
make_instr_impl_1op(pop, r, v)

make_instr_func(popa)
{
    opr_src.data_size = 32;
    opr_src.type = OPR_MEM;
    opr_src.sreg = SREG_DS;

    for (int i = 7; i >= 0; i--)
    {
        if (i != 4)
        {
            opr_src.addr = cpu.esp;
            operand_read(&opr_src);
            cpu.gpr[i].val = opr_src.val;
        }
        cpu.esp += 4;
    }

    print_asm_0("popa", "", 1);

    return 1;
}