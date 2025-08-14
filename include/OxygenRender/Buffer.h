#pragma once
#include <cstdint>
#include "OxygenRender/GraphicsTypes.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <glad/glad.h>

namespace OxyRender
{

    class VertexLayout;
    enum class BufferType
    {
        Vertex, // 顶点缓冲
        Index   // 索引缓冲
    };

    enum class BufferUsage
    {
        StaticDraw,
        DynamicDraw,
        StreamDraw
    };

    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;
        virtual void bind() const noexcept = 0;
        virtual void unbind() const noexcept = 0;
        virtual void setData(const void *data, size_t size, size_t offset = 0) = 0;
    };

    class IndexBuffer : public IBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        virtual uint32_t getCount() const noexcept = 0;
    };
    class IVertexArray
    {
    public:
        virtual ~IVertexArray() = default;
        virtual void bind() const noexcept = 0;
        virtual void unbind() const noexcept = 0;
        virtual void setVertexBuffer(IBuffer *vertexBuffer, const VertexLayout &layout) = 0;
        virtual void setIndexBuffer(IndexBuffer *indexBuffer) = 0;
        virtual IndexBuffer *getIndexBuffer() const noexcept = 0;
    };

    enum class VertexAttribType
    {
        Float1,
        Float2,
        Float3,
        Float4,
        Int1,
        Int2,
        Int3,
        Int4
    };

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

    struct VertexAttribute
    {
        std::string name;
        int location; // location in shader
        VertexAttribType type;
        size_t offset; // offset in vertex
    };

    class VertexLayout
    {
    public:
        VertexLayout() noexcept : stride(0) {}

        void addAttribute(const std::string &name, int location, VertexAttribType type)
        {
            attributes.push_back({name, location, type, stride});
            stride += sizeOfAttribType(type);
        }

        const std::vector<VertexAttribute> &getAttributes() const noexcept { return attributes; }
        size_t getStride() const noexcept { return stride; }

    private:
        std::vector<VertexAttribute> attributes;
        size_t stride;
    };

    class OpenGLVertexBuffer : public IBuffer
    {
    public:
        OpenGLVertexBuffer(BufferUsage usage);
        ~OpenGLVertexBuffer();

        void bind() const noexcept override;
        void unbind() const noexcept override;
        void setData(const void *data, size_t size, size_t offset = 0) override;

    private:
        GLuint m_rendererID;
        GLenum m_usage;
    };
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(BufferUsage usage);
        ~OpenGLIndexBuffer();

        void bind() const noexcept override;
        void unbind() const noexcept override;
        void setData(const void *data, size_t size, size_t offset = 0) override;
        uint32_t getCount() const noexcept override;

    private:
        GLuint m_rendererID;
        GLenum m_usage;
        uint32_t m_count;
    };

    class OpenGLVertexArray : public IVertexArray
    {
    public:
        OpenGLVertexArray();

        ~OpenGLVertexArray();

        void bind() const noexcept override;
        void unbind() const noexcept override;
        void setVertexBuffer(IBuffer *vertexBuffer, const VertexLayout &layout) override;
        void setIndexBuffer(IndexBuffer *indexBuffer) override;
        IndexBuffer *getIndexBuffer() const noexcept override;

    private:
        GLuint m_rendererID;
        IBuffer *m_vertexBuffer;
        IndexBuffer *m_indexBuffer;
    };

    class BufferFactory
    {
    public:
        static std::unique_ptr<IBuffer> createBuffer(BufferType type, BufferUsage usage = BufferUsage::StaticDraw)
        {
            if (OXYG_CurrentBackend == RendererBackend::OpenGL)
            {
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
    };

    class Buffer
    {
    private:
        std::unique_ptr<IBuffer> m_buffer;
        BufferType m_type;

    public:
        Buffer(BufferType type, BufferUsage usage);
        void bind();
        void unbind();
        void setData(const void *data, size_t size, size_t offset = 0);
        BufferType getType() const noexcept { return m_type; }

        IBuffer *asIBuffer() noexcept { return m_buffer.get(); }
        const IBuffer *asIBuffer() const noexcept { return m_buffer.get(); }

        IndexBuffer *asIndexBuffer() noexcept
        {
            return (m_type == BufferType::Index) ? static_cast<IndexBuffer *>(m_buffer.get()) : nullptr;
        }
        const IndexBuffer *asIndexBuffer() const noexcept
        {
            return (m_type == BufferType::Index) ? static_cast<const IndexBuffer *>(m_buffer.get()) : nullptr;
        }
    };

    class VertexArrayFactory
    {
    public:
        static std::unique_ptr<IVertexArray> create()
        {
            if (OXYG_CurrentBackend == RendererBackend::OpenGL)
            {
                return std::make_unique<OpenGLVertexArray>();
            }
            throw std::runtime_error("Unsupported backend for VAO");
        }
    };

    class VertexArray
    {
    private:
        std::shared_ptr<IVertexArray> m_vao;

    public:
        VertexArray();
        virtual void bind() const;
        virtual void unbind() const;
        virtual void setVertexBuffer(Buffer& vertexBuffer, const VertexLayout &layout);
        virtual void setIndexBuffer(Buffer& indexBuffer);
        virtual IndexBuffer *getIndexBuffer() const;
    };

}
