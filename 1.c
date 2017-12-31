/*************************************************************************
	> File Name: 1.c
	> Author: Do
	> Mail: wannance@163.com 
	> Created Time: Fri 29 Dec 2017 03:46:32 PM DST
	> Function:求16位数字转化为二进制后1的个数
 ************************************************************************/

#include<stdio.h>

char func(int num);

int main()
{
	char time;
	int num;
	printf("Please enter a number:");
	scanf("%d", &num);
	time = func(num);
	printf("The time of '1': %d\n", time);
	return 0;
}

char func(int num)
{
	char time = 0;

	if(num < 0 || num > 0xff)
		return 0;
	
	while(num)
	{
		++ time;
		num &= (num -1);
	}

	return time;
}
