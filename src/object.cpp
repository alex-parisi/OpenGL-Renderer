#include "object.hpp"

Object::Object(Material& material, float* vertices, int N, bool wireframeOn)
{
    // Initialize Vertex Array and Vertex Buffer Objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffers.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * N, vertices, GL_STATIC_DRAW);
    // Then configure vertex attributes(s).
    // Position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal Attribute:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture Attribute:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // As we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(material.GetShader()->ID);
    // Set the wireframe mode to be on or off
    if (wireframeOn)
    {
        wireframeMode = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {  
        wireframeMode = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // Initialize the object model
    model = glm::mat4(1.0f);
    // Set the material
    m_material = material;
}

Object::~Object()
{

}

void Object::Translate(glm::vec3 newPos)
{
    model = glm::translate(model, newPos);
}

void Object::Rotate(float angle, glm::vec3 axis)
{
    model = glm::rotate(model, glm::radians(angle), axis);
}

void Object::Scale(glm::vec3 scale)
{
    model = glm::scale(model, scale);
}

// Inherited Render function:
void Object::Render(float deltaTime, Camera& camera)
{
    // Activate the shader
    m_material.GetShader()->Use();
    // Set Light Properties:
    m_material.GetShader()->SetVec3("viewPos", camera.GetCameraPos());
    m_material.GetShader()->SetVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
    m_material.GetShader()->SetVec3("light.ambient", ambientColor);
    m_material.GetShader()->SetVec3("light.diffuse", diffuseColor);
    m_material.GetShader()->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
    // Set Material Properties
    m_material.GetShader()->SetVec3("material.ambient", m_material.GetAmbient());
    m_material.GetShader()->SetVec3("material.diffuse", m_material.GetDiffuse());
    m_material.GetShader()->SetVec3("material.specular", m_material.GetSpecular());
    m_material.GetShader()->SetFloat("material.shininess", m_material.GetShininess());
    // Pass the projection matrix to the shader
    glm::mat4 projection = camera.GetProjectionMatrix();
    m_material.GetShader()->SetMat4("projection", projection);
    // Camera/View Transformation
    glm::mat4 view = camera.GetViewMatrix();
    m_material.GetShader()->SetMat4("view", view);
    // Do time transformations here:
    // <TEMP>
    // model = glm::mat4(1.0f);
    // float t = static_cast<float>(glfwGetTime());
    // Rotate(t * 100, glm::vec3(0.5f, 1.0f, 0.0f));
    // Scale(glm::vec3((0.1 * sin(5 * t)) + 0.9));
    // </TEMP>
    // Bind material's texture onto texture unit (if there is a texture)
    if (m_material.GetTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_material.GetTexture()->texture);
    }
    // Render the object:
    m_material.GetShader()->SetMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::vec3 Object::GetPosition()
{
    return position;
}

void Object::SetWireframeMode(bool newWireframeMode)
{
    wireframeMode = newWireframeMode;
}

bool Object::GetWireframeMode()
{
    return wireframeMode;
}