#include "Camera.h"

Camera::Camera(glm::vec3 position,
    glm::vec3 up,
    float yaw, float pitch) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    Fov(FOV), zNear(ZNEAR), zFar(ZFAR), AspectRatio(ASPECTRATIO),
    MovementSpeed(SPEED), Sensitivity(SENSITIVITY)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, 
    float upX, float upY, float upZ, 
    float yaw, float pitch) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    Fov(FOV), zNear(ZNEAR), zFar(ZFAR), AspectRatio(ASPECTRATIO),
    MovementSpeed(SPEED), Sensitivity(SENSITIVITY)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(Fov), AspectRatio, zNear, zFar);
}

void Camera::Move(int32_t dirs, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    glm::vec3 direction = glm::vec3(0.f, 0.f, 0.f);
    direction.z = (dirs & CAM_FORWARD) / CAM_FORWARD - (dirs & CAM_BACKWARD) / CAM_BACKWARD;
    direction.x = (dirs & CAM_RIGHT) / CAM_RIGHT - (dirs & CAM_LEFT) / CAM_LEFT;
    direction.y = (dirs & CAM_UP) / CAM_UP - (dirs & CAM_DOWN) / CAM_DOWN;
    
    //sadnessforever, thx for bugfix
    if (direction != glm::vec3(0,0,0))
        direction = glm::normalize(direction);
        
    Position += Front * velocity * direction.z;
    Position += Right * velocity * direction.x;
    Position += Up    * velocity * direction.y;
    UpdateCameraVectors();
}

void Camera::Rotate(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ChangeFOV(double value)
{
    Fov -= (float)value;
    if (Fov < 1.0f)
        Fov = 1.0f;
    if (Fov > 120.0f)
        Fov = 120.0f;
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}