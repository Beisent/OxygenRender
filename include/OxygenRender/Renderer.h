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
    enum class RenderPolygonMode
    {
        Fill,
        Line,
        Point
    };
    class IRenderer
    {
    public:
        IRenderer() = default;
        virtual ~IRenderer() = default;
        virtual void setCapability(RenderCapability cap, bool enable) = 0;
        virtual void setPolygonMode(RenderPolygonMode mod, bool enable) = 0;
        virtual void drawTriangles(const VertexArray &vao, size_t indexCount) = 0;
        virtual void drawLines(const VertexArray &vao, size_t indexCount, float thickness) = 0;
       // virtual void drawArray(const VertexArray &vao, size_t vertexCount) = 0;
        virtual void clear() = 0;
        inline void setClearColor(const OxyColor &color) { m_clear_color = color; }

    protected:
        OxyColor m_clear_color = {0.8f, 0.8f, 0.8f, 1.0f};
    };

    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override = default;
        void setCapability(RenderCapability cap, bool enable) override;
        virtual void setPolygonMode(RenderPolygonMode mod, bool enable) override;
        virtual void drawTriangles(const VertexArray &vao, size_t indexCount) override;
        virtual void drawLines(const VertexArray &vao, size_t indexCount, float thickness) override;
        //virtual void drawArray(const VertexArray &vao, size_t vertexCount) override;
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
        void setPolygonMode(RenderPolygonMode mod, bool enable);
        void drawTriangles(const VertexArray &vao, size_t indexCount);
        void drawLines(const VertexArray &vao, size_t indexCount, float thickness);
        //void drawArray(const VertexArray &vao, size_t vertexCount);
        void clear();
    };
} // namespace OxyRender