#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures, float shininess)
{
    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;
    m_shininess = shininess;
    SetupMesh();
}

Mesh::~Mesh()
{

}

void Mesh::Draw(Shader& shader, DirectionalLight& directionalLight, std::vector<PointLight*> pointLights)
{
    // Bind material properties
    shader.Use();
    shader.SetBool("blinn", shader.blinn);
    shader.SetFloat("texScaling", 1.0f);
    shader.SetFloat("material.shininess", m_shininess / 4);
    // shader.SetFloat("material.shininess", m_shininess / 4000.0f);
    // Set Light Properties:
    // Directional light:
    shader.SetVec3("dirLight.direction", directionalLight.GetDirection());
    shader.SetVec3("dirLight.ambient", directionalLight.GetAmbient());
    shader.SetVec3("dirLight.diffuse", directionalLight.GetDiffuse());
    shader.SetVec3("dirLight.specular", directionalLight.GetSpecular());
    // Point light(s):
    shader.SetInt("numLights", static_cast<int>(pointLights.size()));
    for (auto& p : pointLights)
    {
        shader.SetVec3("pointLights[0].position", p->GetPosition());
        shader.SetVec3("pointLights[0].ambient", p->GetAmbient());
        shader.SetVec3("pointLights[0].diffuse", p->GetDiffuse());
        shader.SetVec3("pointLights[0].specular", p->GetSpecular());
        shader.SetFloat("pointLights[0].constant", p->GetConstant());
        shader.SetFloat("pointLights[0].linear", p->GetLinear());
        shader.SetFloat("pointLights[0].quadratic", p->GetQuadratic());
    }
    // Bind textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "material.diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "material.specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "material.normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string
        // Now, set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // Set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawDepth(Shader& shader)
{
    // Bind material properties
    shader.Use();
    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind vertex array
    glBindVertexArray(VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    // Set the Vertex Attribute Pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // Vertex Texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // Vertex Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // Vertex Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // Ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    // Weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}