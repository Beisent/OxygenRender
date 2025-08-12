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

        enum class BufferType
    {
        Vertex, // 顶点缓冲
        Index   // 索引缓冲
    };

    enum class BufferUsage
    {
        StaticDraw,
        DynamicDraw,
        StreamDraw
    };

    using BufferHandle = uint32_t; // 对外句柄类型

}
