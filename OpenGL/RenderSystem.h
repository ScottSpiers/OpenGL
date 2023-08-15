#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "EntityContainer.h"
#include "Shader.h"

class RenderSystem
{
    public:
    RenderSystem(GLFWwindow*, EntityContainer*);
    bool init();
    void render(Camera*);
    void setWireframeMode(bool);
    void dispose();

    private:
    GLFWwindow* m_window;
    EntityContainer* m_entityContainer;

};