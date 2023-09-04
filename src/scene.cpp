#include "scene.hpp"

Scene::Scene()
{
    objects.clear();
    models.clear();
    pointLights.clear();
    depthMap = NULL;
    depthMapFBO = NULL;
}

Scene::~Scene()
{

}

void Scene::Render(float deltaTime, Camera* camera, InputManager* inputManager)
{
    // Set the background color
    glClearColor(BKG_COLOR[0], BKG_COLOR[1], BKG_COLOR[2], BKG_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 1. Render Depth of Scene:
    RenderDepthOfScene(deltaTime, camera, inputManager);
    // 2. Render Scene:
    RenderScene(deltaTime, camera, inputManager);
    // 3. Render skybox:
    if (skybox.loaded)
        skybox.Render(*skyboxShader, *camera);
}

void Scene::AddObject(Object& object)
{
    objects.push_back(&object);
}

void Scene::AddLight(Light& pointLight)
{
    pointLights.push_back(&pointLight);
}

void Scene::SetDirectionalLight(DirectionalLight& newDirectionalLight)
{
    directionalLight = &newDirectionalLight;
}

void Scene::AddModel(Model& model)
{
    models.push_back(&model);
}

void Scene::SetLightShader(Shader& newLightShader)
{
    lightShader = &newLightShader;
}

void Scene::SetShadowshader(Shader& newShadowShader)
{
    shadowShader = &newShadowShader;
}

void Scene::SetLightbulbShader(Shader& newLightbulbShader)
{
    lightbulbShader = &newLightbulbShader;
}

void Scene::SetSkyboxShader(Shader& newSkyboxShader)
{
    skyboxShader = &newSkyboxShader;
}

void Scene::RenderScene(float deltaTime, Camera* camera, InputManager* inputManager)
{
    // 1. Render the point lights (if they are moving, this will update their locations):
    for (auto& l : pointLights)
    {
        l->Render(deltaTime, camera, *lightbulbShader);
    }
    // 2. Render each object in the scene (floors and other geometric shapes)
    // Also pass along the input manager to see if we should be transforming the model, not needed yet (TO - DO)
    for (auto& o : objects)
    {
        o->Render(deltaTime, camera, *lightShader, directionalLight, pointLights);
    }
    // 3. Render each model in the scene:
    // Also pass along the input manager to see if we should be transforming the model
    for (auto& m : models)
    {
        m->Draw(*camera, *lightShader, directionalLight, pointLights, inputManager);
    }
}

void Scene::RenderDepthOfScene(float deltaTime, Camera* camera, InputManager* inputManager)
{
    //glm::mat4 lightProjection, lightView;
    //glm::mat4 lightSpaceMatrix;
    //float near_plane = 1.0f, far_plane = 7.5f;
    //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    //lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    //lightSpaceMatrix = lightProjection * lightView;
    //// render scene from light's point of view
    //simpleDepthShader.use();
    //simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    //renderScene(simpleDepthShader);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// reset viewport
    //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void Scene::ConfigureDepthMap()
{
    glGenFramebuffers(1, &depthMapFBO);
    // Create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}