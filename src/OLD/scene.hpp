#pragma once
#include <vector>
#include "object.hpp"
#include "camera.hpp"
#include "point_light.hpp"
#include "directional_light.hpp"
#include "model.hpp"
#include "input.hpp"
#include "settings.hpp"
#include "skybox.hpp"

class Scene
{
    public:
        // Constructor & Destructor:
        Scene();
        ~Scene();
        // Public Functions:
        void Render(float deltaTime, Camera* camera, InputManager* inputManager);
        // Add object to scene:
        void AddObject(Object& object);
        void AddLight(PointLight& pointLight);
        void SetDirectionalLight(DirectionalLight& newDirectionalLight);
        // Add model to the scene:
        void AddModel(Model& model);
        // Set the shaders
        void SetLightShader(Shader& newLightShader);
        void SetShadowshader(Shader& newShaderShader);
        void SetLightbulbShader(Shader& newLightbulbShader);
        void SetSkyboxShader(Shader& newSkyboxShader);
        // Skybox:
        Skybox skybox;
        glm::mat4 lightSpaceMatrix;

    private:
        void RenderScene(float deltaTime, Camera* camera, InputManager* inputManager);
        void RenderDepthOfScene(float deltaTime, Camera* camera, InputManager* inputManager);
        // List of objects in the scene:
        std::vector<Object *> objects;
        // List of models in the scene:
        std::vector<Model *> models;
        // Lighting: can have unlimited point sources, but only one directional source
        std::vector<PointLight*> pointLights;
        DirectionalLight* directionalLight;
        // Shaders used in the rendering loop:
        Shader* lightShader;
        Shader* shadowShader;
        Shader* lightbulbShader;
        Shader* skyboxShader;
        
};