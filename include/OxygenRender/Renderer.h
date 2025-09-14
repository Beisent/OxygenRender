#pragma once
#include "OxygenRender/GraphicsTypes.h"
#include "OxygenRender/Window.h"
#include "OxygenRender/Buffer.h"
#include <memory>

namespace OxyRender
{
    // ================= 渲染状态相关 =================
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

    // ================= 混合相关 =================
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

    // ================= 多边形模式 =================
    enum class RenderPolygonMode
    {
        Fill,
        Line,
        Point
    };

    // ================= 模板测试相关 =================
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

    // ================= 渲染器抽象接口类 =================
    class IRenderer
    {
    public:
        IRenderer() = default;
        virtual ~IRenderer() = default;

        // 渲染状态
        virtual void setCapability(RenderCapability cap, bool enable) = 0;
        virtual void setPolygonMode(RenderPolygonMode mod, bool enable) = 0;

        // 绘制
        virtual void drawTriangles(const VertexArray &vao, size_t indexCount) = 0;
        virtual void drawLines(const VertexArray &vao, size_t indexCount, float thickness) = 0;
        virtual void drawPoints(const VertexArray &vao, size_t vertexCount) = 0;

        // 清除
        virtual void clear() = 0;
        inline void setClearColor(const OxyColor &color) { m_clear_color = color; }

        // 混合
        virtual void setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor) = 0;

        // // 模板测试
        // virtual void setStencilFunc(StencilFunc func, GLint ref, GLuint mask) = 0;
        // virtual void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass) = 0;
        // virtual void setStencilMask(GLuint mask) = 0;
        // virtual void clearStencil() = 0;

    protected:
        OxyColor m_clear_color = {0.8f, 0.8f, 0.8f, 1.0f};
    };

    // ================= OpenGL渲染器实现类 =================
    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override = default;

        void setCapability(RenderCapability cap, bool enable) override;
        void setPolygonMode(RenderPolygonMode mod, bool enable) override;
        
        void drawTriangles(const VertexArray &vao, size_t indexCount) override;
        void drawLines(const VertexArray &vao, size_t indexCount, float thickness) override;
        void drawPoints(const VertexArray &vao, size_t vertexCount) override;
        void setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor) override;
        void clear() override;

        // void setStencilFunc(StencilFunc func, GLint ref, GLuint mask) override;
        // void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass) override;
        // void setStencilMask(GLuint mask) override;
        // void clearStencil() override;

    // private:
    //     GLenum convertStencilFunc(StencilFunc func);
    //     GLenum convertStencilOp(StencilOp op);
    };

    // ================= 渲染器工厂类 =================
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

    // ================= 渲染器封装类 =================
    class Renderer
    {
    private:
        std::unique_ptr<IRenderer> renderer;
        Window &m_Window;

    public:
        explicit Renderer(Window &window);

        // 渲染状态
        void setCapability(RenderCapability cap, bool enable);
        void setPolygonMode(RenderPolygonMode mod, bool enable);

        // 混合
        void setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor);

        // 绘制
        void drawTriangles(const VertexArray &vao, size_t indexCount);
        void drawLines(const VertexArray &vao, size_t indexCount, float thickness);
        void drawPoints(const VertexArray &vao, size_t vertexCount);

        // 清除
        void setClearColor(const OxyColor &color);
        void clear();

        // // 模板测试
        // void setStencilFunc(StencilFunc func, GLint ref, GLuint mask);
        // void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass);
        // void setStencilMask(GLuint mask);
        // void clearStencil();
    };
} // namespace OxyRender