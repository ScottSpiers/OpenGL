

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

void InitSystems();
void ProcessInput(GLFWwindow*);
void Wireframe(bool);
void Render();
void framebuffer_size_callback(GLFWwindow*, int, int);
void InitShaders();
void Dispose();

bool shouldExit = false;

unsigned int VAO;

unsigned int VBO;

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

Shader* shader;

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

//called only on entities with render components - these will have materials pointing to shaders
//material.shader.ID is the program we use?
void Render()
{
	glClearColor(0.2f, 0.3f ,0.3f ,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	float timeValue = glfwGetTime();
	shader->setFloat("time", timeValue); //will need to use specific shaderProgram first as it sets on cur active
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
	shader = new Shader("C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/vertex.vs", "C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/fragment.fs");
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

	shader->use();
}

void Dispose()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shader->dispose();
	shader = nullptr;
}