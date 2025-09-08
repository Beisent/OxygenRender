// 来自LearnOpenGL教程
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OxygenRender/Mesh.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace OxyRender
{

    class Model
    {
    public:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;

        Model(Renderer& renderer,const std::string &path, bool gamma = false);
        void Draw(Shader &shader);

        // 使用内置源码创建默认模型着色器
        static Shader CreateDefaultShader();

    private:
        Renderer &m_Renderer;
        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);

        // 硬编码的模型着色器源码
        static const char* s_vertexShaderSrc;
        static const char* s_fragmentShaderSrc;
    };

    std::shared_ptr<Texture2D> TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

} // namespace OxyRender