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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 1. Render the point lights (if they are moving, this will update their locations):
    for (auto& l : pointLights)
    {
        l->Render(deltaTime, camera);
    }
    // 2. Render each object in the scene (floors and other geometric shapes)
    for (auto& o : objects)
    {
        o->Render(deltaTime, camera, directionalLight, pointLights);
    }
    // 3. Render each model in the scene:
    // Also pass along the input manager to see if we should be transforming the model
    for (auto& m : models)
    {
        m->Draw(*camera, directionalLight, pointLights, inputManager);
    }
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