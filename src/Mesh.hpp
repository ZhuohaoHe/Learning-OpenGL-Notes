#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

#include "Texture.hpp"
#include "Shader.hpp"

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
        std::vector<Texture*> textures;

    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
        ~Mesh();

        void Render(Shader* shader);
};


