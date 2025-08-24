#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include "OxygenRender/GraphicsTypes.h"
#include <stb_image.h>
#include <glad/glad.h>

namespace OxyRender
{
    enum class TextureFormat
    {
        RGBA8,
        RGB8,
        DEPTH24STENCIL8
    };

    enum class TextureFilter
    {
        Nearest,
        Linear
    };

    enum class TextureWrap
    {
        Repeat,
        ClampToEdge
    };

    class ITexture
    {
    public:
        virtual ~ITexture() = default;
        virtual void bind(uint32_t slot = 0) const noexcept = 0;
        virtual void unbind() const noexcept = 0;
        virtual void setData(const void *data, uint32_t width, uint32_t height) = 0;
        virtual uint32_t getWidth() const noexcept = 0;
        virtual uint32_t getHeight() const noexcept = 0;
    };

    class OpenGLTexture2D : public ITexture
    {
    public:
        OpenGLTexture2D(const std::string &path,
                        TextureFilter filter = TextureFilter::Linear,
                        TextureWrap wrap = TextureWrap::Repeat);

        ~OpenGLTexture2D();

        void bind(uint32_t slot = 0) const noexcept override;
        void unbind() const noexcept override;

        void setData(const void *data, uint32_t width, uint32_t height) override;

        inline uint32_t getWidth() const noexcept override { return m_width; }
        inline uint32_t getHeight() const noexcept override { return m_height; }

    private:
        GLuint m_rendererID;
        uint32_t m_width, m_height;
        GLenum m_format;
        GLenum m_internalFormat;
        GLenum m_filter;
        GLenum m_wrap;
    };

    class TextureFactory
    {
    public:
        static std::unique_ptr<ITexture> createTexture2D(const std::string &path, TextureFilter filter = TextureFilter::Linear, TextureWrap wrap = TextureWrap::Repeat);
    };
    class Texture2D
    {
    private:
        std::shared_ptr<ITexture> m_texture;

    public:
        Texture2D() = default;
        Texture2D(const std::string &path, TextureFilter filter = TextureFilter::Linear, TextureWrap wrap = TextureWrap::Repeat);
        inline void bind(uint32_t slot = 0) const { m_texture->bind(slot); }
        inline void unbind() const { m_texture->unbind(); }
        inline void setData(const void *data, uint32_t width, uint32_t height) { m_texture->setData(data, width, height); }
        inline uint32_t getWidth() const noexcept { return m_texture->getWidth(); }
        inline uint32_t getHeight() const noexcept { return m_texture->getHeight(); }
    };
}
