#include "src/engine.hpp"
#include "src/test_objects.hpp"

int main(int argc, char* argv[])
{
	Engine engine;
    if (engine.Initialize())
    {
        ////////////////////////////////
        // <Start Rotating Cube Test> //
        ////////////////////////////////

        //// First, create a shader for the object (this is actually for lighting)
        //Shader testShader("../src/shaders/colors.vs", "../src/shaders/colors.fs");
        //testShader.SetInt("material.diffuse", 0);
        //testShader.SetInt("material.specular", 1);

        //// Also create a shader for the light (this is actually a small shader used to draw the light cube, not the lighting)
        //Shader lightShader("../src/shaders/light_cube.vs", "../src/shaders/light_cube.fs");
        //
        //// Then, create a texture for the object
        //Texture testTexture;
        //testTexture.LoadRGB("../resources/textures/wood.jpg");

        //// Next, create a material for the object
        //Material testMaterial;
        //testMaterial.SetShader(testShader);
        //testMaterial.SetAmbient(glm::vec3(1.0f, 0.5f, 0.31f));
        //testMaterial.SetDiffuse(glm::vec3(1.0f, 0.5f, 0.31f));
        //testMaterial.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        //testMaterial.SetShininess(32.0f);
        //testMaterial.SetDiffuseTexture(testTexture);
        //testMaterial.SetSpecularTexture(testTexture);

        //// Finally, create the object
        //Object testObject(testMaterial, cubeVertices, 288, false);

        //// And create a point light
        //Light testLight1(lightShader, cubeVertices, 288);
        //testLight1.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
        //testLight1.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        //testLight1.SetDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        //testLight1.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        //testLight1.SetConstant(1.0f);
        //testLight1.SetLinear(0.09f);
        //testLight1.SetQuadratic(0.032f);

        //// And create a directional light
        //DirectionalLight testLight2;
        //testLight2.SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
        //testLight2.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        //testLight2.SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
        //testLight2.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));

        //// Finally, add the object and lights to the scene
        //engine.scene.AddObject(testObject);
        //engine.scene.AddLight(testLight1);
        //engine.scene.SetDirectionalLight(testLight2);

        //////////////////////////////
        // <End Rotating Cube Test> //
        //////////////////////////////

        ////////////////////////////////
        // <Start Model Loading Test> //
        ////////////////////////////////

        stbi_set_flip_vertically_on_load(true);

        // 1. Load the shader:
        Shader modelShader("../src/shaders/model_loading.vs", "../src/shaders/model_loading.fs");

        // 2. Load the model and move it up a teensy bit
        Model model(modelShader, "../resources/models/backpack.obj");
        model.SetModel(glm::translate(model.GetModel(), glm::vec3(0.0f, 5.0f, 0.0f)));

        // 3. Add the model to the scene
        engine.scene.AddModel(model);

        // 4. Create a directional light source
        DirectionalLight directionalLightSource;
        directionalLightSource.SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
        directionalLightSource.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        directionalLightSource.SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
        directionalLightSource.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));

        // 5. Add the directional light source to the scene
        engine.scene.SetDirectionalLight(directionalLightSource);

        // 6. Create a shader for the light object
        Shader lightShader("../src/shaders/light_cube.vs", "../src/shaders/light_cube.fs");

        // 7. Create a point light source
        Light pointLightSource(lightShader, cubeVertices, 288);
        pointLightSource.SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
        pointLightSource.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        pointLightSource.SetDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        pointLightSource.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLightSource.SetConstant(1.0f);
        pointLightSource.SetLinear(0.09f);
        pointLightSource.SetQuadratic(0.032f);

        // 8. Add the point light source to the scene
        engine.scene.AddLight(pointLightSource);

        // 9. Add a floor to the scene
        // 9a. Create a texture for the floor object
        Texture floorTexture;
        floorTexture.LoadRGB("../resources/textures/wood.jpg");
        // 9b. Create a material for the floor object
        Material floorMaterial;
        floorMaterial.SetShader(modelShader);
        floorMaterial.SetAmbient(glm::vec3(1.0f, 0.5f, 0.31f));
        floorMaterial.SetDiffuse(glm::vec3(1.0f, 0.5f, 0.31f));
        floorMaterial.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        floorMaterial.SetShininess(32.0f);
        floorMaterial.SetDiffuseTexture(floorTexture);
        floorMaterial.SetSpecularTexture(floorTexture);
        // 9c. Create the floor object
        Object floorObject(floorMaterial, cubeVertices, 288, false);
        // 9d. Resize and move the floor
        glm::mat4 floorModel = floorObject.GetModel();
        floorModel = glm::scale(floorModel, glm::vec3(100.0f, 0.1f, 100.0f));
        floorModel = glm::translate(floorModel, glm::vec3(0.0f, -0.05f, 0.0f));
        floorObject.SetModel(floorModel);
        // 9e. Set the texture scaling
        floorObject.SetTexScaling(10.0f);
        // 9f. Add the floor object to the scene
        engine.scene.AddObject(floorObject);

        //////////////////////////////
        // <End Model Loading Test> //
        //////////////////////////////

        engine.Execute();
    }
	return 0;
}