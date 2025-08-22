    #pragma once
    #include <cstdint>

    namespace OxyRender
    {
        struct OxyColor
        {
            float r, g, b, a;
        };

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
        namespace Backends
        {
            inline WindowBackend OXYG_CurrentWindowBackend = WindowBackend::GLFW;
            inline RendererBackend OXYG_CurrentBackend = RendererBackend::OpenGL;
        }

        inline void SetRenderBackend(RendererBackend backend)
        {
            Backends::OXYG_CurrentBackend = backend;
        }
        inline void SetWindowBackend(WindowBackend backend)
        {
            Backends::OXYG_CurrentWindowBackend = backend;
        }

        using BufferHandle = uint32_t; // 对外句柄类型

    }
