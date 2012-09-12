
#include <iostream>
#include <iomanip>
#include <GL/glfw.h>
#include <stdio.h>
#include <cmath>
#include <pthread.h>

using namespace std;

void *setpixels(void *pv) {
	int max_x = 256;
	int max_y = 256;
	
	float *p = (float*)pv;
	for(int x = 0; x < max_x; x++) {
		for(int y = 0; y < max_y; y++) {
			int pos = 255*x + y;
			pos = pos * 3;
			p[pos + 0] = 1.0;
			p[pos + 1] = 0.0;
			p[pos + 2] = 0.0;			
			//cout << "hello" << endl;
			usleep(10000);
		}
	}
}

int main(int argc, char ** argv) {
	
	int width = 256;
	int height = 256;
	
	glfwInit();
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // activate anti-alias 4x
	// GLFW_WINDOW
	if(!glfwOpenWindow(width, height, 8,8,8,8, 32,0, GLFW_WINDOW)) {
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return false;
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// making default background color black
    glfwSwapInterval(1);

	bool quitProgram = false;
	float *pixels = new float[width*height*3];
	pthread_t thread;
   int rc;
	rc = pthread_create(&thread, NULL, setpixels, (void*)pixels);
	if (rc){
	         printf("ERROR; return code from pthread_create() is %d\n", rc);
	         //exit(-1);
	      }
		  
	while(!quitProgram) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(width,height,GL_RGB,GL_FLOAT,pixels);
		glfwSwapBuffers();
		
		// check if time to quit
		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED)) {
			quitProgram = true;
		}
	}
	
	delete pixels;
	
	glfwTerminate();
    pthread_exit(NULL);
    
	return 0;
}
