
#include "RenderSystem.h"

RenderSystem::RenderSystem(GLFWwindow* window, EntityContainer* entityContainer)
{
    m_window = window;
	m_entityContainer = entityContainer;
}

void RenderSystem::dispose()
{
	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool RenderSystem::init()
{
    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);

    return true;
}

void RenderSystem::render(Camera* camera)
{
    glfwSwapBuffers(m_window); //RenderSystem render

	glClearColor(0.2f, 0.3f ,0.3f ,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int meshCount = 0;
	const std::vector<Mesh*>& meshes = m_entityContainer->getMeshes();
	const std::vector<Material*>& mats = m_entityContainer->getMaterials();
	for(int i = 0; i < meshes.size(); ++i)
	{
		const Texture* texture = mats[i]->GetTexture();
		const Shader* shader = mats[i]->GetShader();

		if(texture != nullptr)
			texture->bind();
		else
			glBindTexture(GL_TEXTURE_2D, 0);


		if(shader != nullptr)
		{
			shader->use();
			float timeValue = (float) glfwGetTime();
			shader->setFloat("time", timeValue);
			//have static objects which don't have their transforms change - EVER!

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.5f * meshCount));
			++meshCount;
			//model = glm::rotate(model, (float) timeValue * glm::radians(50.0f), glm::vec3(0.5, 1.0f, 0.0f));

			glm::mat4 proj = glm::perspective(glm::radians(camera->fov), 800.0f / 600.0f, 0.1f, 100.0f);

			glm::mat4 view = camera->getView();
			
			shader->setMatrix("model", glm::value_ptr(model));
			shader->setMatrix("view", glm::value_ptr(view));
			shader->setMatrix("projection", glm::value_ptr(proj));
		}

		meshes[i]->bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawElements(GL_TRIANGLES, meshes[i]->indexCount(), GL_UNSIGNED_INT, nullptr); //draw primitives using active shader, the defined VAO and the VBOS data (bound via VAO)
	}
}

void RenderSystem::setWireframeMode(bool useWireframe)
{
	long mode = GL_FILL;
	if(useWireframe)
	{
		mode = GL_LINE;
	}

	glPolygonMode(GL_FRONT_AND_BACK, mode);
}