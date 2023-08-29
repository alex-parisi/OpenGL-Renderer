#include "scene.hpp"

Scene::Scene()
{
    objects.clear();
    lights.clear();
}

Scene::~Scene()
{

}

void Scene::Render(float deltaTime, Camera& camera)
{
    // Set the background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render each object in the scene:
    for (auto& o : objects)
    {
        o.Render(deltaTime, camera);
    }
    // Also render the lights:
    for (auto& l : lights)
    {
        l.Render(deltaTime, camera);
    }
}

void Scene::AddObject(Object& object)
{
    objects.push_back(object);
}

void Scene::AddLight(Light& light)
{
    lights.push_back(light);
}