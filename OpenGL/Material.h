#pragma once

#include "Shader.h"
#include "Texture.h"

class Material
{
    //What if we want one material to use maps and one to use straight values?
    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float m_shininess;

        Shader* m_shader;
        Texture* m_diffMap; //TODO: Allow for multiple textures - is it possible to auto generate from shader assigned?
        Texture* m_specMap;

    public:
        Material(Shader* shader, Texture* diffMap, Texture* specMap): m_ambient( 1.0f, 0.5f, .31f), m_diffuse( 1.0f, 0.5f, .31f),
                                                    m_specular(0.5f, 0.5f, 0.5f), m_shininess(32.0f), m_shader(shader), m_diffMap(diffMap), m_specMap(specMap)
        {
            if(diffMap)
                m_diffMap->bind();

            if(specMap)
                m_specMap->bind(1);
        }

        ~Material() = default;

        const Shader* GetShader() const {return m_shader;}
        const Texture* GetDiffuseMap() const {return m_diffMap;}
        const Texture* GetSpecularMap() const {return m_specMap;}
        const glm::vec3& GetAmbient() const {return m_ambient;}
        const glm::vec3& GetDiffuse() const {return m_diffuse;}
        const glm::vec3& GetSpecular() const {return m_specular;}
        const float GetShininess() const {return m_shininess;}

        void SetShader(Shader* shader)
        {
            if(shader)
                m_shader = shader;
        }

        void SetDiffuseMap(Texture* texture)
        {
            if(texture)
                m_diffMap = texture;
        }

        void SetSpecularMap(Texture* texture)
        {
            if(texture)
                m_specMap = texture;
        }

};