#include <stdio.h> 
#include <time.h> 
void main () 
{ 
time_t start, finish; 
static long num_steps = 1000000000; 
double step; 
int i; 
double x, pi, sum = 0.0; 
step = 1.0/(double) num_steps; 
start = clock(); 
for (i=0;i < num_steps; i++) 
{ 
x = (i+0.5)*step; 
sum = sum + 4.0/(1.0+x*x); 
} 
pi = step * sum; 
finish = clock(); 
printf( "Pi = %16.15f (%d steps), %ld ms\n ", pi, num_steps, finish-start ); 
return; 
} 