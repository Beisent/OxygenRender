#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>
#include "OxygenRender/Buffer.h"

namespace OxyRender
{
    static int ATTRIB_COMPONENT_COUNT(OxyRender::VertexAttribType t)
    {
        using VT = OxyRender::VertexAttribType;
        switch (t)
        {
        case VT::Float1:
        case VT::Int1:
            return 1;
        case VT::Float2:
        case VT::Int2:
            return 2;
        case VT::Float3:
        case VT::Int3:
            return 3;
        case VT::Float4:
        case VT::Int4:
            return 4;
        default:
            return 0;
        }
    }

    static GLenum ATTRIB_GL_TYPE(OxyRender::VertexAttribType t)
    {
        using VT = OxyRender::VertexAttribType;
        switch (t)
        {
        case VT::Float1:
        case VT::Float2:
        case VT::Float3:
        case VT::Float4:
            return GL_FLOAT;
        case VT::Int1:
        case VT::Int2:
        case VT::Int3:
        case VT::Int4:
            return GL_INT;
        default:
            return GL_FLOAT;
        }
    }

    class GLVertexBuffer : public OxyRender::VertexBuffer
    {
    public:
        GLVertexBuffer(const void *data, size_t size, const OxyRender::VertexLayout &layout);
        ~GLVertexBuffer() override;
        
        void bind() const noexcept override;
        void unbind() const noexcept override;

        void setData(const void *data, size_t size, size_t offset = 0) override;
        const OxyRender::VertexLayout &getLayout() const noexcept override { return m_layout; }

    private:
        GLuint m_rendererID{0};
        OxyRender::VertexLayout m_layout;
        size_t m_size{0};
    };

    class GLIndexBuffer : public OxyRender::IndexBuffer
    {
    public:
        GLIndexBuffer(const uint32_t *data, uint32_t count);
        ~GLIndexBuffer() override;

        void bind() const noexcept override;
        void unbind() const noexcept override;

        void setData(const void *data, size_t size, size_t offset = 0) override;
        uint32_t getCount() const noexcept override;

    private:
        GLuint m_rendererID{0};
        uint32_t m_count{0};
    };

    class GLVertexArray : public OxyRender::VertexArray
    {
    public:
        GLVertexArray();
        ~GLVertexArray() override;
        
        void bind() const noexcept override;
        void unbind() const noexcept override;

        void addVertexBuffer(const std::shared_ptr<OxyRender::VertexBuffer> &vbo) override;
        void setIndexBuffer(const std::shared_ptr<OxyRender::IndexBuffer> &ebo) override;
        std::shared_ptr<OxyRender::IndexBuffer> getIndexBuffer() const noexcept override { return m_indexBuffer; }

    private:
        GLuint m_rendererID{0};
        std::vector<std::shared_ptr<OxyRender::VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<OxyRender::IndexBuffer> m_indexBuffer;
    };

}