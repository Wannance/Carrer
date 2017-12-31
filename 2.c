#include <stdio.h>

float func(int n)
{
	int i = 0;
	float count = 1.0;
	if(n < 1)
		return 0;
	for(; i < n + 1; i ++)
	{
		count += n * i / (i + 3);
	}
	
	return count;
}

int main()
{
	float count = 0;
	int num;
	printf("Please enter a number:");
	scanf("%d", &num);
	count = func(num);
	printf("%f\n", count);
	return 0;
}