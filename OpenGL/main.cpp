

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void InitSystems();
void ProcessInput(GLFWwindow*);
void Wireframe(bool);
void Render();
void framebuffer_size_callback(GLFWwindow*, int, int);
void InitShaders();
unsigned int InitShader(GLenum, const char*);
void Dispose();

bool shouldExit = false;

unsigned int VAO;

unsigned int VBO;
unsigned int vertexShader;

//How do I have these separate?
const char *vertexShaderSource = "#version 450 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"out vec4 vertexColour;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
	"	vertexColour = vec4(gl_Position.xyz, 1.0);\n"
    "}\0";

unsigned int fragmentShader;
const char *fragmentShaderSource = "#version 450 core\n"
    "out vec4 FragColor;\n"
	"in vec4 vertexColour;\n"
	"uniform float time;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vertexColour * (sin(time));\n"
    "}\n\0";

unsigned int shaderProgram;

float triVertices[] = {	-.5f, -.5f, 0.0f,
					 	0.5f, -.5f, 0.0f, 
						0.0f, 0.5f, 0.0f
					};

float sqrVertices[] = {0.5f, 0.5f, 0.0f,
						0.5f, -0.5f, 0.0f,
						-0.5f, -0.5f, 0.0f,
						-0.5f, 0.5f, 0.0f
					};

unsigned int indices[] = {0, 1, 3,
							1, 2, 3
						};

unsigned int EBO;

int main(int args, char** argv)
{
	
	InitSystems();

	GLFWwindow* window = glfwCreateWindow(800,600, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glCreateShader can't be called before creating the context and gladLoadGLLoader!
	InitShaders();

	//Wireframe(true);
	while(!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		Render();
	}
	
	Dispose();
	glfwTerminate();

	return 0;
}

void InitSystems()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void ProcessInput(GLFWwindow* window)
{
	//gather input into actions
	//how is this done properly?
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

void Wireframe(bool useWireframe)
{
	long mode = GL_FILL;
	if(useWireframe)
	{
		mode = GL_LINE;
	}

	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void Render()
{
	glClearColor(0.2f, 0.3f ,0.3f ,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	int timeLocation = glGetUniformLocation(shaderProgram, "time");
	float timeValue = glfwGetTime();
	glUniform1f(timeLocation, timeValue); //will need to use specific shaderProgram first as it sets on cur active
	//if we have multple programs / VAOs we should bind here before drawing the objects that use them
	//if we only need one can we just set them up once and use them for everything without having to repeat calls
	//glUseProgram(shaderProgram);
	//glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw primitives using active shader, the defined VAO and the VBOS data (bound via VAO)
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void InitShaders()
{
	vertexShader = InitShader(GL_VERTEX_SHADER, vertexShaderSource);
	fragmentShader = InitShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//refactor out of here: can have an init shaders but even then this error checking is cumbersome
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error: Shader: Program: Linking Failed:\n" << infoLog << std::endl;
	}

	//This suggests we could read from file compile then delete without needing anything here
	//Just a utils call
	//What files do we compile? Do we grab all or do we have folders?
	//glUseProgram(shaderProgram); //need to e called every frame or only if changing program?
	glDeleteShader(vertexShader); //we only need shaders to link to program after this is done we no longer need them
	glDeleteShader(fragmentShader);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sqrVertices), sqrVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Vertex attr to config (matches location in vertex shader), size of attr, data type, normalise? (irrelevant for floats),
	//stride (0 = OpenGL determined - iff tighly packed), data offset in buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //vertex attr. location

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shaderProgram);
}

//shader utils
unsigned int InitShader(GLenum shaderType, const char* shaderSource)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL); //shader object, number of strigns as source code, source code, ?
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error: Shader: Vertex: Compilation Failed:\n" << infoLog << std::endl;
	}

	return shader;
}

void Dispose()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}