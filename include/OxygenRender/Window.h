#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace OxyRender
{
    enum class WindowBackend
    {
        GLFW,
        SDL,
        WinAPI
    };
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
        virtual void swapBuffers() = 0;
    };
    class GLFWWindow : public IWindow
    {
    private:
        GLFWwindow *m_window;

    public:
        GLFWWindow(int width, int height, std::string title);
        ~GLFWWindow() override;
        void *getWindow() const;
        void setViewport(int x, int y, int width, int height) override;
        bool shouldClose() override;
        void swapBuffers() override;
    };
} // namespace OxyRender