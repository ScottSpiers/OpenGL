#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif


class Texture
{
    private:
        unsigned int m_id;
        std::string m_filePath;
        unsigned char* m_data;
        int m_width;
        int m_height;
        int m_nrChannels;

    public:
        Texture(const std::string&& filePath): m_id(0), m_filePath(filePath), m_data(nullptr), m_width(0), m_height(0), m_nrChannels(0)
        {
            glGenTextures(1, &m_id); //how many textures - unsigned int array to store them
            glBindTexture(GL_TEXTURE_2D, m_id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //set wrap & mipmap behaviour
            m_data = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_nrChannels, 0);

            if(!m_data)
            {
                std::cout << "Failed to load texture: " << m_filePath << std::endl;
                return;
            }

            auto format = GetTextureFormat(m_nrChannels);

            // Texture target - mipmap level - texture format - width - height - legacy 0 - src format - src datatype - img data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(m_data);
        }

        static GLenum GetTextureFormat(int channels)
        {
            if(channels == 1)
               return GL_RED;
            else if(channels == 3)
                return GL_RGB;
            else if(channels == 4)
                return GL_RGBA;
            else
                return GL_RGBA8;
        }


        ~Texture()
        {
            glDeleteTextures(1, &m_id);
        }

        void bind(const unsigned int slot = 0) const
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_id);
        }

        void unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

};