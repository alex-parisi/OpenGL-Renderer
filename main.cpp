//////////////
// main.cpp //
//////////////

#include "src/engine.hpp"
#include "src/vertices.hpp"

int main(int argc, char* argv[])
{
    Engine engine;
    if (engine.Initialize())
    {
        // 1. Shaders:
        // Load the shaders:
        Shader lightingShader("../src/shaders/lighting.vs", "../src/shaders/lighting.fs");
        Shader shadowShader("../src/shaders/shadow.vs", "../src/shaders/shadow.fs");
        Shader pointShadowShader("../src/shaders/point_shadow.vs", "../src/shaders/point_shadow.fs", "../src/shaders/point_shadow.gs");
        Shader lightbulbShader("../src/shaders/light_cube.vs", "../src/shaders/light_cube.fs");
        Shader skyboxShader("../src/shaders/skybox.vs", "../src/shaders/skybox.fs");
        // Set the shaders:
        engine.SetLightingShader(lightingShader);
        engine.SetShadowShader(shadowShader);
        engine.SetPointShadowShader(pointShadowShader);
        engine.SetLightbulbShader(lightbulbShader);
        engine.SetSkyboxShader(skyboxShader);

        // 2. Objects:
        // Load the floor object:
        unsigned int floorTexture = loadTexture("../resources/textures/wood.jpg");
        Object floor("floor", planeVertices, 48, floorTexture);
        floor.SetTexScaling(10.0f);
        // Load the cube object:
        unsigned int cubeTexture = loadTexture("../resources/textures/wood_floor.png");
        Object cube("cube", cubeVertices, 288, cubeTexture);
        cube.SetModel(glm::translate(cube.GetModel(), glm::vec3(0.0f, 3.0f, 0.0f)));
        // Set the objects:
        engine.AddObjectToScene(floor);
        // engine.AddObjectToScene(cube);

        // 3. Models:
        // Load the demo object:
        Model testModel("../resources/models/backpack.obj", "testModel");
        testModel.SetModel(glm::translate(testModel.GetModel(), glm::vec3(0.0f, 3.0f, 0.0f)));
        engine.AddModelToScene(testModel);

        // 4. Point light(s):
        PointLight pointLight0(0, 6);
        pointLight0.SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
        engine.AddPointLightToScene(pointLight0);
        // PointLight pointLight1(1, 5);
        // pointLight1.SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
        // engine.AddPointLightToScene(pointLight1);

        // 5. Skybox:
        std::vector<std::string> facePaths
        {
            "../resources/skybox/right.jpg",
            "../resources/skybox/left.jpg",
            "../resources/skybox/top.jpg",
            "../resources/skybox/bottom.jpg",
            "../resources/skybox/front.jpg",
            "../resources/skybox/back.jpg"
        };
        Skybox skybox;
        skybox.Load(facePaths, skyboxVertices, 108);
        // engine.SetSkybox(skybox);

        // Optional: turn off directional light source:
        engine.GetScene()->GetDirectionalLight()->TurnOff();

        // Begin Engine:
        engine.Configure();
        engine.Execute();
    }
    return 0;
}