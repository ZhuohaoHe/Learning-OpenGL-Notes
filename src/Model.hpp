// #pragma once

// #include <vector>
// #include "Mesh.hpp"
// #include "Shader.hpp"

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

// class Model {
//     public :
//         Model(char *path);
//         void Draw(Shader shader);
//     private :
//         std::vector<Mesh> meshes;
//         std::vector<Texture> loaded_textures;
//         std::string directory;
//         void loadModel(const std::string path);
//         void processNode(aiNode *node, const aiScene *scene);
//         Mesh processMesh(aiMesh *mesh, const aiScene *scene);
//         std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
// };