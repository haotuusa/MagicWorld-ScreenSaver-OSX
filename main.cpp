
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



int main(int argc,char* argv[]) {

	glfwInit();
	// set the version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window not resizable by the user
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//for macos
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

	//ensure using more modern technique for managing OpenGL functionality
	// glewExperimental = GL_TRUE;


	return 0;
}
