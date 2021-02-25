#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/
static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    alu_and(opr_src.val, opr_dest.val, data_size);
}

//85
make_instr_impl_2op(test, r, rm, v)
//84
make_instr_impl_2op(test, r, rm, b)
//f7
make_instr_impl_2op(test, i, rm, v)
//a9
make_instr_impl_2op(test, i, a, v)
//a8
make_instr_impl_2op(test, i, a, b)
//f6
make_instr_impl_2op(test, i, rm, b)