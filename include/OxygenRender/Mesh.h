// 来自LearnOpenGL
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "OxygenRender/Buffer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Texture.h"

#define MAX_BONE_INFLUENCE 4

namespace OxyRender
{

    struct Vertex
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        // bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        // weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Texture
    {
        std::shared_ptr<Texture2D> tex;
        std::string type;
        std::string path;
    };

    class Mesh
    {
    public:
        // mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // constructor
        Mesh(Renderer& renderer,std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        // render the mesh
        void Draw(Shader &shader);

    private:
        VertexArray m_VAO;
        Buffer m_VBO;
        Buffer m_EBO;

        Renderer& m_Renderer; 
        void setupMesh();
    };

} // namespace OxyRender