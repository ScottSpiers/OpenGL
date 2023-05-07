
#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
    public:
        unsigned int ID;

        Shader(const char* vPath, const char* fPath);

        //do I want a Shader class? Prefer to have a shader manager and tell it what shader to use?
        //how would I store the shader? In materials attached to render components on entities?
        //Want needed shaders loaded before shaders are actually used
        void use();
        void dispose();

        void setBool(const char* name, bool value) const;
        void setInt(const char* name, int value) const;
        void setFloat(const char* name, float value) const;
        void setMatrix(const char* name, glm::f32* matrix) const;

    private:
        unsigned int initialise(GLenum shaderType, const char* shaderSource);
};