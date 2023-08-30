#pragma once
#include <vector>
#include "object.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "directional_light.hpp"

class Scene
{
    public:
        // Constructor & Destructor:
        Scene();
        ~Scene();
        // Public Functions:
        void Render(float deltaTime, Camera &camera);
        // Add object to scene:
        void AddObject(Object &object);
        void AddLight(Light& pointLight);
        void SetDirectionalLight(DirectionalLight& newDirectionalLight);

    private:
        // List of objects in the scene:
        std::vector<Object> objects;
        // Lighting: can have unlimited point sources, but only one directional source
        std::vector<Light> pointLights;
        DirectionalLight directionalLight;

};