#include "light.hpp"

Light::Light(Shader& shader, float* vertices, int N)
{
    // Initialize Vertex Array and Vertex Buffer Objects
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffers.
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * N, vertices, GL_STATIC_DRAW);
    // Then configure vertex attributes(s).
    // Position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // As we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shader.ID);
    m_shader = &shader;
    model = glm::mat4(1.0f);
}

Light::~Light()
{

}

void Light::Render(float deltaTime, Camera& camera)
{
    m_shader->Use();
    m_shader->SetMat4("projection", camera.GetProjectionMatrix());
    m_shader->SetMat4("view", camera.GetViewMatrix());
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.2f));
    m_shader->SetMat4("model", model);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::vec3 Light::GetPosition()
{
	return position;
}

void Light::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
}