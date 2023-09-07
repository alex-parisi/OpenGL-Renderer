//////////////
// main.cpp //
//////////////

#include "src/engine.hpp"
#include "src/vertices.hpp"

#define PRINT_INPUTS 0

int main(int argc, char* argv[])
{
	Engine engine;
    if (engine.Initialize())
    {
        // 1. Shaders:
        // Load the shaders:
        Shader lightingShader("../src/shaders/lighting.vs", "../src/shaders/lighting.fs");
        Shader shadowShader("../src/shaders/shadow.vs", "../src/shaders/shadow.fs");
        // Set the shaders:
        engine.SetLightingShader(lightingShader);
        engine.SetShadowShader(shadowShader);

        // 2. Objects:
        // Load the floor object:
        Floor floor(planeVertices, 48);
        // Load the cube object:
        Object cube(cubeVertices, 288);
        // Set the objects:
        engine.AddObjectToScene(floor);
        engine.AddObjectToScene(cube);

        // Begin Engine:
        engine.Execute();
    }
	return 0;
}