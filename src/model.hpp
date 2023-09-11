///////////////
// model.hpp //
///////////////

#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool flipSTBI);

class Model
{
    public:
        // Constructor & Destructor:
        Model();
        Model(std::string const& path, std::string name, bool flipSTBI);
        ~Model();
        // External functions:
        void Draw(Shader& shader, bool useNormalMap, bool useHeightMap);
        // Get/Set:
        glm::mat4 GetModel();
        void SetModel(glm::mat4 model);
        std::string GetName();
        void SetUseNormal(bool useNormal);
        int GetNumVertices();
        int GetNumFaces();
        // External attributes:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;

    private:
        // Private functions:
        void LoadModel(std::string const& path, bool flipSTBI);
        void ProcessNode(aiNode* node, const aiScene* scene, bool flipSTBI);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, bool flipSTBI);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, bool flipSTBI);
        glm::mat4 m_model;
        std::string m_name;
        bool m_useNormal;
        int m_totalVertices;
        int m_totalFaces;
};