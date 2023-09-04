#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct MeshTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        // Constructor & Destructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures, float shininess);
        ~Mesh();
        // Public Functions:
        void Draw(Shader& shader, DirectionalLight& directionalLight, std::vector<PointLight*> pointLights);
        // Public Attributes:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<MeshTexture> m_textures;
        float m_shininess;
        unsigned int VAO;

    private:
        // Private Functions:
        void SetupMesh();
        // Private Attributes:
        unsigned int VBO, EBO;
};