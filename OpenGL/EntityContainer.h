#pragma once

#include <iostream>
#include <vector>

#include "Mesh.h"
#include "Material.h"

class EntityContainer
{
    private:
        std::vector<Mesh*> m_meshes;
        std::vector<Material*> m_materials;

    public:
        ~EntityContainer()
        {
            for(int i = 0; i < m_meshes.size(); ++i) //assume containers are the same size (they SHOULD be)
            {
                delete m_meshes[i];
                m_meshes[i] = 0;

                delete m_materials[i];
                m_materials[i] = 0;
            }

            m_meshes.clear();
            m_materials.clear();
        }

        void addEntity()
        {
            m_meshes.push_back(nullptr);
            m_materials.push_back(nullptr);
        }

        void addEntity(Mesh* mesh, Material* mat)
        {
            m_meshes.push_back(mesh);
            m_materials.push_back(mat);
        }

        bool addMeshToEntity(uint32_t entityId, Mesh* mesh)
        {
            if(m_meshes[entityId] != nullptr)
            {
                std::cout << "Mesh already assigned to entity";
                return false;
            }

            if(m_meshes.size() <= entityId) //assume entityId is just the index (no reason not to be)
            {
                m_meshes.push_back(mesh);
                m_materials.push_back(nullptr); //keep the vectors the same size
                return true;
            }

            m_meshes[entityId] = mesh;
            return true;
        }

        bool addMaterialToEntity(uint32_t entityId, Material* mat)
        {
            if(m_materials[entityId] != nullptr)
            {
                std::cout << "Mesh already assigned to entity";
                return false;
            }

            if(m_materials.size() <= entityId) //assume entityId is just the index (no reason not to be)
            {
                m_materials.push_back(mat);
                m_meshes.push_back(nullptr); //keep the vectors the same size
                return true;
            }

            m_materials[entityId] = mat;
            return true;
        }

        const std::vector<Mesh*>& getMeshes() const
        {
            return m_meshes;
        }

        const std::vector<Material*>& getMaterials() const
        {
            return m_materials;
        }
};