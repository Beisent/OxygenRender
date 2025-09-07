#include "OxygenRender/Mesh.h"

namespace OxyRender
{

    Mesh::Mesh(Renderer &renderer, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : m_Renderer(renderer),
          vertices(std::move(vertices)),
          indices(std::move(indices)),
          textures(std::move(textures)),
          m_VBO(BufferType::Vertex, BufferUsage::StaticDraw),
          m_EBO(BufferType::Index, BufferUsage::StaticDraw)
    {
        setupMesh();
    }

    void Mesh::setupMesh()
    {
        VertexLayout layout;
        layout.addAttribute("Position", 0, VertexAttribType::Float3);
        layout.addAttribute("Normal", 1, VertexAttribType::Float3);
        layout.addAttribute("TexCoords", 2, VertexAttribType::Float2);
        layout.addAttribute("Tangent", 3, VertexAttribType::Float3);
        layout.addAttribute("Bitangent", 4, VertexAttribType::Float3);
        layout.addAttribute("BoneIDs", 5, VertexAttribType::Int4);
        layout.addAttribute("Weights", 6, VertexAttribType::Float4);

        m_VBO.setData(vertices.data(), vertices.size() * sizeof(Vertex));
        m_EBO.setData(indices.data(), indices.size() * sizeof(unsigned int));

        m_VAO.bind();
        m_VAO.setVertexBuffer(m_VBO, layout);
        m_VAO.setIndexBuffer(m_EBO);
        m_VAO.unbind();
    }

    void Mesh::Draw(Shader &shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {

            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            shader.setUniformData(name + number, &i, sizeof(i));
            
            if (textures[i].tex)
                textures[i].tex->bind(i);
        }

        m_Renderer.drawTriangles(m_VAO, indices.size());
    }

} // namespace OxyRender