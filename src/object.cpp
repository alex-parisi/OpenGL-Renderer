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
    // As we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(m_material->GetShader()->ID);
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
void Object::Render(float deltaTime, Camera* camera, DirectionalLight* directionalLight, std::vector<Light *> pointLights)
{
    // Activate the shader
    m_material->GetShader()->Use();
    m_material->GetShader()->SetVec3("viewPos", camera->GetCameraPos());
    m_material->GetShader()->SetBool("blinn", m_material->GetShader()->blinn);
    m_material->GetShader()->SetFloat("texScaling", texScaling);
    // Set Shininess:
    m_material->GetShader()->SetFloat("material.shininess", m_material->GetShininess());
    // Set Light Properties:
    // Start with directional light:
    m_material->GetShader()->SetVec3("dirLight.direction", directionalLight->GetDirection());
    m_material->GetShader()->SetVec3("dirLight.ambient", directionalLight->GetAmbient());
    m_material->GetShader()->SetVec3("dirLight.diffuse", directionalLight->GetDiffuse());
    m_material->GetShader()->SetVec3("dirLight.specular", directionalLight->GetSpecular());
    // Then do all of the point lights: (right now limited to one)
    for (auto& p : pointLights)
    {
        m_material->GetShader()->SetVec3("pointLights[0].position", p->GetPosition());
        m_material->GetShader()->SetVec3("pointLights[0].ambient", p->GetAmbient());
        m_material->GetShader()->SetVec3("pointLights[0].diffuse", p->GetDiffuse());
        m_material->GetShader()->SetVec3("pointLights[0].specular", p->GetSpecular());
        m_material->GetShader()->SetFloat("pointLights[0].constant", p->GetConstant());
        m_material->GetShader()->SetFloat("pointLights[0].linear", p->GetLinear());
        m_material->GetShader()->SetFloat("pointLights[0].quadratic", p->GetQuadratic());
    }
    // Pass the projection matrix to the shader
    glm::mat4 projection = camera->GetProjectionMatrix();
    m_material->GetShader()->SetMat4("projection", projection);
    // Camera/View Transformation
    glm::mat4 view = camera->GetViewMatrix();
    m_material->GetShader()->SetMat4("view", view);
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
    m_material->GetShader()->SetMat4("model", model);
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