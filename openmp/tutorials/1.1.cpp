#include <cstdio>
#include <omp.h>

using namespace std;

int main(int argc, char** argv)
{
	#pragma parallel omp for
	for(int i = 0; i < 10; ++i)
	{
		printf("i = %d\n", i);
	}
	return 0;
}