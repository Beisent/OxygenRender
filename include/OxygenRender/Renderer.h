#pragma once
#include <memory>
namespace OxyRender
{

    enum class RendererBackend
    {
        OpenGL,
        Vulkan,
        DirectX
    };

    class IRenderer
    {
    private:
        RendererBackend backendType;

    public:
        IRenderer() = default;
        virtual ~IRenderer() = default;
        virtual void clear() = 0;
        RendererBackend getBackendType() const { return backendType; }
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
        static std::unique_ptr<IRenderer> createRenderer(RendererBackend type)
        {
            switch (type)
            {
            case RendererBackend::OpenGL:
                return std::make_unique<OpenGLRenderer>();
            default:
                return nullptr;
            }
        }
    };
} // namespace OxyRender