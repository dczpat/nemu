#include "cpu/instr.h"

//e9
make_instr_func(jmp_near)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    // thank Ting Xu from CS'17 for finding this bug
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    cpu.eip += offset;

    return 1 + data_size / 8;
}

//eb
make_instr_func(jmp_short)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = 8;
    rel.addr = eip + 1;
    operand_read(&rel);
    
    int offset = sign_ext(rel.val, 8);
    cpu.eip += offset;

    return 2;
}

//ff
make_instr_func(jmp_near_indirect)
{
    int len = 1;
    opr_src.data_size = data_size;
    len += modrm_rm(eip + 1, &opr_src);
    operand_read(&opr_src);
    cpu.eip = opr_src.val;
    return 0;
}

//ea
make_instr_func(ljmp)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;
    operand_read(&rel);
    
    cpu.eip = sign_ext(rel.val, data_size);
    cpu.cs.val = instr_fetch(eip + 5, 2);
    load_sreg(SREG_CS);
    return 0;
}





























