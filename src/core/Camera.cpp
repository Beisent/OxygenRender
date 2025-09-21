#include "OxygenRender/Camera.h"
namespace OxyRender
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(CameraConstants::DEFAULT_SPEED), m_MouseSensitivity(CameraConstants::DEFAULT_SENSITIVITY), m_Zoom(CameraConstants::DEFAULT_ZOOM)
    {
        m_Position = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(CameraConstants::DEFAULT_SPEED), m_MouseSensitivity(CameraConstants::DEFAULT_SENSITIVITY), m_Zoom(CameraConstants::DEFAULT_ZOOM)
    {
        m_Position = glm::vec3(posX, posY, posZ);
        m_WorldUp = glm::vec3(upX, upY, upZ);
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    void Camera::processKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = m_MovementSpeed * deltaTime;
        if (direction == CameraMovement::Forward)
            m_Position += m_Front * velocity;
        if (direction == CameraMovement::Backward)
            m_Position -= m_Front * velocity;
        if (direction == CameraMovement::Left)
            m_Position -= m_Right * velocity;
        if (direction == CameraMovement::Right)
            m_Position += m_Right * velocity;
        if (direction == CameraMovement::Up)
            m_Position += m_Up * velocity;
        if (direction == CameraMovement::Down)
            m_Position -= m_Up * velocity;
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainm_Pitch)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (constrainm_Pitch)
        {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::processMouseScroll(float yoffset)
    {
        m_Zoom -= (float)yoffset;
        if (m_Zoom < 1.0f)
            m_Zoom = 1.0f;
        if (m_Zoom > 45.0f)
            m_Zoom = 45.0f;
    }
    void Camera::updateCameraVectors()
    {

        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }

    glm::mat4 Camera::getOrthoViewMatrix2D() const
    {

        return glm::lookAt(glm::vec3(m_Position.x, m_Position.y, 1), glm::vec3(m_Position.x, m_Position.y, 0), glm::vec3(0, 1, 0));
    }

    glm::mat4 Camera::getOrthoProjectionMatrix2D(int screenWidth, int screenHeight) const
    {
        float halfWidth = (screenWidth / 2.0f) * m_Zoom;
        float halfHeight = (screenHeight / 2.0f) * m_Zoom;

        return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f);
    }

    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }
    glm::mat4 Camera::getOrthoProjectionMatrix(int screenWidth, int screenHeight) const
    {
        return glm::ortho(0.0f, (float)screenWidth,
                          0.0f, (float)screenHeight,
                          -1.0f, 1.0f);
    }
    glm::mat4 Camera::getPerspectiveProjectionMatrix(int screenWidth, int screenHeight) const
    {
        return glm::perspective(glm::radians(m_Zoom),
                                (float)screenWidth / (float)screenHeight,
                                0.1f, 2000.0f);
    }

    glm::vec2 Camera::windowToWorld(float x, float y,float windowWidth,float windowHeight) const
    {
       
        int screenWidth = windowWidth;
        int screenHeight = windowHeight;


        float ndcX = (2.0f * x) / screenWidth - 1.0f;
        float ndcY = 1.0f - (2.0f * y) / screenHeight; 

        glm::vec4 clipCoords(ndcX, ndcY, -1.0f, 1.0f);


        glm::mat4 proj = getOrthoProjectionMatrix2D(screenWidth, screenHeight);
        glm::mat4 view = getOrthoViewMatrix2D();
        glm::mat4 invVP = glm::inverse(proj * view);

        glm::vec4 worldPos = invVP * clipCoords;

     
        if (worldPos.w != 0.0f)
            worldPos /= worldPos.w;

        return glm::vec2(worldPos.x, worldPos.y);
    }

}