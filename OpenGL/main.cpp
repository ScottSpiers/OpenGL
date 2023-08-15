
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "stb_image.cpp"

#include "Camera.h"
#include "EntityContainer.h"
#include "Mesh.h"
#include "RenderSystem.h"
#include "Shader.h"

void InitSystems();
void ProcessInput(GLFWwindow*, float);
void Render();
void InitCamera();
void Dispose();
void addEntities();

//callbacks
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);

Camera* m_camera;
EntityContainer* m_entityContainer;
RenderSystem* m_renderSystem;

const std::string shaderPath = "C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/shaders/";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400;
float lastY = 300; //see if I can jsut pass these through rather than tracking constantly - functional style!

bool firstMouse = true;

bool shouldExit = false;

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

	m_entityContainer = new EntityContainer();
	m_renderSystem = new RenderSystem(window, m_entityContainer);
	bool success = m_renderSystem->init();

	if(!success)
	{
		std::cout << "Failed to initialize RenderSystem" << std::endl;
		glfwTerminate();
		return -1;
	}

	//InputSystem
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);	

	//InputSystem
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glCreateShader can't be called before creating the context and gladLoadGLLoader!
	//Needs to be more generic - have to handle different shaders!
	addEntities();

	InitCamera();

	while(!glfwWindowShouldClose(window))
	{
		float curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		ProcessInput(window, deltaTime); //Input System - Process
		glfwPollEvents();

		m_renderSystem->render(m_camera);
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
		camPos += m_camera->getForward() * moveSpeed;
	
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camPos -= m_camera->getForward() * moveSpeed;
	
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camPos -= m_camera->getRight() * moveSpeed;

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camPos += m_camera->getRight() * moveSpeed;

	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camPos += m_camera->getUp() * moveSpeed;

	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camPos -= m_camera->getUp() * moveSpeed;

	m_camera->setPosition(camPos);

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
	m_camera->resetRight();
	m_camera->resetUp();
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	m_camera->fov = glm::clamp(m_camera->fov - (float) yOffset, 1.0f, 100.0f);
}

void addEntities()
{
	Mesh* cubeMesh1 = new Mesh();
	cubeMesh1->initBuffers();

	Mesh* cubeMesh2 = new Mesh();
	cubeMesh2->initBuffers();

	Mesh* light = new Mesh();
	light->initBuffers();

	Shader* shader = new Shader( (shaderPath + "vertex.vs").c_str(), (shaderPath + "fragment.fs").c_str());
	Texture* texture = new Texture("C:/Users/Scott/Documents/Random/Learning/OpenGL/OpenGL/OpenGL/data/container.jpg");
	Material* defaultMat = new Material(shader, texture);

	shader->use();
	shader->setVec3("objColour",1.0f, 0.5f, .33f);
	shader->setVec3("lightColour", 1.0f, 1.0f, 1.0f);

	Shader* lightShader = new Shader((shaderPath + "light.vs").c_str(), (shaderPath + "light.fs").c_str());
	Material* lightMaterial = new Material(lightShader, nullptr);

	m_entityContainer->addEntity(cubeMesh1, defaultMat);
	m_entityContainer->addEntity(cubeMesh2, defaultMat);
	m_entityContainer->addEntity(light, lightMaterial);
}

void InitCamera()
{
	m_camera = new Camera();
	m_camera->setPosition(0.0, 0.0, 3.0);
	m_camera->setForward(0.0, 0.0, -1.0);
}

void Dispose()
{
	m_renderSystem->dispose();
	
	delete m_camera;
	m_camera = 0;

	delete m_renderSystem;
	m_renderSystem = 0;
}