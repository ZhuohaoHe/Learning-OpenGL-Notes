#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

#include "Texture.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
    private:
        VertexArray* m_VAO;
        VertexBuffer* m_VBO;
        IndexBuffer* m_IBO;
    public:
        // std::vector<Vertex> vertices;
        // std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh();

        void Draw();
};


