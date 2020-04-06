

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>


class Camera
{
public:
    float yaw;
    float pitch;

    float MouseSensitivity  = 0.002f;
    float Zoom;


    glm::vec3 target;
    
    Camera(float yaw = 0, float pitch = 0)
    {
        yaw = pitch = 0;
    }

    void followTarget(glm::vec3 target)
    {
        this->target = target;
    }

    glm::mat4 getViewMatrix()
    {
        glm::vec4 offset(0, 0, 1, 1);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.f), yaw, glm::vec3(0, 1, 0));
        rotation = glm::rotate(rotation, pitch, glm::vec3(1, 0, 0));
        offset = rotation * offset;

        glm::vec3 position = target + glm::vec3(offset.x,offset.y,offset.z) * 64.f;

        return glm::lookAt(position, target, glm::vec3(0, 1, 0));
    }

    glm::vec3 getPosition()
    {
        glm::vec4 offset(0, 0, 1, 1);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.f), yaw, glm::vec3(0, 1, 0));
        rotation = glm::rotate(rotation, pitch, glm::vec3(1, 0, 0));
        offset = rotation * offset;

        glm::vec3 position = target + glm::vec3(offset.x,offset.y,offset.z) * 64.f;

        return position;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;
    
        if (pitch > 0.0f)   
        {
            pitch = 0.0f;
        }

        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }   
        
    }
};
#endif

