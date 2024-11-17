
#pragma once

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera
{
    public:
        float yaw = -90.0f;
        float pitch = 0.0f;
        float roll = 0.0f;

        const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float fov = 45.0f;
        
        void setPosition(double x, double y, double z) {position = glm::vec3(x,y,z);}
        void setPosition(glm::vec3 newPos) {position = newPos;}

        void setUp(double x,  double y, double z) {up = glm::vec3(x,y,z);}
        void setRight(double x, double y, double z) {right = glm::vec3(x,y,z);}
        void setForward(double x, double y, double z) {forward = glm::vec3(x,y,z);}
        void setForward(glm::vec3 f) {forward = f;}

        void setYaw(float y) {yaw = y;}
        void setPitch(float p) {pitch = p;}
        void setRoll(float r) {roll = r;}

        void resetRight() {right = glm::normalize(glm::cross(forward, worldUp));}
        void resetUp() {up = glm::normalize(glm::cross(right, forward));}

        glm::mat4 getView() const {return glm::lookAt(position, position + forward, up);}

        const glm::vec3 getUp() const {return up;}
        const  glm::vec3 getRight() const {return right;}
        const glm::vec3 getForward() const {return forward;}
        const glm::vec3 getPosition() const {return position;}

        const glm::vec3 calcDirection() const
        {
            glm::vec3 dir;
            dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            dir.y = sin(glm::radians(pitch));
            dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

            return glm::normalize(dir);
        }

        const void printUp() const { std::cout << "Up: " << glm::to_string(up) << std::endl;}
        const void printRight() const {std::cout << "Right: " << glm::to_string(right) << std::endl;}
        const void printPos() const {std::cout << "Position " << glm::to_string(position) << std::endl;}


    private:
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

        glm::vec3 up =      glm::vec3(0.0, 1.0, 0.0);
        glm::vec3 right =   glm::vec3(1.0, 0.0, 0.0);
        glm::vec3 forward = glm::vec3(0.0, 0.0, 1.0);
};