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
        m_triVertices.clear();
        m_triIndices.clear();
        m_triIndexCount = 0;

        m_lineBatches.clear();
    }

    void Graphics2D::drawRect(float x, float y, float width, float height, glm::vec4 color)
    {
        Vertex v0 = {{x, y, 0.0f}, color};
        Vertex v1 = {{x + width, y, 0.0f}, color};
        Vertex v2 = {{x + width, y + height, 0.0f}, color};
        Vertex v3 = {{x, y + height, 0.0f}, color};

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        m_triVertices.push_back(v0);
        m_triVertices.push_back(v1);
        m_triVertices.push_back(v2);
        m_triVertices.push_back(v3);

        m_triIndices.push_back(startIndex + 0);
        m_triIndices.push_back(startIndex + 1);
        m_triIndices.push_back(startIndex + 2);
        m_triIndices.push_back(startIndex + 2);
        m_triIndices.push_back(startIndex + 3);
        m_triIndices.push_back(startIndex + 0);

        m_triIndexCount += 6;
    }

    void Graphics2D::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, glm::vec4 color)
    {
        Vertex v1 = {{x1, y1, 0.0f}, color};
        Vertex v2 = {{x2, y2, 0.0f}, color};
        Vertex v3 = {{x3, y3, 0.0f}, color};

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        m_triVertices.push_back(v1);
        m_triVertices.push_back(v2);
        m_triVertices.push_back(v3);

        m_triIndices.push_back(startIndex + 0);
        m_triIndices.push_back(startIndex + 1);
        m_triIndices.push_back(startIndex + 2);

        m_triIndexCount += 3;
    }

    void Graphics2D::drawLine(float x1, float y1, float x2, float y2, glm::vec4 color, float thickness)
    {

        LineBatch *batch = nullptr;
        for (auto &b : m_lineBatches)
        {
            if (fabs(b.thickness - thickness) < 0.001f)
            {
                batch = &b;
                break;
            }
        }
        if (!batch)
        {
            m_lineBatches.push_back(LineBatch{thickness});
            batch = &m_lineBatches.back();
        }

        Vertex v1 = {{x1, y1, 0.0f}, color};
        Vertex v2 = {{x2, y2, 0.0f}, color};

        unsigned int startIndex = (unsigned int)batch->vertices.size();
        batch->vertices.push_back(v1);
        batch->vertices.push_back(v2);

        batch->indices.push_back(startIndex + 0);
        batch->indices.push_back(startIndex + 1);

        batch->indexCount += 2;
    }



    void Graphics2D::flush()
    {
        if (m_triIndexCount == 0 && m_lineBatches.empty())
            return;

        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setCapability(RenderCapability::CullFace, false);
        m_renderer.setCapability(RenderCapability::DepthTest, false);
        m_renderer.setCapability(RenderCapability::StencilTest, false);

        m_shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.Get2DOrthoViewMatrix();
        glm::mat4 projection = m_camera.Get2DOrthoProjectionMatrix(m_window.getWidth(), m_window.getHeight());

        m_shader.setUniformData("model", &model, sizeof(glm::mat4));
        m_shader.setUniformData("view", &view, sizeof(glm::mat4));
        m_shader.setUniformData("projection", &projection, sizeof(glm::mat4));

        m_vao.bind();

        // 绘制三角形
        if (m_triIndexCount > 0)
        {
            m_vbo.setData(m_triVertices.data(), m_triVertices.size() * sizeof(Vertex));
            m_ebo.setData(m_triIndices.data(), m_triIndices.size() * sizeof(unsigned int));
            glDrawElements(GL_TRIANGLES, (GLsizei)m_triIndexCount, GL_UNSIGNED_INT, 0);
        }

        // 绘制线段按不同厚度分别画
        for (auto &batch : m_lineBatches)
        {
            if (batch.indexCount == 0)
                continue;

            glLineWidth(batch.thickness);
            m_vbo.setData(batch.vertices.data(), batch.vertices.size() * sizeof(Vertex));
            m_ebo.setData(batch.indices.data(), batch.indices.size() * sizeof(unsigned int));
            glDrawElements(GL_LINES, (GLsizei)batch.indexCount, GL_UNSIGNED_INT, 0);
        }

        m_vao.unbind();

        m_triIndexCount = 0;
        m_lineBatches.clear();
    }
}
