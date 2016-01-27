#include <stdio.h>
#include <omp.h>
int main()
{
#pragma omp parallel
printf( "[%d] Hello\n ", omp_get_thread_num());
return 0;
} 