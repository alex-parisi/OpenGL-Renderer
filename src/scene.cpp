///////////////
// scene.cpp //
///////////////

#include "scene.hpp"

// Constructor:
Scene::Scene()
{
	m_lightingShader = nullptr;
	m_shadowShader = nullptr;
    m_lightbulbShader = nullptr;
    m_pointShadowShader = nullptr;
    m_skyboxShader = nullptr;
    m_hdrShader = nullptr;
    m_skybox = nullptr;
    m_depthMap = NULL;
    m_depthMapFBO = NULL;
    m_cubeMap = NULL;
    m_cubeMapFBO = NULL;
    m_heightScale = 0.1f;
    m_hdrFBO = NULL;
    m_rboDepth = NULL;
    m_quadVAO = NULL;
    m_quadVBO = NULL;
    m_exposure = 1.0f;
}
// Destructor:
Scene::~Scene()
{

}

// External functions:
void Scene::Render(Camera& camera, InputManager& inputManager)
{
    // +TEMP
    // Rotate the model object if the arrow keys are pressed:
    for (auto& m : m_models)
    {
        // if (m->GetName() == "testModel")
        if (1)
        {
            // if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_UP))
            if (!inputManager.m_keyboard.GetKeyState(GLFW_KEY_SPACE))
                m->SetModel(glm::rotate(m->GetModel(), glm::radians(0.1f), glm::vec3(-1.0f, 0.0f, 0.0f)));
            if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_DOWN))
                m->SetModel(glm::rotate(m->GetModel(), glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f)));
            // if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_LEFT))
            if (!inputManager.m_keyboard.GetKeyState(GLFW_KEY_SPACE))
                m->SetModel(glm::rotate(m->GetModel(), glm::radians(0.1f), glm::vec3(0.0f, -1.0f, 0.0f)));
            if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_RIGHT))
                m->SetModel(glm::rotate(m->GetModel(), glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f)));
        }
    }
    // -TEMP

    // 1. For each point light in the scene, perform the point shadow depth mapping:
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < static_cast<int>(m_pointLights.size()); i++)
    {
        glm::vec3 lightPos = m_pointLights[i]->GetPosition();
        // 1a. Create depth cubemap transformation matrices
        float near_plane = 1.0f;
        float far_plane = 50.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        // 1b. Render scene to depth cubemap
        m_pointShadowShader->Use();
        for (unsigned int i = 0; i < 6; ++i)
            m_pointShadowShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        m_pointShadowShader->SetFloat("far_plane", far_plane);
        m_pointShadowShader->SetVec3("lightPos", lightPos);
        RenderScene(*m_pointShadowShader, false, false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Render Depth of Scene (directional light):
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
    RenderScene(*m_shadowShader, false, false);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Reset viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3. Render Final Scene:
    // Render Scene into the floating point buffer first:
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_lightingShader->Use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    m_lightingShader->SetMat4("projection", projection);
    m_lightingShader->SetMat4("view", view);
    // Set number of point lights to use in render loop:
    m_lightingShader->SetInt("n_pointLights", static_cast<int>(m_pointLights.size()));
    // Set light uniforms
    m_lightingShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    m_directionalLight.SetUniforms(*m_lightingShader);   
    m_lightingShader->SetVec3("viewPos", camera.GetCameraPos());
    m_lightingShader->SetFloat("far_plane", 50.0f);
    m_lightingShader->SetFloat("heightScale", m_heightScale);
    for (int i = 0; i < static_cast<int>(m_pointLights.size()); i++)
    {
        m_pointLights[i]->SetUniforms(*m_lightingShader, i);
    }
    // Bind the depth texture(s)
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    // + TEMP
    bool turnOnNormalMap = false;
    bool turnOnHeightMap = false;
    int turnOnBloom = 0;
    if (!inputManager.m_keyboard.GetKeyState(GLFW_KEY_1))
        turnOnNormalMap = true;
    if (!inputManager.m_keyboard.GetKeyState(GLFW_KEY_2))
        turnOnHeightMap = true;
    if (!inputManager.m_keyboard.GetKeyState(GLFW_KEY_3))
        turnOnBloom = 1;
    // - TEMP
    RenderScene(*m_lightingShader, turnOnNormalMap, turnOnHeightMap);
    // 4. Draw the lighbulbs
    DrawLightbulbs(camera, inputManager);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Blur bright fragments:
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    m_blurShader->Use();
    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);
        m_blurShader->SetInt("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? m_colorBuffers[1] : m_pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
        RenderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    // --------------------------------------------------------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_hdrShader->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_pingpongColorbuffers[!horizontal]);
    m_hdrShader->SetInt("bloom", turnOnBloom);
    m_hdrShader->SetFloat("exposure", m_exposure);
    RenderQuad();

    // 5. Render the skybox:
    if (m_skybox != nullptr)
        if (m_skybox->loaded)
            m_skybox->Render(*m_skyboxShader, camera);
}

void Scene::AddObject(Object& object)
{
    // Add the object to the list
    // Object instancing takes care of creating all buffers
	m_objects.push_back(&object);
}

void Scene::AddModel(Model& model)
{
    m_models.push_back(&model);
}

void Scene::AddPointLight(PointLight& pointLight)
{
    // Add the point light to the list
    m_pointLights.push_back(&pointLight);
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

void Scene::ConfigureCubeMap()
{
    glGenFramebuffers(1, &m_cubeMapFBO);
    // Create depth cubemap texture
    glGenTextures(1, &m_cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_cubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::ConfigureShaders()
{
    // Lighting:
    m_lightingShader->Use();
    m_lightingShader->SetInt("diffuseTexture", 0);
    m_lightingShader->SetInt("specularTexture", 1);
    m_lightingShader->SetInt("normalTexture", 2);
    m_lightingShader->SetInt("heightTexture", 3);
    m_lightingShader->SetInt("shadowMap", 4);
    m_lightingShader->SetInt("pointShadowMap", 5);
    // Shadow:
    m_shadowShader->Use();
    m_shadowShader->SetInt("depthMap", 0);
    // Blur:
    m_blurShader->Use();
    m_blurShader->SetInt("image", 0);
    // HDR:
    m_hdrShader->Use();
    m_hdrShader->SetInt("scene", 0);
    m_hdrShader->SetInt("bloomBlur", 1);
}

void Scene::ConfigureHDR()
{
    glGenFramebuffers(1, &m_hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
    glGenTextures(2, m_colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i], 0);
    }
    // create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &m_rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::ConfigureBlur()
{
    glGenFramebuffers(2, m_pingpongFBO);
    glGenTextures(2, m_pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, m_pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongColorbuffers[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
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

void Scene::SetLightbulbShader(Shader& lightbulbShader)
{
    m_lightbulbShader = &lightbulbShader;
}

Shader* Scene::GetLightbulbShader()
{
    return m_lightbulbShader;
}

DirectionalLight* Scene::GetDirectionalLight()
{
    return &m_directionalLight;
}

Skybox* Scene::GetSkybox()
{
    return m_skybox;
}

void Scene::SetSkybox(Skybox& skybox)
{
    m_skybox = &skybox;
}

Shader* Scene::GetSkyboxShader()
{
    return m_skyboxShader;
}

void Scene::SetSkyboxShader(Shader& skyboxShader)
{
    m_skyboxShader = &skyboxShader;
}

float Scene::GetHeightScale()
{
    return m_heightScale;
}

void Scene::SetHeightScale(float heightScale)
{
    m_heightScale = heightScale;
}

Shader* Scene::GetHDRShader()
{
    return m_hdrShader;
}

void Scene::SetHDRShader(Shader& hdrShader)
{
    m_hdrShader = &hdrShader;
}

float Scene::GetExposure()
{
    return m_exposure;
}

void Scene::SetExposure(float exposure)
{
    m_exposure = exposure;
}

Shader* Scene::GetBlurShader()
{
    return m_blurShader;
}

void Scene::SetBlurShader(Shader& blurShader)
{
    m_blurShader = &blurShader;
}

// Internal functions:
void Scene::RenderScene(Shader& shader, bool useNormalMap, bool useHeightMap)
{
    // Render all objects:
	for (auto& o : m_objects)
	{
		o->Render(shader);
	}
    // Render all models:
    for (auto& m : m_models)
    {
        m->Draw(shader, useNormalMap, useHeightMap);
    }
}

void Scene::DrawLightbulbs(Camera& camera, InputManager& inputManager)
{
    m_lightbulbShader->Use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    m_lightbulbShader->SetMat4("projection", projection);
    m_lightbulbShader->SetMat4("view", view);
    for (auto& l : m_pointLights)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // if (inputManager.m_keyboard.GetKeyState(GLFW_KEY_SPACE))
        if (!inputManager.m_keyboard.GetKeyState(GLFW_KEY_SPACE))
        {
            model = glm::rotate(model, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, l->GetPosition());
            model = glm::scale(model, glm::vec3(0.1f));
            l->SetPosition(glm::vec3(model[3]));
        }
        else
        {
            model = glm::translate(model, l->GetPosition());
            model = glm::scale(model, glm::vec3(0.1f));
        }
        m_lightbulbShader->SetMat4("model", model);
        glBindVertexArray(l->GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Scene::RenderQuad()
{
    if (m_quadVAO == NULL)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &m_quadVAO);
        glGenBuffers(1, &m_quadVBO);
        glBindVertexArray(m_quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// TO - DO: move this function to where it belongs, not sure where that would be though
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format, internalFormat;
        if (nrComponents == 1)
        {
            format = GL_RED;
            internalFormat = GL_RED;
        }   
        else if (nrComponents == 3)
        {
            format = GL_RGB;
            internalFormat = GL_SRGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
            internalFormat = GL_SRGB_ALPHA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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