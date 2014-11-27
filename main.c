#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "glfem.h"


Triangulation *triangulation(char *FileName, const char *ResultName);
void test(int n);

int main(void)
{   
		//test(42);
		char *FileName="data_new.txt";
		//struct timespec start, finish;
		//double elapsed;
		char *ResultName="solution";
				
		//clock_gettime(CLOCK_MONOTONIC, &start);
		
		Triangulation *theTriangulation= triangulation(FileName, ResultName);

		 glfemInit("Delaunay");
    do
    {
        int w,h;
        glfwGetWindowSize(&w,&h);
        glfemReshapeWindows(theTriangulation,w,h);
        glfemPlotMesh(theTriangulation); 
        glfwSwapBuffers();
    } 
    while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED) );
           
    // Check if the ESC key was pressed or the window was closed 
    glfwTerminate(); 
    
    
		//clock_gettime(CLOCK_MONOTONIC, &finish);
/* 
		elapsed = (finish.tv_sec - start.tv_sec);
		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		printf("time:%f\n",elapsed);
*/		
	 exit(0);
       
}


