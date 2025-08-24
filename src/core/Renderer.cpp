#include "OxygenRender/Renderer.h"
#include <glad/glad.h>
namespace OxyRender
{
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
    void Renderer::setClearColor(const OxyColor &color)
    {
        renderer->setClearColor(color);
    }
} // namespace OxyRender
