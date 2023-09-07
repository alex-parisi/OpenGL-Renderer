///////////////
// scene.cpp //
///////////////

#include "scene.hpp"

// Constructor:
Scene::Scene()
{
	m_lightingShader = nullptr;
	m_shadowShader = nullptr;
    m_depthMap = NULL;
    m_depthMapFBO = NULL;
    woodTexture = NULL;
}
// Destructor:
Scene::~Scene()
{

}

// External functions:
void Scene::Render(Camera& camera, InputManager& inputManager)
{
    // TEMP:
    // Rotate the cube object if the arrow keys are pressed:
    for (auto& o : m_objects)
    {
        if (o->GetName() == "cube")
        {
            if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_UP))
                o->SetModel(glm::rotate(o->GetModel(), glm::radians(0.1f), glm::vec3(-1.0f, 0.0f, 0.0f)));
            if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_DOWN))
                o->SetModel(glm::rotate(o->GetModel(), glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f)));
            if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_LEFT))
                o->SetModel(glm::rotate(o->GetModel(), glm::radians(0.1f), glm::vec3(0.0f, -1.0f, 0.0f)));
            if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_RIGHT))
                o->SetModel(glm::rotate(o->GetModel(), glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f)));
        }
    }

	// 1. Render Depth of Scene:
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;

    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(m_directionalLight.GetPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // Render scene from light's point of view
    m_shadowShader->Use();
    m_shadowShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    RenderScene(*m_shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Render Scene:
    m_lightingShader->Use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    m_lightingShader->SetMat4("projection", projection);
    m_lightingShader->SetMat4("view", view);
    // set light uniforms
    m_lightingShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    m_directionalLight.SetUniforms(*m_lightingShader);   
    m_lightingShader->SetVec3("viewPos", camera.GetCameraPos());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    RenderScene(*m_lightingShader);
}

void Scene::AddObject(Object& object)
{
	m_objects.push_back(&object);
}

void Scene::ConfigureDepthMap()
{
    glGenFramebuffers(1, &m_depthMapFBO);
    // Create depth texture
    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::ConfigureShaders()
{
    // Lighting:
    m_lightingShader->Use();
    m_lightingShader->SetInt("diffuseTexture", 0);
    m_lightingShader->SetInt("shadowMap", 1);
    // Shadow:
    m_shadowShader->Use();
    m_shadowShader->SetInt("depthMap", 0);
}

// Get/Set:
void Scene::SetLightingShader(Shader& lightingShader)
{
	m_lightingShader = &lightingShader;
}

Shader* Scene::GetLightingShader()
{
    return m_lightingShader;
}

void Scene::SetShadowShader(Shader& shadowShader)
{
	m_shadowShader = &shadowShader;
}

Shader* Scene::GetShadowShader()
{
    return m_shadowShader;
}

void Scene::SetPointShadowShader(Shader& pointShadowShader)
{
    m_pointShadowShader = &pointShadowShader;
}

Shader* Scene::GetPointShadowShader()
{
    return m_pointShadowShader;
}

DirectionalLight* Scene::GetDirectionalLight()
{
    return &m_directionalLight;
}

// Internal functions:
void Scene::RenderScene(Shader& shader)
{
	for (auto& o : m_objects)
	{
		o->Render(shader);
	}
}





unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}