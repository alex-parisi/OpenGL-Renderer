#include "object.hpp"

Object::Object(Material& material, float* vertices, int N, bool wireframeOn)
{
    // Initialize the object model
    model = glm::mat4(1.0f);
    texScaling = 1.0f;
    // Set the material
    m_material = &material;
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
void Object::Render(float deltaTime, Camera* camera, Shader& shader, DirectionalLight* directionalLight, std::vector<PointLight*> pointLights)
{
    // Activate the shader
    shader.Use();
    shader.SetVec3("viewPos", camera->GetCameraPos());
    shader.SetBool("blinn", shader.blinn);
    shader.SetFloat("texScaling", texScaling);
    // Set Shininess:
    shader.SetFloat("material.shininess", m_material->GetShininess());
    // Set Light Properties:
    // Start with directional light:
    shader.SetVec3("dirLight.direction", directionalLight->GetDirection());
    shader.SetVec3("dirLight.ambient", directionalLight->GetAmbient());
    shader.SetVec3("dirLight.diffuse", directionalLight->GetDiffuse());
    shader.SetVec3("dirLight.specular", directionalLight->GetSpecular());
    // Then do all of the point lights: (right now limited to one)
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
    // Pass the projection matrix to the shader
    glm::mat4 projection = camera->GetProjectionMatrix();
    shader.SetMat4("projection", projection);
    // Camera/View Transformation
    glm::mat4 view = camera->GetViewMatrix();
    shader.SetMat4("view", view);
    // Do time transformations here:
    // <TEMP>
    // Do moving stuff here
    // Maybe add another function that loops, and each unique object can overload this function (TO - DO)
    // </TEMP>
    // Bind material's texture onto texture unit (if there is a texture)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_material->GetDiffuseTexture()->texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_material->GetSpecularTexture()->texture);
    // Render the object:
    shader.SetMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Object::RenderDepth(Shader& shader)
{
    // Activate the shader
    shader.Use();
    // Render the object:
    shader.SetMat4("model", model);
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

void Object::SetModel(glm::mat4 newModel)
{
    model = newModel;
}

glm::mat4 Object::GetModel()
{
    return model;
}

void Object::SetTexScaling(float newTexScaling)
{
    texScaling = newTexScaling;
}