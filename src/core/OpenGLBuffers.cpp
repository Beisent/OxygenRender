#include "OxygenRender/OpenGLBuffers.h"

namespace OxyRender
{
    GLVertexBuffer::GLVertexBuffer(const void *data, size_t size, const OxyRender::VertexLayout &layout)
        : m_layout(layout), m_size(size)
    {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
    }

    GLVertexBuffer::~GLVertexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void GLVertexBuffer::bind() const noexcept { glBindBuffer(GL_ARRAY_BUFFER, m_rendererID); }
    void GLVertexBuffer::unbind() const noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void GLVertexBuffer::setData(const void *data, size_t size, size_t offset)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        if (offset == 0 && size == m_size)
            glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
        else
            glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, (GLsizeiptr)size, data);
    }
    GLIndexBuffer::GLIndexBuffer(const uint32_t *data, uint32_t count)
        : m_count(count)
    {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(uint32_t) * count), data, GL_STATIC_DRAW);
    }

    GLIndexBuffer::~GLIndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void GLIndexBuffer::bind() const noexcept { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID); }
    void GLIndexBuffer::unbind() const noexcept { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    void GLIndexBuffer::setData(const void *data, size_t size, size_t offset)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        if (offset == 0 && size == sizeof(uint32_t) * m_count)
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
        else
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)offset, (GLsizeiptr)size, data);
    }

    uint32_t GLIndexBuffer::getCount() const noexcept { return m_count; }

    GLVertexArray::GLVertexArray()
    {
        glGenVertexArrays(1, &m_rendererID);
    }

    GLVertexArray::~GLVertexArray()
    {
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void GLVertexArray::bind() const noexcept { glBindVertexArray(m_rendererID); }
    void GLVertexArray::unbind() const noexcept { glBindVertexArray(0); }

    void GLVertexArray::addVertexBuffer(const std::shared_ptr<OxyRender::VertexBuffer> &vbo)
    {
        assert(vbo);
        std::shared_ptr<GLVertexBuffer> glvbo = std::dynamic_pointer_cast<GLVertexBuffer>(vbo);
        if (!glvbo)
            throw std::runtime_error("addVertexBuffer requires GLVertexBuffer in this implementation");

        bind();
        glvbo->bind();

        const OxyRender::VertexLayout &layout = glvbo->getLayout();
        const auto &attrs = layout.getAttributes();
        size_t stride = layout.getStride();

        for (const auto &attr : attrs)
        {
            int loc = attr.location;
            int comp = ATTRIB_COMPONENT_COUNT(attr.type);
            GLenum gtype = ATTRIB_GL_TYPE(attr.type);

            glEnableVertexAttribArray(loc);
            if (gtype == GL_FLOAT)
            {
                glVertexAttribPointer(loc, comp, gtype, GL_FALSE, (GLsizei)stride, (const void *)attr.offset);
            }
            else // integer types
            {
                glVertexAttribIPointer(loc, comp, gtype, (GLsizei)stride, (const void *)attr.offset);
            }
        }

        m_vertexBuffers.push_back(vbo);
        // leave VBO bound to VAO state
    }

    void GLVertexArray::setIndexBuffer(const std::shared_ptr<OxyRender::IndexBuffer> &ebo)
    {
        assert(ebo);
        std::shared_ptr<GLIndexBuffer> glEbo = std::dynamic_pointer_cast<GLIndexBuffer>(ebo);
        if (!glEbo)
            throw std::runtime_error("setIndexBuffer requires GLIndexBuffer in this implementation");

        bind();
        glEbo->bind();
        m_indexBuffer = ebo;
    }
}