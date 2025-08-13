#pragma once
#include <cstdint>

namespace OxyRender
{
    enum class RendererBackend
    {
        OpenGL,
        Vulkan,
        DirectX
    };

    enum class WindowBackend
    {
        GLFW,
        SDL,
        WinAPI
    };

    inline WindowBackend OXYG_CurrentWindowBackend = WindowBackend::GLFW;
    inline RendererBackend OXYG_CurrentBackend = RendererBackend::OpenGL;
    inline void SetRenderBackend(RendererBackend backend)
    {
        OXYG_CurrentBackend = backend;
    }
    inline void SetWindowBackend(WindowBackend backend)
    {
        OXYG_CurrentWindowBackend = backend;
    }
    
    using BufferHandle = uint32_t; // 对外句柄类型

}
