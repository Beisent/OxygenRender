#pragma once
#include <memory>
namespace OxyRender
{

    enum class RendererType
    {
        OpenGL,
        Vulkan,
        DirectX
    };

    class IRenderer
    {
    public:
        IRenderer() = default;
        virtual ~IRenderer() = default;
        virtual void clear() = 0;
    };

    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer() = default;
        ~OpenGLRenderer() override = default;
        void clear() override;
    };

    class RendererFactory
    {
    public:
        static std::unique_ptr<IRenderer> createRenderer(RendererType type)
        {
            switch (type)
            {
            case RendererType::OpenGL:
                return std::make_unique<OpenGLRenderer>();
            default:
                return nullptr;
            }
        }
    };
} // namespace OxyRender