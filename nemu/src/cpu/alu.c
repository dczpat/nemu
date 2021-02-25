#include "cpu/cpu.h"

void set_ZF(uint32_t res, size_t data_size)
{
    cpu.eflags.ZF = (cut_high(res, data_size) == 0);
}

void set_SF(uint32_t res, size_t data_size)
{
    cpu.eflags.SF = sign(sign_ext(res, data_size));
}

void set_PF(uint32_t res)
{
    res &= 0xff;
    int cnt = 0;
    while(res > 0)
    {
        if((res & 1) == 1)
            cnt++;
        res >>= 1;
    }
    cpu.eflags.PF = ((cnt & 1) == 0);
}

void set_OF_add(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    res = sign_ext(res, data_size);
    src = sign_ext(src, data_size);
    dest = sign_ext(dest, data_size);
    if(sign(src) == sign(dest) && sign(src) != sign(res))
        cpu.eflags.OF = 1;
    else
        cpu.eflags.OF = 0;
}

void set_OF_sub(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    res = sign_ext(res, data_size);
    src = sign_ext(src, data_size);
    dest = sign_ext(dest, data_size);
    if(sign(src) != sign(dest) && sign(dest) != sign(res))
        cpu.eflags.OF = 1;
    else
        cpu.eflags.OF = 0;
}

void set_CF_add(uint32_t res, uint32_t src, size_t data_size)
{
    res = sign_ext(res, data_size);
    src = sign_ext(src, data_size);
    cpu.eflags.CF = (res < src);
}

void set_CF_adc(uint32_t res, uint32_t src, size_t data_size)
{
    res = sign_ext(res, data_size);
    src = sign_ext(src, data_size);
    if(cpu.eflags.CF == 0)
        cpu.eflags.CF = (res < src);
    else
        cpu.eflags.CF = (res <= src);
}

void set_CF_sub(uint32_t src, uint32_t dest, size_t data_size)
{
    src = sign_ext(src, data_size);
    dest = sign_ext(dest, data_size);
    cpu.eflags.CF = (dest < src);
}

void set_CF_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
    src = sign_ext(src, data_size);
    dest = sign_ext(dest, data_size);
    if(cpu.eflags.CF == 0)
        cpu.eflags.CF = (dest < src);
    else
        cpu.eflags.CF = (dest <= src);
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
    uint32_t res = src + dest;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    set_OF_add(res, src, dest, data_size);
    set_CF_add(res, src, data_size);
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
    uint32_t res = src + dest + cpu.eflags.CF;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    set_OF_add(res, src, dest, data_size);
    set_CF_adc(res, src, data_size);
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
    uint32_t res = dest - src;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    set_OF_sub(res, src, dest, data_size);
    set_CF_sub(src, dest, data_size);
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
    uint32_t res = dest - src - cpu.eflags.CF;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    set_OF_sub(res, src, dest, data_size);
    set_CF_sbb(src, dest, data_size);
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
    uint64_t res = (uint64_t)src * dest;
    uint32_t high = (res >> data_size) & (0xffffffff >> (32 - data_size));
    cpu.eflags.CF = (high > 0);
    cpu.eflags.OF = (high > 0);
    return res;
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
    uint64_t res = (int64_t)src * dest;
    return res;
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
    uint32_t res = dest / src;
    return res;
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
    uint32_t res = dest / src;
    return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
    uint32_t res = dest % src;
    return res;
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
    uint32_t res = dest % src;
    return res;
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
    uint32_t res = src & dest;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    cpu.eflags.OF = 0;
    cpu.eflags.CF = 0;
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
    uint32_t res = src ^ dest;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    cpu.eflags.OF = 0;
    cpu.eflags.CF = 0;
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
    uint32_t res = src | dest;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    cpu.eflags.OF = 0;
    cpu.eflags.CF = 0;
	return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
    dest = cut_high(dest, data_size);
    if(src == 0)
        return dest;
    uint32_t res = (dest << (src - 1));
    cpu.eflags.CF = ((res & (0x80000000 >> (32 - data_size))) > 0);
    res <<= 1;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
    dest = cut_high(dest, data_size);
    if(src == 0)
        return dest;
    uint32_t res = (dest >> (src - 1));
    cpu.eflags.CF = ((res & 0x1) > 0);
    res >>= 1;
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
    uint32_t res = sign_ext(cut_high(dest, data_size), data_size);
    while(src--)
    {
        cpu.eflags.CF = (res & 0x1);
        res = (int32_t)res >> 1;
    }
    set_PF(res);
    set_SF(res, data_size);
    set_ZF(res, data_size);
    return cut_high(res, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
    return alu_shl(src, dest, data_size);
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
#endif
}
