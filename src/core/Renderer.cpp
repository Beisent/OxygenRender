#include "OxygenRender/Renderer.h"
#include <glad/glad.h>
namespace OxyRender
{
    GLenum toGLBlendFunc(RenderBlendFunc func)
    {
        switch (func)
        {
        case RenderBlendFunc::Zero:
            return GL_ZERO;
        case RenderBlendFunc::One:
            return GL_ONE;
        case RenderBlendFunc::SrcColor:
            return GL_SRC_COLOR;
        case RenderBlendFunc::OneMinusSrcColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case RenderBlendFunc::DstColor:
            return GL_DST_COLOR;
        case RenderBlendFunc::OneMinusDstColor:
            return GL_ONE_MINUS_DST_COLOR;
        case RenderBlendFunc::SrcAlpha:
            return GL_SRC_ALPHA;
        case RenderBlendFunc::OneMinusSrcAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case RenderBlendFunc::DstAlpha:
            return GL_DST_ALPHA;
        case RenderBlendFunc::OneMinusDstAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case RenderBlendFunc::ConstantColor:
            return GL_CONSTANT_COLOR;
        case RenderBlendFunc::OneMinusConstantColor:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case RenderBlendFunc::ConstantAlpha:
            return GL_CONSTANT_ALPHA;
        case RenderBlendFunc::OneMinusConstantAlpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        }
        return GL_ONE;
    }
    OpenGLRenderer::OpenGLRenderer()
    {
    }
    void OpenGLRenderer::clear()
    {
        glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void OpenGLRenderer::drawTriangles(const VertexArray &vao, size_t indexCount)
    {
        vao.bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)indexCount, GL_UNSIGNED_INT, 0);
        vao.unbind();
    }

    void OpenGLRenderer::drawLines(const VertexArray &vao, size_t indexCount, float thickness)
    {
        vao.bind();
        glLineWidth(thickness);
        glDrawElements(GL_LINES, (GLsizei)indexCount, GL_UNSIGNED_INT, 0);
        vao.unbind();
    }
    void OpenGLRenderer::drawPoints(const VertexArray &vao, size_t vertexCount)
    {
        vao.bind();
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertexCount));
        vao.unbind();
    }
    void OpenGLRenderer::setCapability(RenderCapability cap, bool enable)
    {
        GLenum glCap = 0;
        switch (cap)
        {
        case RenderCapability::DepthTest:
            glCap = GL_DEPTH_TEST;
            break;
        case RenderCapability::Blend:
            glCap = GL_BLEND;
            break;
        case RenderCapability::CullFace:
            glCap = GL_CULL_FACE;
            break;
        case RenderCapability::StencilTest:
            glCap = GL_STENCIL_TEST;
            break;
        case RenderCapability::Multisample:
            glCap = GL_MULTISAMPLE;
            break;
        case RenderCapability::ProgramPointSize:
            glCap = GL_PROGRAM_POINT_SIZE;
            break;
        }

        if (enable)
            glEnable(glCap);
        else
            glDisable(glCap);
    }

    void OpenGLRenderer::setPolygonMode(RenderPolygonMode mod, bool enable)
    {
        switch (mod)
        {
        case RenderPolygonMode::Fill:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case RenderPolygonMode::Line:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case RenderPolygonMode::Point:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
    }
    void OpenGLRenderer::setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor)
    {
        glBlendFunc(toGLBlendFunc(sfactor), toGLBlendFunc(dfactor));
    }

    // GLenum OpenGLRenderer::convertStencilFunc(StencilFunc func)
    // {
    //     switch (func)
    //     {
    //     case StencilFunc::Always:
    //         return GL_ALWAYS;
    //     case StencilFunc::Equal:
    //         return GL_EQUAL;
    //     case StencilFunc::Notequal:
    //         return GL_NOTEQUAL;
    //     case StencilFunc::Less:
    //         return GL_LESS;
    //     case StencilFunc::Lequal:
    //         return GL_LEQUAL;
    //     case StencilFunc::Greater:
    //         return GL_GREATER;
    //     case StencilFunc::Gequal:
    //         return GL_GEQUAL;
    //     case StencilFunc::Never:
    //         return GL_NEVER;
    //     default:
    //         return GL_ALWAYS;
    //     }
    // }

    // GLenum OpenGLRenderer::convertStencilOp(StencilOp op)
    // {
    //     switch (op)
    //     {
    //     case StencilOp::Keep:
    //         return GL_KEEP;
    //     case StencilOp::Zero:
    //         return GL_ZERO;
    //     case StencilOp::Replace:
    //         return GL_REPLACE;
    //     case StencilOp::Incr:
    //         return GL_INCR;
    //     case StencilOp::IncrWrap:
    //         return GL_INCR_WRAP;
    //     case StencilOp::Decr:
    //         return GL_DECR;
    //     case StencilOp::DecrWrap:
    //         return GL_DECR_WRAP;
    //     case StencilOp::Invert:
    //         return GL_INVERT;
    //     default:
    //         return GL_KEEP;
    //     }
    // }

    // void OpenGLRenderer::setStencilFunc(StencilFunc func, GLint ref, GLuint mask)
    // {
    //     glStencilFunc(convertStencilFunc(func), ref, mask);
    // }

    // void OpenGLRenderer::setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass)
    // {
    //     glStencilOp(convertStencilOp(sfail), convertStencilOp(dpfail), convertStencilOp(dppass));
    // }

    // void OpenGLRenderer::setStencilMask(GLuint mask)
    // {
    //     glStencilMask(mask);
    // }

    // void OpenGLRenderer::clearStencil()
    // {
    //     glClear(GL_STENCIL_BUFFER_BIT);
    // }
    Renderer::Renderer(Window &window) : m_Window(window)
    {
        switch (Backends::OXYG_CurrentBackend)
        {
        case RendererBackend::OpenGL:
            renderer = RendererFactory::createRenderer();
            break;
        default:
            break;
        }
        setCapability(RenderCapability::DepthTest, true);
        m_Window.setViewport(0, 0, m_Window.getWidth(), m_Window.getHeight());
    }
    void Renderer::clear()
    {
        if (renderer)
            renderer->clear();
    }
    void Renderer::drawTriangles(const VertexArray &vao, size_t indexCount)
    {
        if (renderer)
            renderer->drawTriangles(vao, indexCount);
    }

    void Renderer::drawLines(const VertexArray &vao, size_t indexCount, float thickness)
    {
        if (renderer)
            renderer->drawLines(vao, indexCount, thickness);
    }
    void Renderer::drawPoints(const VertexArray &vao, size_t vertexCount)
    {
        if (renderer)
            renderer->drawPoints(vao, vertexCount);
    }
    void Renderer::setCapability(RenderCapability cap, bool enable)
    {
        if (renderer)
            renderer->setCapability(cap, enable);
    }
    void Renderer::setPolygonMode(RenderPolygonMode mod, bool enable)
    {
        if (renderer)
            renderer->setPolygonMode(mod, enable);
    }
    void Renderer::setBlendFunc(RenderBlendFunc sfactor, RenderBlendFunc dfactor)
    {
        if (renderer)
            renderer->setBlendFunc(sfactor, dfactor);
    }
    void Renderer::setClearColor(const OxyColor &color)
    {
        if (renderer)
            renderer->setClearColor(color);
    }

    // void Renderer::setStencilFunc(StencilFunc func, GLint ref, GLuint mask)
    // {
    //     if (renderer)
    //         renderer->setStencilFunc(func, ref, mask);
    // }

    // void Renderer::setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass)
    // {
    //     if (renderer)
    //         renderer->setStencilOp(sfail, dpfail, dppass);
    // }
    // void Renderer::setStencilMask(GLuint mask)
    // {
    //     if (renderer)
    //         renderer->setStencilMask(mask);
    // }
    // void Renderer::clearStencil()
    // {
    //     if (renderer)
    //         renderer->clearStencil();
    // }
} // namespace OxyRender
