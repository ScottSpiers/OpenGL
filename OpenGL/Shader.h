
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
        const void use() const;
        void dispose();

        void setBool(const char* name, bool value) const;
        void setInt(const char* name, int value) const;
        void setFloat(const char* name, float value) const;
        void setVec3(const char* name, float x, float y, float z) const;
        void setVec3(const char* name, glm::vec3 value) const;
        void setMatrix3(const char* name, glm::f32* matirx) const;
        void setMatrix4(const char* name, glm::f32* matrix) const;

    private:
        unsigned int initialise(GLenum shaderType, const char* shaderSource);
};