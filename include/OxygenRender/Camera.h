// 来自LearnOpenGL教程
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace OxyRender
{
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    namespace CameraConstants
    {
        const float YAW = -90.0f;
        const float PITCH = 0.0f;
        const float SPEED = 2.5f;
        const float SENSITIVITY = 0.1f;
        const float ZOOM = 45.0f;

    }

    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CameraConstants::YAW, float pitch = CameraConstants::PITCH);

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        glm::mat4 getViewMatrix();
        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void processMouseScroll(float yoffset);

        glm::mat4 getOrthoProjectionMatrix(int screenWidth, int screenHeight);
        glm::mat4 getPerspectiveProjectionMatrix(int screenWidth, int screenHeight);

        glm::mat4 get2DOrthoViewMatrix() const;
        glm::mat4 get2DOrthoProjectionMatrix(int screenWidth, int screenHeight) const;

        glm::vec3 getPosition() const { return m_Position; }
        glm::vec3 getFront() const { return m_Front; }
        glm::vec3 getUp() const { return m_Up; }
        glm::vec3 getRight() const { return m_Right; }
        glm::vec3 getWorldUp() const { return m_WorldUp; }
        float getYaw() const { return m_Yaw; }
        float getPitch() const { return m_Pitch; }
        float getMovementSpeed() const { return m_MovementSpeed; }
        float getMouseSensitivity() const { return m_MouseSensitivity; }
        float getZoom() const { return m_Zoom; }

        void setPosition(glm::vec3 position) { m_Position = position; }
        void setFront(glm::vec3 front) { m_Front = front; }
        void setUp(glm::vec3 up) { m_Up = up; }
        void setRight(glm::vec3 right) { m_Right = right; }
        void setWorldUp(glm::vec3 worldUp) { m_WorldUp = worldUp; }
        void setYaw(float yaw) { m_Yaw = yaw; }
        void setPitch(float pitch) { m_Pitch = pitch; }
        void setMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }
        void setMouseSensitivity(float mouseSensitivity) { m_MouseSensitivity = mouseSensitivity; }
        void setZoom(float zoom) { m_Zoom = zoom; }

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
        /**
         * @brief 更新相机的方向向量
         * 根据偏航角和俯仰角重新计算Front、Right、Up向量
         */
        void updateCameraVectors();
    };
}
