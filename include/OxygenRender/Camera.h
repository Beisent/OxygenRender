#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OxyRender
{
    // 相机移动方向
    enum class CameraMovement
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    // 默认相机参数
    namespace CameraConstants
    {
        constexpr float DEFAULT_YAW = -90.0f;
        constexpr float DEFAULT_PITCH = 0.0f;
        constexpr float DEFAULT_SPEED = 2.5f;
        constexpr float DEFAULT_SENSITIVITY = 0.1f;
        constexpr float DEFAULT_ZOOM = 45.0f;
    }

    // 相机类
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = CameraConstants::DEFAULT_YAW,
               float pitch = CameraConstants::DEFAULT_PITCH);

        Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch);

        glm::mat4 getOrthoViewMatrix2D() const;
        glm::mat4 getOrthoProjectionMatrix2D(int screenWidth, int screenHeight) const;

        glm::mat4 getViewMatrix() const;
        glm::mat4 getOrthoProjectionMatrix(int screenWidth, int screenHeight) const;
        glm::mat4 getPerspectiveProjectionMatrix(int screenWidth, int screenHeight) const;

        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
        void processMouseScroll(float yOffset);

        glm::vec2 windowToWorld(float x, float y,float windowWidth,float windowHeight) const;

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
