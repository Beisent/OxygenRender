// 来自LearnOpenGL教程
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace OxyRender
{
    // 定义相机移动的可能选项。用作processKeyboard的参数
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    // 默认相机值
    namespace CameraConstants
    {
        const float YAW = -90.0f;
        const float PITCH = 0.0f;
        const float SPEED = 2.5f;
        const float SENSITIVITY = 0.1f;
        const float ZOOM = 45.0f;

    }
    // 相机类，处理视图和投影矩阵，并响应输入以移动相机位置
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CameraConstants::YAW, float pitch = CameraConstants::PITCH);

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        glm::mat4 get2DOrthoViewMatrix() const;
        glm::mat4 get2DOrthoProjectionMatrix(int screenWidth, int screenHeight) const;

        glm::mat4 getViewMatrix() const;
        glm::mat4 getOrthoProjectionMatrix(int screenWidth, int screenHeight) const;
        glm::mat4 getPerspectiveProjectionMatrix(int screenWidth, int screenHeight) const;

        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void processMouseScroll(float yoffset);
        inline glm::vec3 getPosition() const { return m_Position; }
        inline glm::vec3 getFront() const { return m_Front; }
        inline float getZoom() const { return m_Zoom; }
        inline void setPosition(glm::vec3 position) { m_Position = position; }
        inline void setYaw(float yaw) { m_Yaw = yaw; }
        inline void setPitch(float pitch) { m_Pitch = pitch; }
        inline void setMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }
        inline void setMouseSensitivity(float mouseSensitivity) { m_MouseSensitivity = mouseSensitivity; }
        inline void setZoom(float zoom) { m_Zoom = zoom; }

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

        float m_Yaw;
        float m_Pitch;

        float m_MovementSpeed;
        float m_MouseSensitivity;
        float m_Zoom;

        void updateCameraVectors();
    };
}
