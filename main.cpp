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
        // Set the shaders:
        engine.SetLightingShader(lightingShader);
        engine.SetShadowShader(shadowShader);
        engine.SetPointShadowShader(pointShadowShader);

        // 2. Objects:
        // Load the floor object:
        Object floor("floor", planeVertices, 48);
        // Load the cube object:
        Object cube("cube", cubeVertices, 288);
        cube.SetModel(glm::translate(cube.GetModel(), glm::vec3(0.0f, 3.0f, 0.0f)));
        // Set the objects:
        engine.AddObjectToScene(floor);
        engine.AddObjectToScene(cube);

        // Begin Engine:
        engine.Configure();
        engine.Execute();
    }
	return 0;
}