
#include "OxygenRender/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
 #include <stb_image.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace OxyRender
{

    struct Model::Impl
    {

        void loadModel(Model *self, const std::string &path);
        void processNode(Model *self, aiNode *node, const aiScene *scene);
        Mesh processMesh(Model *self, aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(Model *self, aiMaterial *mat, aiTextureType type, const std::string &typeName);
    };

    const char *Model::s_vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(transpose(inverse(model))) * aNormal);

    TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

    const char *Model::s_fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 specularColor = texture(texture_specular1, TexCoords).rgb;

    vec3 tangentNormal = texture(texture_normal1, TexCoords).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;
    vec3 norm = normalize(TBN * tangentNormal);

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);

    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * specularColor;

    vec3 result = ambient + diffuse + specular;
    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, 1.0);
}
)";

    Shader Model::CreateDefaultShader()
    {
        return Shader("model_shader_builtin", s_vertexShaderSrc, s_fragmentShaderSrc);
    }

    // 👇 构造函数：创建 pImpl
    Model::Model(Renderer &renderer, const std::string &path, bool gamma)
        : m_Renderer(renderer), gammaCorrection(gamma), pImpl(std::make_unique<Impl>())
    {
        stbi_set_flip_vertically_on_load(1);
        pImpl->loadModel(this, path);
    }

    // 👇 析构函数（必须在 .cpp 中定义，因为 unique_ptr 需要知道 Impl 的完整定义）
    Model::~Model() = default;

    void Model::Draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    // 👇 实现细节全部移到 Impl 中
    void Model::Impl::loadModel(Model *self, const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        self->directory = path.substr(0, path.find_last_of('/'));
        processNode(self, scene->mRootNode, scene);
    }

    void Model::Impl::processNode(Model *self, aiNode *node, const aiScene *scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            self->meshes.push_back(processMesh(self, mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(self, node->mChildren[i], scene);
        }
    }

    Mesh Model::Impl::processMesh(Model *self, aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(self, material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(self, material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps = loadMaterialTextures(self, material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps = loadMaterialTextures(self, material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(self->m_Renderer, vertices, indices, textures);
    }

    std::vector<Texture> Model::Impl::loadMaterialTextures(Model *self, aiMaterial *mat, aiTextureType type, const std::string &typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < self->textures_loaded.size(); j++)
            {
                if (std::strcmp(self->textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(self->textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                Texture texture;
                texture.tex = TextureFromFile(str.C_Str(), self->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                self->textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

    std::shared_ptr<Texture2D> TextureFromFile(const char *path, const std::string &directory, bool gamma)
    {
        std::string filename = directory + '/' + std::string(path);
        try
        {
            auto texture = std::make_shared<Texture2D>(filename);
            return texture;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Texture failed to load at path: " << filename << "\n"
                      << e.what() << std::endl;
            return nullptr;
        }
    }

} // namespace OxyRender