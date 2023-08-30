#include "src/engine.hpp"

int main(int argc, char* argv[])
{
	Engine engine;
    if (engine.Initialize())
    {
        // <TEST>
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        // First, create a shader for the object (this is actually for lighting)
        Shader testShader;
        testShader.Initialize("../src/shaders/colors.vs", "../src/shaders/colors.fs");
        testShader.SetInt("material.diffuse", 0);
        testShader.SetInt("material.specular", 1);

        // Also create a shader for the light (this is actually a small shader used to draw the light cube, not the lighting)
        Shader lightShader;
        lightShader.Initialize("../src/shaders/light_cube.vs", "../src/shaders/light_cube.fs");
        
        // Then, create a texture for the object
        Texture testTexture;
        testTexture.LoadRGB("../resources/textures/wood.jpg");

        // Next, create a material for the object
        Material testMaterial;
        testMaterial.SetShader(testShader);
        testMaterial.SetAmbient(glm::vec3(1.0f, 0.5f, 0.31f));
        testMaterial.SetDiffuse(glm::vec3(1.0f, 0.5f, 0.31f));
        testMaterial.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        testMaterial.SetShininess(32.0f);
        testMaterial.SetDiffuseTexture(testTexture);
        testMaterial.SetSpecularTexture(testTexture);

        // Finally, create the object
        Object testObject(testMaterial, vertices, 288, false);

        // And create a point light
        Light testLight1(lightShader, vertices, 288);
        testLight1.SetPosition(glm::vec3(1.2f, 1.0f, 2.0f));
        testLight1.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        testLight1.SetDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        testLight1.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        testLight1.SetConstant(1.0f);
        testLight1.SetLinear(0.09f);
        testLight1.SetQuadratic(0.032f);

        // And create a directional light
        DirectionalLight testLight2;
        testLight2.SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
        testLight2.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        testLight2.SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
        testLight2.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));

        // Finally, add the object and lights to the scene
        engine.scene.AddObject(testObject);
        engine.scene.AddLight(testLight1);
        engine.scene.SetDirectionalLight(testLight2);
        // </TEST>
        engine.Execute();
    }
	return 0;
}