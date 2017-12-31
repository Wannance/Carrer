#include <stdio.h>

int main()
{

	union example
	{
		struct
		{
			int x;
			int y;
		}in;
		
		int a;
		int b;
		int c;
	}e;
	
	e.a = 1;
	//e.b = 2;
	e.in.x = e.a * e.b;
	e.in.y = e.a + e.b;
	printf(" e.in.x = %d\n e.in.y = %d\n", e.in.x, e.in.y);
	printf(" e.c = %d\n", e.c);
	printf(" sizeof(e) = %d\n", sizeof(e));
	
	return 0;
}