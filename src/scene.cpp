#include "scene.hpp"

Scene::Scene()
{
    objects.clear();
    models.clear();
    pointLights.clear();
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

void Scene::AddLight(PointLight& pointLight)
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

}