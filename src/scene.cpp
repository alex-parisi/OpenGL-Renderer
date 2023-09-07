///////////////
// scene.cpp //
///////////////

#include "scene.hpp"

// Constructor:
Scene::Scene()
{
	m_lightingShader = nullptr;
	m_shadowShader = nullptr;
}
// Destructor:
Scene::~Scene()
{

}

// External functions:
void Scene::Render()
{

}

void Scene::AddObject(Object& object)
{
	m_objects.push_back(&object);
}

// Get/Set:
void Scene::SetLightingShader(Shader& lightingShader)
{
	m_lightingShader = &lightingShader;
}

void Scene::SetShadowShader(Shader& shadowShader)
{
	m_shadowShader = &shadowShader;
}

// Internal functions: