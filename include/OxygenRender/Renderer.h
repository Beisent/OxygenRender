#pragma once
#include "OxygenRender/GraphicsTypes.h"
#include <memory>
namespace OxyRender
{
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
        static std::unique_ptr<IRenderer> createRenderer()
        {
            switch (OXYG_CurrentBackend)
            {
            case RendererBackend::OpenGL:
                return std::make_unique<OpenGLRenderer>();
            default:
                return nullptr;
            }
        }
    };

    class Renderer
    {
    private:
        std::unique_ptr<IRenderer> renderer;

    public:
        Renderer();
        void clear();
    };
} // namespace OxyRender