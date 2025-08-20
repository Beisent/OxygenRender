// 来自LearnOpenGL教程
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace OxyRender
{
    enum Camera_Movement
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

    /**
     * @brief 相机类，用于处理3D场景中的摄像机视角
     *
     * 该类实现了第一人称摄像机的功能，包括位置、方向、移动、视角控制等
     */
    class Camera
    {
    public:
        /**
         * @brief 构造函数，使用向量参数初始化相机
         * @param position 相机初始位置，默认为(0,0,0)
         * @param up 世界上的方向，默认为(0,1,0)
         * @param yaw 偏航角，默认为CameraConstants::YAW
         * @param pitch 俯仰角，默认为CameraConstants::PITCH
         */
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CameraConstants::YAW, float pitch = CameraConstants::PITCH);

        /**
         * @brief 构造函数，使用浮点数参数初始化相机
         * @param posX 相机位置X坐标
         * @param posY 相机位置Y坐标
         * @param posZ 相机位置Z坐标
         * @param upX 上方向X分量
         * @param upY 上方向Y分量
         * @param upZ 上方向Z分量
         * @param yaw 偏航角
         * @param pitch 俯仰角
         */
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        /**
         * @brief 获取视图矩阵
         * @return glm::mat4 视图变换矩阵
         */
        glm::mat4 GetViewMatrix();

        /**
         * @brief 处理键盘输入控制相机移动
         * @param direction 移动方向枚举值
         * @param deltaTime 帧间隔时间，用于平滑移动
         */
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);

        /**
         * @brief 处理鼠标移动控制相机视角
         * @param xoffset 鼠标X轴偏移量
         * @param yoffset 鼠标Y轴偏移量
         * @param constrainPitch 是否限制俯仰角范围，默认为true
         */
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

        /**
         * @brief 处理鼠标滚轮控制视野缩放
         * @param yoffset 滚轮Y轴偏移量
         */
        void ProcessMouseScroll(float yoffset);
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
