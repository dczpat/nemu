#include "cpu/instr.h"
/*
Put the implementations of `sub' instructions here.
*/
static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
    opr_dest.val = sign_ext(opr_dest.val, opr_dest.data_size);
    opr_dest.val = alu_sub(opr_src.val, opr_dest.val, data_size);
    operand_write(&opr_dest);
}

//83
make_instr_impl_2op(sub, i, rm, bv)
//2b
make_instr_impl_2op(sub, rm, r, v)
//29
make_instr_impl_2op(sub, r, rm, v)
//81
make_instr_impl_2op(sub, i, rm, v)
//2d
make_instr_impl_2op(sub, i, a, v)
//2c
make_instr_impl_2op(sub, i, a, b)