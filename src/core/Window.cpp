#include "OxygenRender/Window.h"
#include "OxygenRender/EventSystem.h"
#include <iostream>

namespace OxyRender
{
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Event e;
        KeyCode k = glfwToKeyCode(key);
        if (action == GLFW_PRESS)
        {
            e.type = EventType::KeyPressed;
            e.data = KeyEvent{k, scancode, mods};
        }
        else if (action == GLFW_RELEASE)
        {
            e.type = EventType::KeyReleased;
            e.data = KeyEvent{k, scancode, mods};
        }
        EventSystem::getInstance().pushEvent(e);
    }
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        Event e;
        e.type = EventType::MouseScrolled;
        e.data = MouseScrollEvent{(float)xoffset, (float)yoffset};
        EventSystem::getInstance().pushEvent(e);
    }

    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        Event e;
        if (action == GLFW_PRESS)
        {
            e.type = EventType::MouseButtonPressed;
            e.data = MouseButtonEvent{button, mods};
        }
        else if (action == GLFW_RELEASE)
        {
            e.type = EventType::MouseButtonReleased;
            e.data = MouseButtonEvent{button, mods};
        }
        EventSystem::getInstance().pushEvent(e);
    }

    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
    {
        Event e;
        e.type = EventType::MouseMoved;
        e.data = MouseMoveEvent{{(float)xpos, (float)ypos}};
        EventSystem::getInstance().pushEvent(e);
    }

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        Event e;
        e.type = EventType::WindowResized;
        e.data = WindowResizeEvent{width, height};
        EventSystem::getInstance().pushEvent(e);
    }
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
        glfwWindowHint(GLFW_SAMPLES, 8);

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

        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetCursorPosCallback(m_window, cursorPosCallback);
        glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
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

    void GLFWWindow::shutdown()
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
    void GLFWWindow::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }
    void GLFWWindow::pollEvents()
    {
        glfwPollEvents();
    }
    void GLFWWindow::setCursorPos(float x, float y)
    {
        glfwSetCursorPos(m_window, x, y);
    }
    void GLFWWindow::setCursorMode(CursorMode mode)
    {
        switch (mode)
        {
        case CursorMode::Normal:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CursorMode::Disabled:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case CursorMode::Hidden:
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
    }

    double GLFWWindow::getTime()
    {
        return glfwGetTime();
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
    void Window::shutdown()
    {
        m_window->shutdown();
    }
    void Window::swapBuffers()
    {
        m_window->swapBuffers();
    }
    void Window::pollEvents()
    {
        return m_window->pollEvents();
    }

    int Window::getWidth() const
    {
        return m_window->getWidth();
    }
    int Window::getHeight() const
    {
        return m_window->getHeight();
    }

    void Window::setCursorPos(float x, float y)
    {
        m_window->setCursorPos(x, y);
    }
    void Window::setCursorMode(CursorMode mode)
    {
        m_window->setCursorMode(mode);
    }

    double Window::getTime()
    {
        return m_window->getTime();
    }

    void Window::update()
    {
        pollEvents();
        swapBuffers();
    }

} // namespace OxyRender
