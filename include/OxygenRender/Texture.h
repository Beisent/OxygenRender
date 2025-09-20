#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "OxygenRender/GraphicsTypes.h"

namespace OxyRender
{
    // Texture相关枚举
    enum class TextureFormat
    {
        RGBA8,
        RGB8,
        DEPTH24STENCIL8
    };
    // Texture过滤和环绕模式
    enum class TextureFilter
    {
        Nearest,
        Linear
    };
    // 纹理环绕模式
    enum class TextureWrap
    {
        Repeat,
        ClampToEdge
    };
    // Texture 抽象接口类
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

    // OpenGL实现Texture2D
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
        uint32_t m_rendererID;
        uint32_t m_width, m_height;
        uint32_t m_format;
        uint32_t m_internalFormat;
        uint32_t m_filter;
        uint32_t m_wrap;
    };

    // Texture2D类对外接口
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

    class ICubemap
    {
    public:
        virtual ~ICubemap() = default;
        virtual void bind(uint32_t slot = 0) const noexcept = 0;
        virtual void unbind() const noexcept = 0;
        virtual uint32_t getRendererID() const noexcept = 0;
    };

    // OpenGL实现Cubemap
    class OpenGLCubemap : public ICubemap
    {
    public:
        OpenGLCubemap(const std::vector<std::string> &faces);
        ~OpenGLCubemap();

        void bind(uint32_t slot = 0) const noexcept override;
        void unbind() const noexcept override;
        inline uint32_t getRendererID() const noexcept override { return m_rendererID; }

    private:
        uint32_t m_rendererID;
    };

    // Texture工厂类
    class TextureFactory
    {
    public:
        static std::unique_ptr<ITexture> createTexture2D(const std::string &path, TextureFilter filter = TextureFilter::Linear, TextureWrap wrap = TextureWrap::Repeat);
        static std::unique_ptr<ICubemap> createCubemap(const std::vector<std::string> &faces);
    };
    // Cubemap类对外接口
    class Cubemap
    {
    private:
        std::shared_ptr<ICubemap> m_cubemap;

    public:
        Cubemap(const std::vector<std::string> &faces);
        inline void bind(uint32_t slot = 0) const { m_cubemap->bind(slot); }
        inline void unbind() const { m_cubemap->unbind(); }
        inline uint32_t getRendererID() const { return m_cubemap->getRendererID(); }
    };
}
