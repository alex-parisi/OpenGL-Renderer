#pragma once
#include <vector>
#include "object.hpp"
#include "camera.hpp"
#include "light.hpp"

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
        void AddLight(Light& light);

    private:
        // List of objects in the scene:
        std::vector<Object> objects;
        std::vector<Light> lights;
};