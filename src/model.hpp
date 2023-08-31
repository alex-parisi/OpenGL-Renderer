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
#include "camera.hpp"
#include "light.hpp"
#include "directional_light.hpp"
#include "input.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
    public:
        // Constructor & Destructor
        Model(Shader& shader, std::string const& path, bool gamma = false) : gammaCorrection(gamma)
        {
            m_shader = &shader;
            LoadModel(path);
            m_model = glm::mat4(1.0f);
        }
        void Draw(Camera& camera, DirectionalLight* directionalLight, std::vector<Light*> pointLights, InputManager* inputManager);
        // Public Attributes
        std::vector<MeshTexture> texturesLoaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;

    private:
        void LoadModel(std::string const& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        Shader* m_shader;
        glm::mat4 m_model;
};