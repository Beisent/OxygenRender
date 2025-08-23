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
        namespace Backends
        {
            inline RendererBackend OXYG_CurrentBackend = RendererBackend::OpenGL;
        }

        inline void SetRenderBackend(RendererBackend backend)
        {
            Backends::OXYG_CurrentBackend = backend;
        }
    

        using BufferHandle = uint32_t; // 对外句柄类型

    }
