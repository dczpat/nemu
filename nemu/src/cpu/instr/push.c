#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/
static void instr_execute_1op()
{
    operand_read(&opr_src);
    
    cpu.esp -= data_size / 8;
    opr_dest.type = OPR_MEM;
    opr_dest.data_size = data_size;
    opr_dest.val = opr_src.val;
    opr_dest.sreg = SREG_DS;
    opr_dest.addr = cpu.esp;
    
    operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm,v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)

make_instr_func(pusha)
{
    int len = 1;
    uint32_t esp = cpu.esp;
    cpu.esp-=4;
	vaddr_write(cpu.esp,SREG_DS,4,cpu.eax);

    cpu.esp-=4;
    vaddr_write(cpu.esp,SREG_DS,4,cpu.ecx);
    
    cpu.esp-=4;
    vaddr_write(cpu.esp,SREG_DS,4,cpu.edx);
	cpu.esp-=4;
	vaddr_write(cpu.esp,SREG_DS,4,cpu.ebx);

	cpu.esp-=4;
	vaddr_write(cpu.esp,SREG_DS,4,esp);
    cpu.esp-=4;
	vaddr_write(cpu.esp,SREG_DS,4,cpu.ebp);
    
	cpu.esp-=4;
	vaddr_write(cpu.esp,SREG_DS,4,cpu.esi);
	cpu.esp-=4;
	vaddr_write(cpu.esp,SREG_DS,4,cpu.edi);
    print_asm_0("pusha","",1);
    return len;
}