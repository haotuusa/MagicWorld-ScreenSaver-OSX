
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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLuint initShader(GLenum shaderType, const char * filename);
std::string textFileRead (const char * filename);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLFWwindow * initWindow(int x, int y, const char * title);
GLuint initShaderProgram(GLuint * shaders, int num);
void initTriangleBuffers(GLfloat * vertices, int size, GLuint & VBO, GLuint & VAO);
void scaleVertices(GLfloat * vertices, int size, GLfloat scale);
void translateVertices(GLfloat * vertices, int size, GLfloat x, GLfloat y, GLfloat z);
void updateTransformation();
void transformVertices(GLfloat * input, GLfloat * output, int size, glm::mat4 transf);
void animation();

enum Mode { scale, translation, rotation, null};
Mode actionMode = null;

bool upFlag = FALSE, downFlag = FALSE, rightFlag = FALSE, leftFlag = FALSE;

glm::mat4 scaleMat4;
glm::mat4 translationMat4;
glm::mat4 rotationMat4;


//create a triangle vertices in Normalized Device Coordinate
GLfloat vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};  

int main(int argc,char* argv[]) {

	GLFWwindow * window = initWindow(800, 800, "Learn OpenGL");
	if(window == NULL)
		return -1;

	// GLint nrAttributes;
	// glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	// std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

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

		//update transformation matrix
		// updateTransformation();
		// animation();
		GLfloat newVertices[9];
		glm::mat4 transf = translationMat4 * rotationMat4 * scaleMat4;
		transformVertices(vertices, newVertices, 9, transf);

		//animation vertices update
		initTriangleBuffers(newVertices, sizeof(newVertices), VBO, VAO);

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


void transformVertices(GLfloat * input, GLfloat * output, int size, glm::mat4 transf){
	for(int i = 0; i < size; i += 3){
		glm::vec4 newPosition = transf * glm::vec4(input[i], input[i+1], input[i+2], 1.0f);
		output[i] = newPosition.x;
		output[i+1] = newPosition.y;
		output[i+2] = newPosition.z;
	}
}

void animation(){
	glm::mat4 identity;
	static GLfloat scalex = 1.0f,
				 scaley = 1.0f, 
				 scalez = 1.0f, 
				 transx = 0.0f, 
				 transy = 0.0f, 
				 transz = 0.0f, 
				 degree = 0.0f;
	static bool scaleUpy = TRUE, scaleUpx = TRUE,
							transUpy = TRUE, transUpx = TRUE,
							rotateClock = TRUE;

	// switch(actionMode){
		// case scale:
  		// std::cerr << "scale vertices" << std::endl;
  		if(scaleUpy && scaley <= 1.5f)
				scaley += 0.012f;
			else
				scaleUpy = FALSE;

			if(!scaleUpy && scaley >= 0.0f)
				scaley -= 0.012f;
			else
				scaleUpy = TRUE;

			if(scaleUpx && scalex <= 1.7f)
				scalex += 0.01f;
			else
				scaleUpx = FALSE;

			if(!scaleUpx && scalex >= 0.0f)
				scalex -= 0.01f;
			else
				scaleUpx = TRUE;


			// if(upFlag || downFlag || rightFlag || leftFlag)
			scaleMat4 = glm::scale(identity, glm::vec3(scalex, scaley, scalez));  
			// std::cerr << "animate scaling" << std::endl;
			// break;

		// case translation:
  	// 	std::cerr << "translate vertices" << std::endl;
  		if(transUpy && transy <= 0.5f)
				transy += 0.015f;
			else
				transUpy = FALSE;

			if(!transUpy && transy >= -0.5f)
				transy -= 0.015f;
			else
				transUpy = TRUE;

  		if(transUpx && transx <= 0.5f)
				transx += 0.01f;
			else
				transUpx = FALSE;

			if(!transUpx && transx >= -0.5f)
				transx -= 0.01f;
			else
				transUpx = TRUE;

			translationMat4 = glm::translate(identity, glm::vec3(transx, transy, transz));
			// std::cerr << "animate translation" << std::endl;
			// break;
		// case rotation:
		  if(rotateClock && degree >= -180.0f)
				degree -= 2.0f;
			else
				rotateClock = FALSE;

			if(!rotateClock && degree <= 180.0f)
				degree += 2.0f;
			else
				rotateClock = TRUE;

			// if(rightFlag || leftFlag)
			rotationMat4 = glm::rotate(identity, glm::radians(degree), glm::vec3(0.0, 0.0, 1.0));
			// std::cerr << "animate rotation" << std::endl;
			// break;
		// case null:
		  // std::cerr << "do nothing with vertices" << std::endl;
			// break;
	// }
}



void updateTransformation(){
	glm::mat4 identity;
	static GLfloat scalex = 1.0f,
				 scaley = 1.0f, 
				 scalez = 1.0f, 
				 transx = 0.0f, 
				 transy = 0.0f, 
				 transz = 0.0f, 
				 degree = 0.0f;

	switch(actionMode){
		case scale:
  		std::cerr << "scale vertices" << std::endl;
  		if(upFlag)
				scaley += 0.01f;
			else if(downFlag)
				scaley -= 0.01f;

			if(rightFlag)
				scalex += 0.01f;
			else if(leftFlag)
				scalex -= 0.01f;

			if(upFlag || downFlag || rightFlag || leftFlag)
				scaleMat4 = glm::scale(identity, glm::vec3(scalex, scaley, scalez));  
			break;
		case translation:
  		std::cerr << "translate vertices" << std::endl;
  		if(upFlag)
				transy += 0.01f;
			else if(downFlag)
				transy -= 0.01f;

			if(rightFlag)
				transx += 0.01f;
			else if(leftFlag)
				transx -= 0.01f;
			if(upFlag || downFlag || rightFlag || leftFlag)
				translationMat4 = glm::translate(identity, glm::vec3(transx, transy, transz));
			// translateVertices(vertices, size, x, y, z);
			break;
		case rotation:
			if(rightFlag)
				degree -= 1.0f;
			else if(leftFlag)
				degree += 1.0f;

			if(rightFlag || leftFlag)
				rotationMat4 = glm::rotate(identity, glm::radians(degree), glm::vec3(0.0, 0.0, 1.0));
			// transformVertices(vertices, 9, trans);
			std::cerr << "rotate vertices" << std::endl;
			break;
		case null:
		  std::cerr << "do nothing with vertices" << std::endl;
			break;
	}
}