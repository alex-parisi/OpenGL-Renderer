#include "src/engine.hpp"
#include "src/test_objects.hpp"

int main(int argc, char* argv[])
{
	Engine engine;
    if (engine.Initialize())
    {
        stbi_set_flip_vertically_on_load(true);

        // 1. Configure the shadow buffers:
        engine.scene.ConfigureDepthMap();

        // 2. Main Lighting Shader:
        // Used to render all objects and models in the scene
        Shader lightingShader("../src/shaders/generic_lighting.vs", "../src/shaders/generic_lighting.fs");
        engine.scene.SetLightShader(lightingShader);

        // 3. Shadow Shader:
        // Used to render shadows in the scene
        Shader shadowShader("../src/shaders/shadow_mapping_depth.vs", "../src/shaders/shadow_mapping_depth.fs");
        engine.scene.SetShadowshader(shadowShader);

        // 4. Lightbulb Shader:
        // Used to draw a white cube at the point light source locations
        Shader lightbulbShader("../src/shaders/light_cube.vs", "../src/shaders/light_cube.fs");
        engine.scene.SetLightbulbShader(lightbulbShader);

        // 5. Model(s):
        // 5a. Load an example model
        Model model("../resources/models/backpack.obj");
        // 5b. Translate it up a teensy bit:
        model.SetModel(glm::translate(model.GetModel(), glm::vec3(0.0f, 5.0f, 0.0f)));
        // 5c. Add the model to the scene:
        engine.scene.AddModel(model);

        // 6. Object(s):
        // Perhaps deprecated, unsure if objects will ever be used over models
        // Having objects is nice for test scenes, but this could be a limitation

        // 7. Skybox:
        // TO - DO

        // 8. Directional Light Source:
        DirectionalLight directionalLightSource;
        // 8a. Set the lighting properties
        directionalLightSource.SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
        directionalLightSource.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        directionalLightSource.SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
        directionalLightSource.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        // 8b. Add the directional light source to the scene
        engine.scene.SetDirectionalLight(directionalLightSource);

        // 9. Point Light Source(s):
        Light pointLightSource(cubeVertices, 288);
        // 9a. Set the lighting properties
        pointLightSource.SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
        pointLightSource.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        pointLightSource.SetDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        pointLightSource.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLightSource.SetPreset(200);
        // 9b. Add the point light source to the scene
        engine.scene.AddLight(pointLightSource);

        // 10. Add a floor to the scene
        // 10a. Create a texture for the floor object
        Texture floorDiffuseTexture;
        Texture floorSpecularTexture;
        floorDiffuseTexture.LoadSRGB("../resources/textures/wood.jpg");
        floorSpecularTexture.LoadRGB("../resources/textures/wood.jpg");
        // 10b. Create a material for the floor object
        Material floorMaterial;
        floorMaterial.SetAmbient(glm::vec3(1.0f, 0.5f, 0.31f));
        floorMaterial.SetDiffuse(glm::vec3(1.0f, 0.5f, 0.31f));
        floorMaterial.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        floorMaterial.SetShininess(32.0f);
        floorMaterial.SetDiffuseTexture(floorDiffuseTexture);
        floorMaterial.SetSpecularTexture(floorSpecularTexture);
        // 10c. Create the floor object
        Object floorObject(floorMaterial, cubeVertices, 288, false);
        // 10d. Resize and move the floor
        glm::mat4 floorModel = floorObject.GetModel();
        floorModel = glm::scale(floorModel, glm::vec3(100.0f, 0.1f, 100.0f));
        floorModel = glm::translate(floorModel, glm::vec3(0.0f, -0.05f, 0.0f));
        floorObject.SetModel(floorModel);
        // 10e. Set the texture scaling
        floorObject.SetTexScaling(10.0f);
        // 10f. Add the floor object to the scene
        engine.scene.AddObject(floorObject);

        // Optional. Move the camera to near the point light source
        engine.camera.SetCameraPos(glm::vec3(10.0f, 9.0f, 0.0f));

        // Begin Engine
        engine.Execute();
    }
	return 0;
}