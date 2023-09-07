////////////////
// object.cpp //
////////////////

#include "object.hpp"

// Constructor:
Object::Object()
{
    m_model = glm::mat4(1.0f);
    m_VAO = NULL;
    m_VBO = NULL;
}

Object::Object(const float* vertices, int N)
{
    // Initialize model and object buffers:
    m_model = glm::mat4(1.0f);
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    // Fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * N, vertices, GL_STATIC_DRAW);
    // Link vertex attributes
    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Destructor:
Object::~Object()
{

}

// External functions:
void Object::Render(Shader& shader)
{
    // Set the shader settings:
    shader.Use();
    shader.SetMat4("model", m_model);
    // Render the object:
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// Get/Set:
glm::mat4 Object::GetModel()
{
	return m_model;
}

void Object::SetModel(glm::mat4 model)
{
	m_model = model;
}

unsigned int Object::GetVAO()
{
    return m_VAO;
}

unsigned int Object::GetVBO()
{
    return m_VBO;
}

// Floor-type object
void Floor::Render(Shader& shader)
{
    // Set the shader settings:
    shader.Use();
    shader.SetMat4("model", GetModel());
    // Render the object:
    glBindVertexArray(GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}