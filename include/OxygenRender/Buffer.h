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

    // Vertex属性结构体
    struct VertexAttribute
    {
        std::string name;
        int location; // location in shader
        VertexAttribType type;
        size_t offset; // offset in vertex
    };

    // Buffer 抽象接口类
    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;
        virtual void bind() const noexcept = 0;
        virtual void unbind() const noexcept = 0;
        virtual void setData(const void *data, size_t size, size_t offset = 0) = 0;
    };

    // 索引缓冲抽象接口类
    class IndexBuffer : public IBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        virtual uint32_t getCount() const noexcept = 0;
    };

    // 顶点数组抽象接口类
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

    // 顶点布局抽象接口类
    class VertexLayout
    {
    public:
        VertexLayout() noexcept : stride(0) {}

        void addAttribute(const std::string &name, int location, VertexAttribType type);
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
        inline uint32_t getCount() const noexcept override;

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
        static std::unique_ptr<IBuffer> createBuffer(BufferType type, BufferUsage usage = BufferUsage::StaticDraw);
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

        inline BufferType getType() const noexcept { return m_type; }
        inline IBuffer *asIBuffer() noexcept { return m_buffer.get(); }
        inline const IBuffer *asIBuffer() const noexcept { return m_buffer.get(); }
        inline IndexBuffer *asIndexBuffer() noexcept
        {
            return (m_type == BufferType::Index) ? static_cast<IndexBuffer *>(m_buffer.get()) : nullptr;
        }
        inline const IndexBuffer *asIndexBuffer() const noexcept
        {
            return (m_type == BufferType::Index) ? static_cast<const IndexBuffer *>(m_buffer.get()) : nullptr;
        }
    };

    class VertexArrayFactory
    {
    public:
        static std::unique_ptr<IVertexArray> create();
    };

    class VertexArray
    {
    private:
        std::shared_ptr<IVertexArray> m_vao;

    public:
        VertexArray();
        void bind() const;
        void unbind() const;
        void setVertexBuffer(Buffer &vertexBuffer, const VertexLayout &layout);
        void setIndexBuffer(Buffer &indexBuffer);
        inline IndexBuffer *getIndexBuffer() const;
    };

}
