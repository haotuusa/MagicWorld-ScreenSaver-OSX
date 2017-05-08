
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

//for drawing or reading image
#include <FreeImage.h>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

GLuint initShader(GLenum shaderType, const char * filename);
std::string textFileRead (const char * filename);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLFWwindow * initWindow(int x, int y, const char * title);
GLuint initShaderProgram(GLuint * shaders, int num);
void initTriangleBuffers(GLfloat * vertices, int size, GLuint & VBO, GLuint & VAO);
void scaleVertices(GLfloat * vertices, int size, GLfloat scale);
void translateVertices(GLfloat * vertices, int size, GLfloat x, GLfloat y, GLfloat z);
void updateVertices(GLfloat * vertices, int size);

enum Mode { scale, translation, rotation, null};
Mode actionMode = null;

bool upFlag = FALSE, downFlag = FALSE, rightFlag = FALSE, leftFlag = FALSE;

//create a triangle vertices in Normalized Device Coordinate
GLfloat vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};  

int main(int argc,char* argv[]) {

	GLFWwindow * window = initWindow(800, 600, "Learn OpenGL");
	if(window == NULL)
		return -1;

	//set keyboard event
	glfwSetKeyCallback(window, key_callback); 

	//init and compile the vertex and fragment shaders
	GLuint vertexShader = initShader(GL_VERTEX_SHADER, "shaders/light.vert.glsl");
	GLuint fragmentShader = initShader(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl");
	GLuint shaders[] = {vertexShader, fragmentShader};

	//create shaderProgram based on shaders
	GLuint shaderProgram = initShaderProgram(shaders, 2);

	GLuint VBO, VAO;

	//game rendering loop
	while(!glfwWindowShouldClose(window)){
		//check and call events
		glfwPollEvents();

		/*-----------------------begin rendering command----------------------------*/

		//clear color first
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		updateVertices(vertices, 9);

		//animation vertices update
		initTriangleBuffers(vertices, sizeof(vertices), VBO, VAO);

		//draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);



		/*-----------------------end of rendering command----------------------------*/

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

    if(key == GLFW_KEY_S && action == GLFW_PRESS){
    	std::cerr << "scale mode" << std::endl;
    	actionMode = scale;
    }

    if(key == GLFW_KEY_T && action == GLFW_PRESS){
    	std::cerr << "translation mode" << std::endl;
    	actionMode = translation;
    }

    if(key == GLFW_KEY_R && action == GLFW_PRESS){
    	std::cerr << "rotation mode" << std::endl;
    	actionMode = rotation;
    }
    if(key == GLFW_KEY_N && action == GLFW_PRESS){
    	std::cerr << "do nothing mode" << std::endl;
    	actionMode = null;
    }

    if(key == GLFW_KEY_UP){
    	if(action == GLFW_PRESS){
	    	std::cerr << "Up pressed" << std::endl;
	    	upFlag = TRUE;
    	}
    	else if(action == GLFW_RELEASE){
	    	std::cerr << "Up released" << std::endl;
	    	upFlag = FALSE;
    	}
    }

    if(key == GLFW_KEY_DOWN){
    	if(action == GLFW_PRESS){
	    	std::cerr << "Down pressed" << std::endl;
	    	downFlag = TRUE;
    	}
    	else if(action == GLFW_RELEASE){
	    	std::cerr << "Down released" << std::endl;
	    	downFlag = FALSE;
    	}
    }

    if(key == GLFW_KEY_LEFT){
    	if(action == GLFW_PRESS){
	    	std::cerr << "Left pressed" << std::endl;
	    	leftFlag = TRUE;
    	}
    	else if(action == GLFW_RELEASE){
	    	std::cerr << "Left released" << std::endl;
	    	leftFlag = FALSE;
    	}
    }

    if(key == GLFW_KEY_RIGHT){
    	if(action == GLFW_PRESS){
	    	std::cerr << "Right pressed" << std::endl;
	    	rightFlag = TRUE;
    	}
    	else if(action == GLFW_RELEASE){
	    	std::cerr << "Right released" << std::endl;
	    	rightFlag = FALSE;
    	}
    }

}

std::string textFileRead (const char * filename) {
	std::string str,ret = ""; 
	std::ifstream in; 
	in.open(filename); 
	if (in.is_open()) {
		getline (in,str); 
		while (in) {
			ret += str + "\n"; 
			getline (in,str); 
		}
		//    cout << "Shader below\n" << ret << "\n"; 
		return ret; 
	}
	else {
		std::cerr << "Unable to Open File " << filename << "\n"; 
		throw 2; 
	}
}

GLuint initShader(GLenum shaderType, const char * filename){

	//Generate shader and get the ID
	GLuint shader = glCreateShader(shaderType);

	//read in all the code from the file into a string
	std::string shaderSource = textFileRead(filename);

	// Weirdness to get a const char for shader to compile
	const GLchar * cstr = shaderSource.c_str(); 

	//attach the shader source code
	glShaderSource(shader, 1, &cstr, NULL);
	glCompileShader(shader);

	//log to check if shader compile successfully
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:" << filename << ":COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}


GLFWwindow * initWindow(int x, int y, const char * title){
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
	GLFWwindow * window = glfwCreateWindow(x, y, title, nullptr, nullptr);
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	//make the window the main context on current thread
	glfwMakeContextCurrent(window);

	/*set to GL_TRUE, ensure using more modern technique for managing 
		OpenGL functionality*/
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}

	/* we don't explicitely use 600, 800 so it works on dip screens*/
	//get the width height for frame buffer of the window specified
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	//first 2 params set location of lower left corner
	glViewport(0, 0, width, height);
	return window;
}

GLuint initShaderProgram(GLuint * shaders, int num){
	//create a shader program and link the compiled shaders
	GLuint shaderProgram = glCreateProgram();

	for(int i = 0; i < num; i++){
		glAttachShader(shaderProgram, shaders[i]);
	}

	glLinkProgram(shaderProgram);

	//log for error
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//after we link the shader into program, no longer need shaders
	for(int i = 0; i < num; i++){
		glDeleteShader(shaders[i]);
	}

	return shaderProgram;
}

void initTriangleBuffers(GLfloat * vertices, int size, GLuint & VBO, GLuint & VAO){

		//Generate Buffer and get the ID
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		// Bind Vertex Array Object
		glBindVertexArray(VAO);

		//vertex buffer object is a array buffer type, bind it
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//copies the previously defined vertex data into the buffer's memory
		//use GL_STATIC_DRAW because position of the triangle is unlikely to change
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		/*GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
			GL_DYNAMIC_DRAW: the data is likely to change a lot.
			GL_STREAM_DRAW: the data will change every time it is drawn.*/

		// set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//unbind VAO
		glBindVertexArray(0);
}

void scaleVertices(GLfloat * vertices, int size, GLfloat x, GLfloat y, GLfloat z){
	for(int i = 0; i < size; i += 3){
		vertices[i] *= x;
		vertices[i+1] *= y;
		vertices[i+2] *= z;
	}
}

void translateVertices(GLfloat * vertices, int size, GLfloat x, GLfloat y, GLfloat z){
	for(int i = 0; i < size; i += 3){
		vertices[i] += x;
		vertices[i+1] += y;
		vertices[i+2] += z;
	}
}
void updateVertices(GLfloat * vertices, int size){
	GLfloat x, y;
	switch(actionMode){
		case scale:
  		std::cerr << "scale vertices" << std::endl;
  		if(upFlag)
				y = 1.01f;
			else if(downFlag)
				y = 0.99f;
			else 
				y = 1.0f;

			if(rightFlag)
				x = 1.01f;
			else if(leftFlag)
				x = 0.99f;
			else
				x = 1.0f;

			scaleVertices(vertices, size, x, y, 1.0f);
			break;
		case translation:
  		std::cerr << "translate vertices" << std::endl;
  		if(upFlag)
				y = 0.01f;
			else if(downFlag)
				y = -0.01f;
			else 
				y = 0.0f;

			if(rightFlag)
				x = 0.01f;
			else if(leftFlag)
				x = -0.01f;
			else
				x = 0.0f;

			translateVertices(vertices, size, x, y, 0.0f);
			break;
		case rotation:
			std::cerr << "rotate vertices" << std::endl;
			break;
		case null:
		  std::cerr << "do nothing withm vertices" << std::endl;
			break;
	}
}