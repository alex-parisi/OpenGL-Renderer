#include "src/engine.hpp"

int main(int argc, char* argv[])
{
	Engine engine;
    if (engine.Initialize())
    {
        // <TEST>
        float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        };
        // First, create a shader for the object
        Shader testShader;
        testShader.Initialize("../src/shaders/colors.vs", "../src/shaders/colors.fs");
        // Also create a shader for the light
        Shader lightShader;
        lightShader.Initialize("../src/shaders/light_cube.vs", "../src/shaders/light_cube.fs");
        // Then, create a texture for the object
        // Texture testTexture;
        // testTexture.LoadRGB("../resources/textures/test2.jpg");
        // Next, create a material for the object
        Material testMaterial;
        testMaterial.SetShader(testShader);
        // testMaterial.SetTexture(testTexture);
        // Finally, create the object
        Object testObject(testMaterial, vertices, 108, false);
        // And create the light
        Light testLight(lightShader, vertices, 108);
        testLight.SetPosition(glm::vec3(1.2f, 1.0f, 2.0f));
        // And add the object and light to the scene
        engine.scene.AddObject(testObject);
        engine.scene.AddLight(testLight);
        // </TEST>
        engine.Execute();
    }
	return 0;
}