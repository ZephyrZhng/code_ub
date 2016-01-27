#include <stdio.h>

int main()
{
	int i = 2, j = 2;
	int p = (i++)+(++j);
	printf("%d\n", p);

	int a = 2;
	int b = (a++)+3*a;
	printf("%d\n", b);
	return 0;
}