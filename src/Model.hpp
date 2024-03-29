#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public :
        Model(const char *path);
        ~Model();
        void Render(Shader *shader);
    private :
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Texture>> loaded_textures;
        std::string directory;
        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType tex_type);
};