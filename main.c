#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void triangulation(char *FileName, const char *ResultName);
void test(int n);

int main(void)
{   
		//test(42);
		char *FileName="data_new.txt";
		//struct timespec start, finish;
		//double elapsed;
		char *ResultName="solution";
				
		//clock_gettime(CLOCK_MONOTONIC, &start);
		
		triangulation(FileName, ResultName);

		//clock_gettime(CLOCK_MONOTONIC, &finish);
/* 
		elapsed = (finish.tv_sec - start.tv_sec);
		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		printf("time:%f\n",elapsed);
*/		
	 exit(0);
       
}


