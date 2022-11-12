#ifndef CAMERA_H
#define CAMERA_H

#include <../include/glad/glad.h>
#include <../include/glfw/glfw3.h>
#include <../include/glm/glm.hpp>
#include <../include/glm/gtc/matrix_transform.hpp>
#include <../include/glm/gtc/type_ptr.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera{
    public:
        glm::vec3 camPosition;
        glm::vec3 camFront;
        glm::vec3 camUp;
        glm::vec3 camRight;
        glm::vec3 worldUp;

        float camYaw; 
        float camPitch;

        float camMovementSpeed;
        float camMouseSensitivity;
        float camZoom;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : 
        camFront(glm::vec3(0.0f, 0.0f, -1.0f)), camMovementSpeed(SPEED), camMouseSensitivity(SENSITIVITY), camZoom(ZOOM){
            camPosition = position;
            worldUp = up;
            camYaw = yaw;
            camPitch = pitch;
            updateCameraVariables();
        }

        glm::mat4 GetVieWMatrix(){
            return glm::lookAt(camPosition, camPosition + camFront, camUp);
        }

        void ProcessKeyboard(Camera_Movement direction, float deltaTime){
            float velocity = camMovementSpeed * deltaTime;
            if(direction == UP)
                camPosition += worldUp * velocity;
            if(direction == DOWN)
                camPosition -= worldUp * velocity;
            if(direction == FORWARD)
                camPosition += camFront * velocity;
            if(direction == BACKWARD)
                camPosition -= camFront * velocity;
            if(direction == LEFT)
                camPosition -= camRight * velocity;
            if(direction == RIGHT)
                camPosition += camRight * velocity;
        }

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true){
            xoffset *= camMouseSensitivity;
            yoffset *= camMouseSensitivity;
    
            camYaw   += xoffset;
            camPitch += yoffset;

            if(constrainPitch){
                if(camPitch > 89.0f)
                    camPitch = 89.0f;
                if(camPitch < -89.0f)
                    camPitch = -89.0f;
            }
            updateCameraVariables();
        }
    
        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset){
            camZoom -= (float)yoffset;
            if(camZoom < 1.0f)
                camZoom = 1.0f;
            if(camZoom > 45.0f)
                camZoom = 45.0f; 
        }
    
    private:

        void updateCameraVariables(){
            glm::vec3 front;
            front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
            front.y = sin(glm::radians(camPitch));
            front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
            camFront = glm::normalize(front);
            camRight = glm::normalize(glm::cross(camFront, worldUp));
            camUp    = glm::normalize(glm::cross(camRight, camFront));
        }
};

#endif
