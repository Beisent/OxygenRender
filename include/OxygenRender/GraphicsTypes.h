    #pragma once
    #include <cstdint>

    namespace OxyRender
    {
        // 颜色结构体
        struct OxyColor
        {
            float r, g, b, a;
            OxyColor(float r, float g, float b, float a = 1.0f)
            {
                this->r = r;
                this->g = g;
                this->b = b;
                this->a = a;
            }
        };

        // 渲染器后端枚举
        enum class RendererBackend
        {
            OpenGL,
            Vulkan,
            DirectX
        };
        // 当前渲染器后端命名空间
        namespace Backends
        {
            inline RendererBackend OXYG_CurrentBackend = RendererBackend::OpenGL;
        }

        // 设置当前渲染器后端
        inline void SetRenderBackend(RendererBackend backend)
        {
            Backends::OXYG_CurrentBackend = backend;
        }
    

        using BufferHandle = uint32_t; // 对外句柄类型

    }
