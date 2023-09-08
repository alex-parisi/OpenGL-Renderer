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
        // Set the shaders:
        engine.SetLightingShader(lightingShader);
        engine.SetShadowShader(shadowShader);
        engine.SetPointShadowShader(pointShadowShader);
        engine.SetLightbulbShader(lightbulbShader);

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
        engine.AddObjectToScene(cube);

        // 3. Point light(s):
        PointLight pointLight(0, 7);
        pointLight.SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
        engine.AddPointLightToScene(pointLight);

        // engine.GetScene()->GetDirectionalLight()->TurnOff();

        // Begin Engine:
        engine.Configure();
        engine.Execute();
    }
    return 0;
}