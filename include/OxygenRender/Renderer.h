#pragma once
#include "OxygenRender/GraphicsTypes.h"
#include "OxygenRender/Window.h"
#include "OxygenRender/Buffer.h"
#include <glm/glm.hpp>
#include <memory>
namespace OxyRender
{

    enum class RenderCapability
    {
        DepthTest,
        Blend,
        CullFace,
        StencilTest,
        Multisample,
        ColorMask,
        ProgramPointSize
    };
    enum class RenderBlendFunc
    {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha
    };


    enum class RenderPolygonMode
    {
        Fill,
        Line,
        Point
    };
    enum class StencilFunc
    {
        Always,
        Equal,
        Notequal,
        Less,
        Lequal,
        Greater,
        Gequal,
        Never
    };

    enum class StencilOp
    {
        Keep,
        Zero,
        Replace,
        Incr,
        IncrWrap,
        Decr,
        DecrWrap,
        Invert
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
        virtual void drawPoints(const VertexArray &vao, size_t vertexCount) = 0;
        virtual void clear() = 0;
        virtual void setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor) = 0;

        // virtual void setStencilFunc(StencilFunc func, GLint ref, GLuint mask) = 0;
        // virtual void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass) = 0;

        // virtual void setStencilMask(GLuint mask) = 0;
        // virtual void clearStencil() = 0;
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
        virtual void drawPoints(const VertexArray &vao, size_t vertexCount) override;
        void setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor);

        void clear() override;

        // void setStencilFunc(StencilFunc func, GLint ref, GLuint mask) override;
        // void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass) override;

        // virtual void setStencilMask(GLuint mask) override;
        // virtual void clearStencil() override;

    private:
        GLenum convertStencilFunc(StencilFunc func);
        GLenum convertStencilOp(StencilOp op);
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
        void setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor);
        void drawTriangles(const VertexArray &vao, size_t indexCount);
        void drawLines(const VertexArray &vao, size_t indexCount, float thickness);
        void drawPoints(const VertexArray &vao, size_t vertexCount);
        void setClearColor(const OxyColor &color);
        void clear();

        // void setStencilFunc(StencilFunc func, GLint ref, GLuint mask);
        // void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass);
        // void setStencilMask(GLuint mask);
        // void clearStencil();
    };
} // namespace OxyRender