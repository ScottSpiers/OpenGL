
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
		auto mat = mats[i];
		
		const Texture* diffMap = mat->GetDiffuseMap();
		const Texture* specMap = mat->GetSpecularMap();
		const Shader* shader = mat->GetShader();

		glBindTexture(GL_TEXTURE_2D, 0);
		
		if(diffMap != nullptr)
			diffMap->bind();

		if(specMap != nullptr)
			specMap->bind(1);

		if(shader != nullptr)
		{
			shader->use();
			float timeValue = (float) glfwGetTime();
			shader->setFloat("time", timeValue);
			//have static objects which don't have their transforms change - EVER!

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.5f * meshCount, -1.25f * meshCount, 2.5f * meshCount));
			++meshCount;
			//model = glm::rotate(model, (float) timeValue * glm::radians(50.0f), glm::vec3(0.5, 1.0f, 0.0f));

			glm::mat4 proj = glm::perspective(glm::radians(camera->fov), 800.0f / 600.0f, 0.1f, 100.0f);

			glm::mat4 view = camera->getView();
			
			glm::mat3 normalMat = glm::mat3(transpose(inverse(model)));
			shader->setMatrix3("normalMat", glm::value_ptr(normalMat));
			shader->setMatrix4("model", glm::value_ptr(model));
			shader->setMatrix4("view", glm::value_ptr(view));
			shader->setMatrix4("projection", glm::value_ptr(proj));

			auto camPos = camera->getPosition();
			shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);

			shader->setVec3("mat.ambient", mat->GetAmbient());
			// shader->setVec3("mat.diffuse", mat->GetDiffuse());
			// shader->setVec3("mat.specular", mat->GetSpecular());
			shader->setInt("mat.diffuse", 0);
			shader->setInt("mat.specular", 1);
			shader->setFloat("mat.shininess", mat->GetShininess());
			
			shader->setVec3("light.position", 0, 0, 0);
			shader->setVec3("light.ambient", .2f, .2f, .2f);
			shader->setVec3("light.diffuse", .5f, .5f, .5f);
			shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
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