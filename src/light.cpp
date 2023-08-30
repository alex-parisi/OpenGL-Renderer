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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // As we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shader.ID);
    m_shader = &shader;
    model = glm::mat4(1.0f);
    draw = true;
}

Light::~Light()
{

}

void Light::Render(float deltaTime, Camera& camera)
{
    if (draw)
    {
        m_shader->Use();
        m_shader->SetMat4("projection", camera.GetProjectionMatrix());
        m_shader->SetMat4("view", camera.GetViewMatrix());
        model = glm::mat4(1.0f);
        model = glm::rotate(model, deltaTime / 5, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::translate(model, position);
        position = glm::vec3(model[3]);
        model = glm::scale(model, glm::vec3(0.1f));
        m_shader->SetMat4("model", model);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

glm::vec3 Light::GetPosition()
{
	return position;
}

void Light::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

void Light::SetDraw(bool newDraw)
{
    draw = newDraw;
}

glm::vec3 Light::GetAmbient()
{
    return ambient;
}

glm::vec3 Light::GetDiffuse()
{
    return diffuse;
}

glm::vec3 Light::GetSpecular()
{
    return specular;
}

float Light::GetConstant()
{
    return constant;
}

float Light::GetLinear()
{
    return linear;
}

float Light::GetQuadratic()
{
    return quadratic;
}

void Light::SetAmbient(glm::vec3 newAmbient)
{
    ambient = newAmbient;
}

void Light::SetDiffuse(glm::vec3 newDiffuse)
{
    diffuse = newDiffuse;
}

void Light::SetSpecular(glm::vec3 newSpecular)
{
    specular = newSpecular;
}

void Light::SetConstant(float newConstant)
{
    constant = newConstant;
}

void Light::SetLinear(float newLinear)
{
    linear = newLinear;
}

void Light::SetQuadratic(float newQuadratic)
{
    quadratic = newQuadratic;
}