#pragma once
#include "OxygenRender/GraphicsTypes.h"
#include "OxygenRender/Window.h"
#include "OxygenRender/Buffer.h"
#include <memory>
namespace OxyRender
{
    enum class RenderCapability
    {
        DepthTest,
        Blend,
        CullFace,
        StencilTest
    };
    class IRenderer
    {
    public:
        IRenderer() = default;
        virtual ~IRenderer() = default;
        virtual void setCapability(RenderCapability cap, bool enable) = 0;

        virtual void drawTriangles(const VertexArray &vao, size_t indexCount) = 0;
        virtual void drawLines(const VertexArray &vao, size_t indexCount, float thickness) = 0;
        virtual void clear() = 0;
    };

    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override = default;
        void setCapability(RenderCapability cap, bool enable) override;
        virtual void drawTriangles(const VertexArray &vao, size_t indexCount) override;
        virtual void drawLines(const VertexArray &vao, size_t indexCount, float thickness) override;
        void clear() override;
    };

    class RendererFactory
    {
    public:
        static std::unique_ptr<IRenderer> createRenderer()
        {
            switch (Backends::OXYG_CurrentBackend)
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
        Window &m_Window;

    public:
        explicit Renderer(Window &window);
        void setCapability(RenderCapability cap, bool enable);
        void drawTriangles(const VertexArray &vao, size_t indexCount);
        void drawLines(const VertexArray &vao, size_t indexCount, float thickness);
        void clear();
    };
} // namespace OxyRender