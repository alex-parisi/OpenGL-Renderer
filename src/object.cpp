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
    m_N = NULL;
    m_name = "";
    m_texScaling = 1.0f;
    m_texture = NULL;
}

Object::Object(std::string name, const float* vertices, int N)
{
    // Initialize model and object buffers:
    m_model = glm::mat4(1.0f);
    m_N = N;
    m_name = name;
    m_texScaling = 1.0f;
    m_VAO = NULL;
    m_VBO = NULL;
    m_texture = NULL;
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

Object::Object(std::string name, const float* vertices, int N, unsigned int texture)
{
    // Initialize model and object buffers:
    m_model = glm::mat4(1.0f);
    m_N = N;
    m_name = name;
    m_texScaling = 1.0f;
    m_VAO = NULL;
    m_VBO = NULL;
    m_texture = texture;
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
    shader.SetMat4("model", m_model);
    shader.SetFloat("texScaling", m_texScaling);
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    // Render the object:
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_N / 8);
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

std::string Object::GetName()
{
    return m_name;
}

void Object::SetName(std::string name)
{
    m_name = name;
}

void Object::SetTexScaling(float texScaling)
{
    m_texScaling = texScaling;
}

void Object::SetTexture(unsigned int texture)
{
    m_texture = texture;
}

unsigned int Object::GetTexture()
{
    return m_texture;
}