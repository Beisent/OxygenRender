#define STB_IMAGE_IMPLEMENTATION
#include "OxygenRender/Texture.h"
 #include <stb_image.h>
namespace OxyRender
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string &path,
                                     TextureFilter filter,
                                     TextureWrap wrap)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1); // 翻转Y轴
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            throw std::runtime_error("Failed to load texture: " + path);
        }

        m_width = width;
        m_height = height;

        // 根据通道数设置格式
        switch (channels)
        {
        case 3:
            m_internalFormat = GL_RGB8;
            m_format = GL_RGB;
            break;
        case 4:
            m_internalFormat = GL_RGBA8;
            m_format = GL_RGBA;
            break;
        default:
            stbi_image_free(data);
            throw std::runtime_error("Unsupported number of channels in texture: " + path);
        }

        glGenTextures(1, &m_rendererID);
        glBindTexture(GL_TEXTURE_2D, m_rendererID);

        // 设置过滤器
        m_filter = (filter == TextureFilter::Linear) ? GL_LINEAR : GL_NEAREST;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);

        // 设置包装模式
        m_wrap = (wrap == TextureWrap::Repeat) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap);

        // 上传数据
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_rendererID);
    }

    void OpenGLTexture2D::bind(uint32_t slot) const noexcept
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_rendererID);
    }

    void OpenGLTexture2D::unbind() const noexcept
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void OpenGLTexture2D::setData(const void *data, uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;
        glBindTexture(GL_TEXTURE_2D, m_rendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    std::unique_ptr<ITexture> TextureFactory::createTexture2D(const std::string &path, TextureFilter filter, TextureWrap wrap)
    {
        if (Backends::OXYG_CurrentBackend == RendererBackend::OpenGL)
        {
            return std::make_unique<OpenGLTexture2D>(path, filter, wrap);
        }
        throw std::runtime_error("Unsupported backend for Texture");
    }
    Texture2D::Texture2D(const std::string &path, TextureFilter filter, TextureWrap wrap)
    {
        if (Backends::OXYG_CurrentBackend == RendererBackend::OpenGL)
        {
            m_texture = std::make_shared<OpenGLTexture2D>(path, filter, wrap);
        }
        else
        {
            throw std::runtime_error("Unsupported backend for Texture2D");
        }
    }

}