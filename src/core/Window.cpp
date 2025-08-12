#include "OxygenRender/Window.h"
#include <iostream>

namespace OxyRender
{
    IWindow::IWindow(int width, int height, std::string title) : m_width(width), m_height(height), m_title(std::move(title))
    {
    }
    GLFWWindow::GLFWWindow(int width, int height, std::string title) : IWindow(width, height, title)
    {
        if (!glfwInit())
        {
            std::cerr << "GLFW 初始化失败\n";
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            std::cerr << "窗口创建失败\n";
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(m_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "GLAD 初始化失败\n";
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void GLFWWindow::setViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
    bool GLFWWindow::shouldClose()
    {
        return glfwWindowShouldClose(m_window);
    }

    void GLFWWindow::swapBuffers()
    {
        glfwSwapBuffers(m_window);
        glfwPollEvents(); // 暂时放这里等待后续实现事件处理再改
    }

    Window::Window(int width, int height, std::string title)
    {
        m_window = WindowFactory::createWindow(width, height, std::move(title));
        if (!m_window)
        {
            std::cerr << "窗口创建失败\n";
            throw std::runtime_error("Failed to create window");
        }
    }
    void Window::setViewport(int x, int y, int width, int height)
    {
        m_window->setViewport(x, y, width, height);
    }
    bool Window::shouldClose()
    {
        return m_window->shouldClose();
    }
    void Window::swapBuffers()
    {
        m_window->swapBuffers();
    }

} // namespace OxyRender
