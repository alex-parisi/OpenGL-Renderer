///////////////
// scene.hpp //
///////////////

#pragma once

#include "object.hpp"
#include "shader.hpp"

#include <vector>

class Scene
{
	public:
		// Constructor & Destructor:
		Scene();
		~Scene();
		// External functions:
		void Render();
		void AddObject(Object& object);
		// Get/Set:
		void SetLightingShader(Shader& lightingShader);
		void SetShadowShader(Shader& shadowShader);
	private:
		Shader* m_lightingShader;
		Shader* m_shadowShader;
		std::vector<Object*> m_objects;
};