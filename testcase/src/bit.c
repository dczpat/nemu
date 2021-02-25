#include "trap.h"

typedef unsigned char uint8_t;
typedef char bool;

bool getbit(void *buf, int offset)
{
	int byte = offset >> 3;
	offset &= 7;
	uint8_t mask = 1 << offset;
	return (((uint8_t *)buf)[byte] & mask) != 0;
}

void setbit(void *buf, int offset, bool bit)
{
    //BREAK_POINT
	int byte = offset >> 3;
	//nemu_assert(byte==0x1);
	offset &= 7;
	//nemu_assert(offset==0x0)
	uint8_t mask = 1 << offset;
    //nemu_assert(mask==0x1)
    //BREAK_POINT
	uint8_t *p = buf + byte;
	*p = (bit == 0 ? (*p & ~mask) : (*p | mask));
	//BREAK_POINT
	//nemu_assert((*p&0x1)==0x1);
	// bit = bit == 0 ? 1 : 0;
	// nemu_assert(getbit(buf, offset) == bit);
}

int main()
{
	uint8_t buf[2];

	buf[0] = 0xaa;
	nemu_assert(getbit(buf, 0) == 0);
	nemu_assert(getbit(buf, 1) == 1);
	nemu_assert(getbit(buf, 2) == 0);
	nemu_assert(getbit(buf, 3) == 1);
	nemu_assert(getbit(buf, 4) == 0);
	nemu_assert(getbit(buf, 5) == 1);
	nemu_assert(getbit(buf, 6) == 0);
	nemu_assert(getbit(buf, 7) == 1);
    
	setbit(buf, 8, 1);
	//BREAK_POINT
	//nemu_assert((buf[1]&0x1) == 0x1);
	setbit(buf, 9, 0);
	//nemu_assert((buf[1]&0x3) == 0x1);
	setbit(buf, 10, 1);
	//nemu_assert((buf[1]&0x7) == 0x5);
	setbit(buf, 11, 0);
	//nemu_assert((buf[1]&0xf) == 0x5);
	setbit(buf, 12, 1);
	//nemu_assert((buf[1]&0x1f) == 0x15);
	setbit(buf, 13, 0);
	//nemu_assert((buf[1]&0x3f) == 0x15);
	setbit(buf, 14, 1);
	//nemu_assert((buf[1]&0x7f) == 0x55);
	setbit(buf, 15, 0);
	nemu_assert(buf[1] == 0x55);

	HIT_GOOD_TRAP;
	return 0;
}
