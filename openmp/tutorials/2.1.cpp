#include <cstdio>
#include <omp.h>

using namespace std;

int main(int argc, char** argv)
{
	#pragma omp parallel num_threads(8)
	{
		printf("fuck you. thread id = %d\n", omp_get_thread_num());
	}

	return 0;
}