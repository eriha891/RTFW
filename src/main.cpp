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
	printf("Time to render 1 ray/pixel with SimpleRenderer: %f seconds\n", cpu_time);

    middle = clock();
    mr.renderToArray(rt->scene, rt->pixels, WIDTH, HEIGHT, 15);

    end = clock();
    cpu_time = static_cast<double>( end - middle ) / CLOCKS_PER_SEC;
    printf("Time to render 16 ray/pixel with MonteCarloRenderer: %f seconds\n", cpu_time);
    cpu_time = static_cast<double>( end - start ) / CLOCKS_PER_SEC;
    printf("Total time: %f seconds\n", cpu_time);

    return NULL;
}

void initScene(Scene &scene)
{

    Material matWhite;
    matWhite.setDiffuseColor(0.9,0.9,0.9);

    Material mirror;
    mirror.setDiffuseColor(1.0,1.0,1.0);
    mirror.setSpecularFactor(1.0);

    Material matGray;
    matGray.setDiffuseColor(0.5,0.5,0.5);

    Material matBlue;
    matBlue.setDiffuseColor(0.1,0.1,0.9);

    Material matGreen;
    matGreen.setDiffuseColor(0.1,0.8,0.1);
    matGreen.setSpecularFactor(0.3);

    Material matRed;
    matRed.setDiffuseColor(0.8,0.1,0.1);
    matRed.setSpecularFactor(0.3);

    Material matLight;
    matLight.setDiffuseColor(0.9,0.9,0.8);
    matLight.setEmission(80.0,80.0,60.0);
    matLight.setEmission(160.0,160.0,160.0);
    matLight.setSpecularFactor(0.3);
	
    Material floorlamp;
    floorlamp.setDiffuseColor(0.9,0.9,0.8);
    floorlamp.setEmission(7.0,7.0,3.0);
    floorlamp.setSpecularFactor(0.3);

    Material specularish;
    specularish.setDiffuseColor(0.9,0.9,0.1);
    specularish.setSpecularFactor(0.7); //pretty glossy

    Material glass;
    glass.setDiffuseColor(1.0,1.0,1.0);
    glass.setSpecularFactor(1.0);
    glass.setOpacity(0.05);
    glass.setRefractiveIndex(GLASS);

	// definition for the coolbox
	loadObj(scene.geometry, "media/coolbox3.obj", 0.01f);
    scene.material.push_back(specularish);		// floor
    scene.material.push_back(matBlue);		// back wall
    scene.material.push_back(matGreen);		// left wall
    scene.material.push_back(matRed);		// right wall
    scene.material.push_back(matGray);		// cieling
    scene.material.push_back(matLight);		// light box
    scene.material.push_back(matGray);		// torus
    scene.material.push_back(matGray);		// cylinder
    scene.material.push_back(matGray);		// teapot
    scene.material.push_back(matGray);		// box left
    scene.material.push_back(mirror);		// box wall
    scene.material.push_back(floorlamp);		// lamp box
    scene.material.push_back(floorlamp);		// lamp box

    // stanford bunny
    loadObj(scene.geometry, "media/bunny.obj", 0.02f);
    scene.geometry[scene.geometry.size()-1].translate(-0.22f,0.815f,-0.1f);
    scene.material.push_back(matRed);

    loadObj(scene.geometry, "media/sphere.obj", 0.4f);
    scene.geometry[scene.geometry.size()-1].translate(0.15f,0.35f,0.20f);
    scene.material.push_back(glass);

    Camera cam;
    cam.position = vec3(0.04,0.6,1.95);
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
