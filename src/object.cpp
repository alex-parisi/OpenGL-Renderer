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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture Coordinates:
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
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
    // Bind material's texture onto texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_material.GetTexture()->texture);
    // Activate the shader
    m_material.GetShader()->Use();
    // Pass the projection matrix to the shader
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), 800.0f / 600.0f, 0.1f, 100.0f);
    m_material.GetShader()->SetMat4("projection", projection);
    // Camera/View Transformation
    glm::mat4 view = glm::lookAt(camera.GetCameraPos(), camera.GetCameraPos() + camera.GetCameraFront(), camera.GetCameraUp());
    m_material.GetShader()->SetMat4("view", view);
    // Do time transformations here:
    // <TEMP>
    Rotate(deltaTime * 100.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    // </TEMP>
    // Render the object:
    m_material.GetShader()->SetMat4("model", model);
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