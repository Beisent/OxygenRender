#include "OxygenRender/Buffer.h"
namespace OxyRender
{
    // 根据类型获取大小
    constexpr size_t sizeOfAttribType(VertexAttribType type) noexcept
    {
        switch (type)
        {
        case VertexAttribType::Float1:
            return sizeof(float) * 1;
        case VertexAttribType::Float2:
            return sizeof(float) * 2;
        case VertexAttribType::Float3:
            return sizeof(float) * 3;
        case VertexAttribType::Float4:
            return sizeof(float) * 4;
        case VertexAttribType::Int1:
            return sizeof(int) * 1;
        case VertexAttribType::Int2:
            return sizeof(int) * 2;
        case VertexAttribType::Int3:
            return sizeof(int) * 3;
        case VertexAttribType::Int4:
            return sizeof(int) * 4;
        default:
            return 0;
        }
    }
    // 添加属性布局
    void VertexLayout::addAttribute(const std::string &name, int location, VertexAttribType type)
    {
        attributes.push_back({name, location, type, stride});
        stride += sizeOfAttribType(type);
    }
    // OpenGL实现VertexBuffer
    OpenGLVertexBuffer::OpenGLVertexBuffer(BufferUsage usage)
    {
        GLenum glUsage = GL_STATIC_DRAW;
        // 将BufferUsage映射为OpenGL的GLenum
        switch (usage)
        {
        case BufferUsage::StaticDraw:
            glUsage = GL_STATIC_DRAW;
            break;
        case BufferUsage::DynamicDraw:
            glUsage = GL_DYNAMIC_DRAW;
            break;
        case BufferUsage::StreamDraw:
            glUsage = GL_STREAM_DRAW;
            break;
        }
        m_usage = glUsage;
        glGenBuffers(1, &m_rendererID);
    }

    
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLVertexBuffer::bind() const noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer::unbind() const noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void *data, size_t size, size_t offset)
    {
        bind();
        if (offset == 0)
        {
            glBufferData(GL_ARRAY_BUFFER, size, data, m_usage);
        }
        else
        {
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        }
    }

    // OpenGL实现IndexBuffer
    OpenGLIndexBuffer::OpenGLIndexBuffer(BufferUsage usage)
    {
        GLenum glUsage = GL_STATIC_DRAW;
        // 将BufferUsage映射为OpenGL的GLenum
        switch (usage)
        {
        case BufferUsage::StaticDraw:
            glUsage = GL_STATIC_DRAW;
            break;
        case BufferUsage::DynamicDraw:
            glUsage = GL_DYNAMIC_DRAW;
            break;
        case BufferUsage::StreamDraw:
            glUsage = GL_STREAM_DRAW;
            break;
        }
        m_usage = glUsage;
        glGenBuffers(1, &m_rendererID);
        m_count = 0;
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLIndexBuffer::bind() const noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLIndexBuffer::unbind() const noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLIndexBuffer::setData(const void *data, size_t size, size_t offset)
    {
        bind();
        m_count = static_cast<uint32_t>(size / sizeof(uint32_t));
        if (offset == 0)
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, m_usage);
        }
        else
        {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        }
    }

    uint32_t OpenGLIndexBuffer::getCount() const noexcept
    {
        return m_count;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void OpenGLVertexArray::bind() const noexcept
    {
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::unbind() const noexcept
    {
        glBindVertexArray(0);
    }

    // 设置顶点缓冲和布局
    void OpenGLVertexArray::setVertexBuffer(IBuffer *vertexBuffer, const VertexLayout &layout)
    {
        m_vertexBuffer = vertexBuffer;
        bind();
        m_vertexBuffer->bind();

        const auto &attribs = layout.getAttributes();
        for (const auto &attr : attribs)
        {
            glEnableVertexAttribArray(attr.location);
            glVertexAttribPointer(attr.location,
                                  static_cast<GLint>(sizeOfAttribType(attr.type) / sizeof(float)),
                                  GL_FLOAT,
                                  GL_FALSE,
                                  static_cast<GLsizei>(layout.getStride()),
                                  reinterpret_cast<const void *>(attr.offset));
        }
    }

    void OpenGLVertexArray::setIndexBuffer(IndexBuffer *indexBuffer)
    {
        m_indexBuffer = indexBuffer;
        bind();
        m_indexBuffer->bind();
    }

    IndexBuffer *OpenGLVertexArray::getIndexBuffer() const noexcept
    {
        return m_indexBuffer;
    }
    // 创建缓冲对象
    std::unique_ptr<IBuffer> BufferFactory::createBuffer(BufferType type, BufferUsage usage)
    {

        if (Backends::OXYG_CurrentBackend == RendererBackend::OpenGL)
        {
            // 根据类型创建对应的缓冲对象
            if (type == BufferType::Vertex)
            {
                return std::make_unique<OpenGLVertexBuffer>(usage);
            }
            else if (type == BufferType::Index)
            {
                return std::make_unique<OpenGLIndexBuffer>(usage);
            }
        }
        throw std::runtime_error("Unsupported backend or buffer type");
    }
    Buffer::Buffer(BufferType type, BufferUsage usage) : m_type(type)
    {
        m_buffer = BufferFactory::createBuffer(type, usage);
    }
    void Buffer::bind()
    {
        m_buffer->bind();
    }
    void Buffer::unbind()
    {
        m_buffer->unbind();
    }
    void Buffer::setData(const void *data, size_t size, size_t offset)
    {
        m_buffer->setData(data, size, offset);
    }
    std::unique_ptr<IVertexArray> VertexArrayFactory::create()
    {
        if (Backends::OXYG_CurrentBackend == RendererBackend::OpenGL)
        {
            return std::make_unique<OpenGLVertexArray>();
        }
        throw std::runtime_error("Unsupported backend for VAO");
    }
    VertexArray::VertexArray()
    {
        m_vao = VertexArrayFactory::create();
    }
    void VertexArray::bind() const
    {
        m_vao->bind();
    }
    void VertexArray::unbind() const
    {
        m_vao->unbind();
    }
    void VertexArray::setVertexBuffer(Buffer &vertexBuffer, const VertexLayout &layout)
    {
        if (vertexBuffer.getType() != BufferType::Vertex)
            throw std::runtime_error("setVertexBuffer: Provided buffer is not a VertexBuffer");

        m_vao->setVertexBuffer(vertexBuffer.asIBuffer(), layout);
    }
    void VertexArray::setIndexBuffer(Buffer &indexBuffer)
    {
        auto ib = indexBuffer.asIndexBuffer();
        if (!ib)
            throw std::runtime_error("setIndexBuffer: Provided buffer is not an IndexBuffer");

        m_vao->setIndexBuffer(ib);
    }
    IndexBuffer *VertexArray::getIndexBuffer() const
    {
        return m_vao->getIndexBuffer();
    }
}