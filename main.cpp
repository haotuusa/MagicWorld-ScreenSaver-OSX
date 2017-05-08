
#include <iostream>
#include <string>
#include <sstream>

//for drawing or reading image
#include <FreeImage.h>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc,char* argv[]) {

	glfwInit();
	// set the version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window not resizable by the user
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//for OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create window object: width, height, name (last two ignore)
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//make the window the main context on current thread
	glfwMakeContextCurrent(window);

	/*set to GL_TRUE, ensure using more modern technique for managing 
		OpenGL functionality*/
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	/* we don't explicitely use 600, 800 so it works on dip screens*/
	//get the width height for frame buffer of the window specified
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	//first 2 params set location of lower left corner
	glViewport(0, 0, width, height);

	//set keyboard event
	glfwSetKeyCallback(window, key_callback); 

	//game loop
	while(!glfwWindowShouldClose(window)){
		//check and call events
		glfwPollEvents();

		//rendering command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//double buffer: front and back buffer to prevent flickering issue
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}

