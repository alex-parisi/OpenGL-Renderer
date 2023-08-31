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

void Scene::Render(float deltaTime, Camera* camera)
{
    // Set the background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render each object in the scene:
    for (auto& o : objects)
    {
        o->Render(deltaTime, camera, directionalLight, pointLights);
    }
    // Render each model in the scene:
    for (auto& m : models)
    {
        m->Draw(*camera, directionalLight, pointLights);
    }
    // Also render the point lights:
    for (auto& l : pointLights)
    {
        l->Render(deltaTime, camera);
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