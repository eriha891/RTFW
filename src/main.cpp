#ifdef WIN32
#include <GL/glew.h>
#endif

#include <GL/glfw.h>
#include <cstdio>
#include <pthread.h>
#include <vector>
#include <ctime>

#include "SimpleRenderer.h"
#include "MonteCarloRenderer.h"
#include "Scene.h"
#include "ObjLoader.h"

#define WIDTH 512
#define HEIGHT 512

typedef struct
{
    Scene *scene;
    float *pixels;
}RenderTarget;

void *renderParallell( void *arg )
{

    RenderTarget *rt = (RenderTarget*)arg;

	clock_t start = clock();
	clock_t middle = clock(),end;
    double cpu_time;

    SimpleRenderer sr;
    MonteCarloRenderer mr;

    sr.buildBVH(rt->scene);
    mr.buildBVH(rt->scene);

    end = clock();
    cpu_time = static_cast<double>( end - middle ) / CLOCKS_PER_SEC;
    printf("Time to build BVH: %f seconds\n", cpu_time);

    middle = clock();
    sr.renderToArray(rt->scene, rt->pixels, WIDTH, HEIGHT, 1);

	end = clock();
	cpu_time = static_cast<double>( end - middle ) / CLOCKS_PER_SEC;
	printf("Time to render 1 ray/pixel: %f seconds\n", cpu_time);

    middle = clock();
    mr.renderToArray(rt->scene, rt->pixels, WIDTH, HEIGHT, 1);

    end = clock();
    cpu_time = static_cast<double>( end - middle ) / CLOCKS_PER_SEC;
    printf("Time to render 16 ray/pixel: %f seconds\n", cpu_time);
    cpu_time = static_cast<double>( end - start ) / CLOCKS_PER_SEC;
    printf("Total time: %f seconds\n", cpu_time);

    return NULL;
}

void initScene(Scene &scene)
{
    //loadObj(scene.geometry, "media/bunny.obj", 8.0f);

	/*
	loadObj(scene.geometry, "media/golv_bakvagg_tak_lador.obj", 1.0f);
    loadObj(scene.geometry, "media/gron_vagg.obj", 1.0f);
    loadObj(scene.geometry, "media/rod_vagg.obj", 1.0f);
    loadObj(scene.geometry, "media/lampa.obj", 1.0f);
	*/


    //std::cout<<"scene geoms "<<scene.geometry.size()<<std::endl;

    Material matGray;
    matGray.setDiffuseColor(0.5,0.5,0.5);
    //matGray.setEmission(0.01,0.01,0.01);

    Material matGreen;
    matGreen.setDiffuseColor(0.1,0.8,0.1);
    //matGreen.setEmission(0.01,0.01,0.01);

    Material matRed;
    matRed.setDiffuseColor(0.8,0.1,0.1);
    //matRed.setEmission(0.01,0.01,0.01);

    Material matLight;
    matLight.setDiffuseColor(0.9,0.9,0.8);
    matLight.setEmission(60.0,60.0,50.0);

	// definition for the coolbox
	loadObj(scene.geometry, "media/coolbox.obj", 0.01f);
    scene.material.push_back(matGray);		// floor
    scene.material.push_back(matGray);		// back wall
    scene.material.push_back(matGreen);		// left wall
    scene.material.push_back(matRed);		// right wall
    scene.material.push_back(matGray);		// cieling
    scene.material.push_back(matLight);		// light box
    scene.material.push_back(matGray);		// torus
    scene.material.push_back(matGray);		// cylinder
    scene.material.push_back(matGray);		// teapot
    scene.material.push_back(matGray);		// box left
    scene.material.push_back(matGray);		// box right

    // stanford bunny
    //loadObj(scene.geometry, "media/bunny.obj", 0.02f);
    //scene.geometry[scene.geometry.size()-1].translate(0.55f,0.27f,0.60f);
    //scene.material.push_back(matGray);

    Camera cam;
    cam.position = vec3(0.04,0.6,1.9);
    cam.direction = vec3(0,0,-1);
    cam.up = vec3(0,1,0);

    scene.camera.push_back(cam);

    cam.position = vec3(.1,.6,1.0);
    cam.direction = vec3(-0.1,0.5,-1);
    cam.up = vec3(0,1,0);

    scene.camera.push_back(cam);

    scene.activeCamera = 0;
}

int main(int argc, char ** argv) {


	/*
    int th_id, nthreads;
    #pragma omp parallel private(th_id) shared(nthreads)
    {
      th_id = omp_get_thread_num();
      #pragma omp critical
      {
		  //cout << "Hello World from thread " << th_id << '\n';
		  printf("Hello World from thread %i\n", th_id);

	}
	#pragma omp barrier

      #pragma omp master
      {
        nthreads = omp_get_num_threads();
        //cout << "There are " << nthreads << " threads" << '\n';
		printf("There are %i threads\n", nthreads);

      }
    }
	*/

	glfwInit();

	// GLFW_WINDOW
	if(!glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8, 32,0, GLFW_WINDOW)) {
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return 0;
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// making default background color black
    glfwSwapInterval(0);

	float *pixels = new float[WIDTH*HEIGHT*3];

    Scene scene;

    initScene(scene);

    RenderTarget rt;
    rt.scene = &scene;
    rt.pixels = pixels;
    renderParallell((void*)&rt);
    /*
	pthread_t thread;
	int rc = pthread_create(&thread, NULL, renderParallell, (void*)&rt);
	if (rc)
	{
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        return 0;
    }
*/
    // ray tracing is already done
    bool quitProgram = false;
	while(!quitProgram) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(WIDTH,HEIGHT,GL_RGB,GL_FLOAT,pixels);
		glfwSwapBuffers();

		// check if time to quit
		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED)) {
			quitProgram = true;
		}
	}

	delete pixels;

	glfwTerminate();

	return 0;
}
