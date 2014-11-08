/** main.cpp **/

#include <cstdio>
#include <GL/glfw.h>
#include <cstdlib>
#include <ctime>
#include "settings.hpp"

/*

	This program is quite a simple two dimensional fluid simulator
	The simulation is not supposed to be accurate but an approximation that runs in realtime
	The fluid consists of many individual particles and are rendered simulating metaballs to make them look like liquid

	NOTES:
	The fluid can be drained towards the mouse cursor by pressing down mouse1
	The fluid can be repelled away from the mouse cursor by pressing down mouse2
	The program defaults to a water like liquid but there are few presets that can easily be chosen by editing the PRESET define in loop.cpp

*/

//From loop.cpp
void loop();

int main() {
	srand(time(NULL));

	//Init GLFW
	if(glfwInit() == GL_FALSE) {
		printf("Couldn't initialize GLFW!\n");
		return 1;
	}
	//Window settings
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	//Open window
	if(glfwOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 8, 8, 8, 8, 0, 0, GLFW_WINDOW) == GL_FALSE) {
		printf("Couldn't open window!\n");
		return 1;
	}
	glfwSetWindowTitle("Fluid simulator");

	//Some OpenGL settings
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

	//Start loop
	loop();

	glfwCloseWindow();
	glfwTerminate();
	return 0;
}
