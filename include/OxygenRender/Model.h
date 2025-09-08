

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "OxygenRender/Mesh.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
namespace OxyRender
{
    class Renderer;

    class Model
    {
    public:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;

        Model(Renderer &renderer, const std::string &path, bool gamma = false);
        ~Model();
        void Draw(Shader &shader);

        static Shader CreateDefaultShader();

    private:
        Renderer &m_Renderer;
        static const char *s_vertexShaderSrc;
        static const char *s_fragmentShaderSrc;
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };

    std::shared_ptr<Texture2D> TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
}