

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "stb_image.cpp"

#include "Camera.h"
#include "Shader.h"

void InitSystems();
void ProcessInput(GLFWwindow*, float);
void Wireframe(bool);
void Render();
void InitShaders();
void LoadTexture2D(unsigned int, char*);
void InitCamera();
void UpdateCamera();
void Dispose();

//callbacks
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);

Camera* m_camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400;
float lastY = 300; //see if I can jsut pass these through rather than tracking constantly - functional style!

bool firstMouse = true;

bool shouldExit = false;

unsigned int VAO;

unsigned int VBO;

float triVertices[] = {	-.5f, -.5f, 0.0f,
					 	0.5f, -.5f, 0.0f, 
						0.0f, 0.5f, 0.0f
					};

float triTexCoords[] = { 0.0f, 0.0f,
						 1.0f, 0.0f,
						 0.5f, 1.0f
					};

//Vertex struct - that struct is this array - attrib pointers use that - otherwise how do we mix types (we don't)
float sqrVertices[] = {	0.5f, 0.5f, 0.0f, 	1.0f, 0.0f, 1.0f,	1.0f, 1.0f, //top right
						0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f, //bottom right
						-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f, //bottom left
						-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f 	//top left
					};

float cubeVertices[] = { 
							-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     						0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     						0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     						0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    						-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     						0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     						0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     						0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    						-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    						-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    						-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    						-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

							0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
							0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
							0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
							0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
							0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
							0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

							-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
							0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
							0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
							0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
							-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
							-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

							-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
							0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
							0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
							0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
							-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
							-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

					};

unsigned int indices[] = {0, 1, 3,
							1, 2, 3
						};

unsigned int EBO;

Shader* shader;

unsigned int texture;

int main(int args, char** argv)
{
	
	InitSystems();

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glCreateShader can't be called before creating the context and gladLoadGLLoader!
	InitShaders();

	LoadTexture2D(texture, "C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/data/container.jpg");

	InitCamera();

	//Wireframe(true);
	while(!glfwWindowShouldClose(window))
	{
		float curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		ProcessInput(window, deltaTime);
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

void ProcessInput(GLFWwindow* window, float deltaTime)
{
	glm::vec3 camPos = m_camera->getPosition();
	const float moveSpeed = 2.5f * deltaTime;

	//gather input into actions
	//how is this done properly?
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camPos.z -= moveSpeed;
	
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camPos.z += moveSpeed;
	
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camPos -= glm::normalize(glm::cross(-m_camera->getDirection(), m_camera->getUp())) * moveSpeed;

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camPos += glm::normalize(glm::cross(-m_camera->getDirection(), m_camera->getUp())) * moveSpeed;

	m_camera->setPosition(camPos);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float timeValue = (float) glfwGetTime();
	shader->setFloat("time", timeValue); //will need to use specific shaderProgram first as it sets on cur active
	//if we have multple programs / VAOs we should bind here before drawing the objects that use them
	//if we only need one can we just set them up once and use them for everything without having to repeat calls
	//glUseProgram(shaderProgram);
	//glBindVertexArray(VAO);

	//have static cobjects which don't have their transforms change - EVER!

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, (float) timeValue * glm::radians(50.0f), glm::vec3(0.5, 1.0f, 0.0f));

	glm::mat4 proj = glm::perspective(glm::radians(m_camera->fov), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 view = m_camera->getView();
	
	shader->setMatrix("model", glm::value_ptr(model));
	shader->setMatrix("view", glm::value_ptr(view));
	shader->setMatrix("projection", glm::value_ptr(proj));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw primitives using active shader, the defined VAO and the VBOS data (bound via VAO)
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//this should probably just affect some data (MouseInput) that gets used in udpate functions (e.g. UpdateCamera)
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if(firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
		return;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos; //bottom to top

	lastX = xPos;
	lastY = yPos;

	const float sensitivity = 0.1f;

	m_camera->yaw += xOffset * sensitivity;
	m_camera->pitch += yOffset * sensitivity;

	m_camera->pitch = glm::clamp(m_camera->pitch, -89.0f, 89.0f);

	m_camera->setForward(m_camera->calcDirection());
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	m_camera->fov = glm::clamp(m_camera->fov - (float) yOffset, 1.0f, 100.0f);
}

void InitShaders()
{
	shader = new Shader("C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/vertex.vs", "C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/fragment.fs");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Vertex attr to config (matches location in vertex shader), size of attr, data type, normalise? (irrelevant for floats),
	//stride (0 = OpenGL determined - iff tighly packed), data offset in buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //vertex attr. location

	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	// glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); //could we debug by enabling / disabling these individually - could I create a switch for that to make it easier?

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->use();
}

void LoadTexture2D(unsigned int texture, char* path)
{
	glGenTextures(1, &texture); //how many textures - unsigned int array to store them
	glBindTexture(GL_TEXTURE_2D, texture);

	//set wrap & mipmap behaviour 
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if(!data)
	{
		std::cout << "Failed to load texture: " << path << std::endl;
		stbi_image_free(data);
		return;
	}

	// Texture target - mipmap level - texture format - width - height - legacy 0 - src format - src datatype - img data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void InitCamera()
{
	m_camera = new Camera();
	m_camera->setPosition(0.0, 0.0, 3.0);
	m_camera->setTarget(0.0, 0.0, 0.0);
	m_camera->setForward(0.0, 0.0, -1.0);
}

void UpdateCamera()
{

}

void Dispose()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	delete m_camera;
	m_camera = 0;

	//gldispose texture?
	texture = 0;
	shader->dispose();
	shader = nullptr;
}