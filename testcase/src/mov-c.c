#include "trap.h"

int A[10];
int b;

int main()
{
    //BREAK_POINT
	A[0] = 0;
	//BREAK_POINT
	A[1] = 1;
	//BREAK_POINT
	A[2] = 2;
	//BREAK_POINT
	A[3] = 3;
	//BREAK_POINT
	A[4] = 4;
	//BREAK_POINT

	b = A[3];
	//BREAK_POINT
	A[5] = b;
    //BREAK_POINT
    
	nemu_assert(A[0] == 0);
	nemu_assert(A[1] == 1);
	nemu_assert(A[2] == 2);
	nemu_assert(A[3] == 3);
	nemu_assert(A[4] == 4);
	nemu_assert(b == 3);
	nemu_assert(A[5] == 3);

	HIT_GOOD_TRAP;

	return 0;
}
