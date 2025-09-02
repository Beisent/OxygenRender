#pragma once
#include <string>
#include "OxygenRender/GraphicsTypes.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
namespace OxyRender
{
    // 鼠标光标模式
    enum class CursorMode
    {
        Normal,
        Disabled,
        Hidden
    };

    // Window 抽象接口类
    class IWindow
    {
    protected:
        int m_width;
        int m_height;
        std::string m_title;

    public:
        IWindow() = default;
        IWindow(int width, int height, std::string title);
        virtual ~IWindow() = default;
        virtual void setViewport(int x, int y, int width, int height) = 0;
        virtual bool shouldClose() = 0;
        virtual void shutdown() = 0;
        virtual void swapBuffers() = 0;
        virtual void pollEvents() = 0;
        inline int getWidth() const { return m_width; }

        inline int getHeight() const { return m_height; }

        // Cursor
        virtual void setCursorPos(float x, float y) = 0;
        virtual void setCursorMode(CursorMode mode) = 0;

        virtual double getTime() = 0;
    };
    // GLFW实现Window
    class GLFWWindow : public IWindow
    {
    private:
        GLFWwindow *m_window;

    public:
        GLFWWindow(int width, int height, std::string title);
        ~GLFWWindow() override;
        void setViewport(int x, int y, int width, int height) override;
        bool shouldClose() override;
        void shutdown() override;
        void swapBuffers() override;
        void pollEvents() override;

        void setCursorPos(float x, float y) override;
        void setCursorMode(CursorMode mode) override;

        double getTime() override;
    };
    // Window工厂类
    class WindowFactory
    {
    public:
        static std::unique_ptr<IWindow> createWindow(int width, int height, const std::string &title)
        {
            return std::make_unique<GLFWWindow>(width, height, title);
        }
    };
    // Window类对外接口
    class Window
    {
    private:
        std::unique_ptr<IWindow> m_window;

    public:
        Window(int width, int height, std::string title);
        void setViewport(int x, int y, int width, int height);
        bool shouldClose();
        void shutdown();
        void swapBuffers();
        void pollEvents();
        int getWidth() const;
        int getHeight() const;
        void update() ;

        void setCursorPos(float x, float y);
        void setCursorMode(CursorMode mode);

        double getTime();
    };
} // namespace OxyRender