#pragma once

#include "Shader.h"
#include "Texture.h"

class Material
{
    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float m_shininess;

        Shader* m_shader;
        Texture* m_texture; //TODO: Allow for multiple textures - is it possible to auto generate from shader assigned?

    public:
        Material(Shader* shader, Texture* texture): m_ambient(0.5f, 0.5f, 0.5f), m_diffuse(1.0f, 1.0f, 1.0f),
                                                    m_specular(0.5f, 0.5f, 0.5f), m_shininess(32.0f), m_shader(shader), m_texture(texture)
        {
            if(m_texture)
                m_texture->bind();
        }

        ~Material() = default;

        const Shader* GetShader() const {return m_shader;}
        const Texture* GetTexture() const {return m_texture;}
        const glm::vec3& GetAmbient() const {return m_ambient;}
        const glm::vec3& GetDiffuse() const {return m_diffuse;}
        const glm::vec3& GetSpecular() const {return m_specular;}
        const float GetShininess() const {return m_shininess;}

        void SetShader(Shader* shader)
        {
            if(shader)
                m_shader = shader;
        }

        void SetTexture(Texture* texture)
        {
            if(texture)
                m_texture = texture;
        }

};