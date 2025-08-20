#include "OxygenRender/Graphics2D.h"

namespace OxyRender
{
    Graphics2D::Graphics2D(Window &window, Renderer &renderer)
        : m_window(window),
          m_renderer(renderer),
          m_camera(glm::vec3(0, 0, 10.0f)),
          m_shader("default", "shaders/graphics2d/vertex.vert", "shaders/graphics2d/fragment.frag"),
          m_vbo(BufferType::Vertex, BufferUsage::DynamicDraw),
          m_ebo(BufferType::Index, BufferUsage::DynamicDraw)
    {
       
        VertexLayout layout;
        layout.addAttribute("aPos", 0, VertexAttribType::Float3);
        layout.addAttribute("aColor", 1, VertexAttribType::Float4);

        m_vao.setVertexBuffer(m_vbo, layout);
        m_vao.setIndexBuffer(m_ebo);
    }

    void Graphics2D::begin()
    {
        m_cpuVertices.clear();
        m_cpuIndices.clear();
        m_indexCount = 0;
    }

    void Graphics2D::drawRect(float x, float y, float width, float height, glm::vec4 color)
    {
        Vertex v0 = {{x, y, 0.0f}, color};
        Vertex v1 = {{x + width, y, 0.0f}, color};
        Vertex v2 = {{x + width, y + height, 0.0f}, color};
        Vertex v3 = {{x, y + height, 0.0f}, color};

        unsigned int startIndex = (unsigned int)m_cpuVertices.size();
        m_cpuVertices.push_back(v0);
        m_cpuVertices.push_back(v1);
        m_cpuVertices.push_back(v2);
        m_cpuVertices.push_back(v3);

        m_cpuIndices.push_back(startIndex + 0);
        m_cpuIndices.push_back(startIndex + 1);
        m_cpuIndices.push_back(startIndex + 2);
        m_cpuIndices.push_back(startIndex + 2);
        m_cpuIndices.push_back(startIndex + 3);
        m_cpuIndices.push_back(startIndex + 0);

        m_indexCount += 6;
    }

    void Graphics2D::flush()
    {
        if (m_indexCount == 0)
            return;

        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setCapability(RenderCapability::CullFace, false);
        m_renderer.setCapability(RenderCapability::DepthTest, false);
        m_renderer.setCapability(RenderCapability::StencilTest, false);

        // 上传数据
        m_vbo.setData(m_cpuVertices.data(), m_cpuVertices.size() * sizeof(Vertex));
        m_ebo.setData(m_cpuIndices.data(), m_cpuIndices.size() * sizeof(unsigned int));

        m_shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.Get2DOrthoViewMatrix();
        glm::mat4 projection = m_camera.Get2DOrthoProjectionMatrix(m_window.getWidth(), m_window.getHeight());

        m_shader.setUniformData("model", &model, sizeof(glm::mat4));
        m_shader.setUniformData("view", &view, sizeof(glm::mat4));
        m_shader.setUniformData("projection", &projection, sizeof(glm::mat4));

        m_vao.bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)m_indexCount, GL_UNSIGNED_INT, 0);
        m_vao.unbind();

        m_indexCount = 0;
    }
}
