#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
namespace OxyRender
{

    class Buffer
    {
    public:
        virtual ~Buffer() = default;
        virtual void bind() const noexcept = 0;
        virtual void unbind() const noexcept = 0;
        virtual void setData(const void *data, size_t size, size_t offset = 0) = 0;
    };

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() = default;
        virtual uint32_t getCount() const noexcept = 0;
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
        int location;
        VertexAttribType type;
        size_t offset;
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

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() = default;
        virtual const VertexLayout &getLayout() const noexcept = 0;
    };

    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const noexcept = 0;
        virtual void unbind() const noexcept = 0;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vbo) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> &ebo) = 0;
        virtual std::shared_ptr<IndexBuffer> getIndexBuffer() const noexcept = 0;
    };
}
