#include "shader.h"
#include "settings.h"

// Default constructor:
Shader::Shader()
{
    // Prepare variables for initialization
    vertexShaderSource = NULL;
    fragmentShaderSource = NULL;
    vertexShader = NULL;
    fragmentShader = NULL;
    shaderProgram = NULL;
    VBO = NULL;
    VAO = NULL;
    EBO = NULL;
    texture1 = NULL;
    texture2 = NULL;
    wireFrameOn = false;
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    firstMouse = true;
    yaw = -90.0f;
    pitch = 0.0f;
    lastX = 800.0f / 2.0;
    lastY = 600.0f / 2.0;
    fov = 45.0f;
}

// Default destructor:
Shader::~Shader()
{

}

// Public Functions:
bool Shader::Initialize(const char* vertexPath, const char* fragmentPath)
{
    if (LoadGLSLCode(vertexPath, fragmentPath) && Compile() && Link() && BindVertices() && LoadTexture())
    {
        glUseProgram(shaderProgram);
        // Set the uniforms manually
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
        // Update shader info:
        // Bind texture:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // Activate shader:
        glUseProgram(shaderProgram);
        return true;
    }
    return false;
}

void Shader::Delete()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Shader::Draw()
{
    // Render loop:
    glm::vec3 cubePositions[] = { glm::vec3(0.0f,  0.0f,  0.0f) };
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // Bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Activate shader
    glUseProgram(shaderProgram);
    // Pass Projection matrix to shader
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    setMat4("projection", projection);
    // Camera/View Transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    setMat4("view", view);
    // Render all boxes in list
    glBindVertexArray(VAO);
    int idx = 0;
    for (glm::vec3 position : cubePositions)
    {
        float t = (float)glfwGetTime();
        // Calculate the model matrix for each object and pass it to the shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        float angle = 20.0f * idx;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        // Do fun rotation and scaling
        model = glm::rotate(model, t, glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.125 * sin(4 * t) + 0.875, 0.125 * sin(4 * t) + 0.875, 0.125 * sin(4 * t) + 0.875));
        setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        idx++;
    }
    
}

void Shader::EnableWireframeMode()
{
    wireFrameOn = true;
}

void Shader::DisableWireframeMode()
{
    wireFrameOn = false;
}

void Shader::SetCameraPos(glm::vec3 newCameraPos)
{
    cameraPos = newCameraPos;
}

void Shader::SetCameraFront(glm::vec3 newCameraFront)
{
    cameraFront = newCameraFront;
}

void Shader::SetCameraUp(glm::vec3 newCameraUp)
{
    cameraUp = newCameraUp;
}

glm::vec3 Shader::GetCameraPos()
{
    return cameraPos;
}

glm::vec3 Shader::GetCameraFront()
{
    return cameraFront;
}

glm::vec3 Shader::GetCameraUp()
{
    return cameraUp;
}

float Shader::GetDeltaTime()
{
    return deltaTime;
}

void Shader::SetFirstMouse(bool newFirstMouse)
{
    firstMouse = newFirstMouse;
}

void Shader::SetYaw(float newYaw)
{
    yaw = newYaw;
}

void Shader::SetPitch(float newPitch)
{
    pitch = newPitch;
}

void Shader::SetLastX(float newLastX)
{
    lastX = newLastX;
}

void Shader::SetLastY(float newLastY)
{
    lastY = newLastY;
}

void Shader::SetFov(float newFov)
{
    fov = newFov;
}

bool Shader::GetFirstMouse()
{
    return firstMouse;
}

float Shader::GetYaw()
{
    return yaw;
}

float Shader::GetPitch()
{
    return pitch;
}

float Shader::GetLastX()
{
    return lastX;
}

float Shader::GetLastY()
{
    return lastY;
}

float Shader::GetFov()
{
    return fov;
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Private functions:
bool Shader::LoadGLSLCode(const char* vertexPath, const char* fragmentPath)
{

    // Ensure the file loaders can throw exceptions
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // Load the vertex and fragment shader files
    try
    {
        // Open the files
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        // Read the files into streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        // Close the file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexGLSLCode = vertexShaderStream.str();
        fragmentGLSLCode = fragmentShaderStream.str();
        // Convert string into char array
        vertexShaderSource = vertexGLSLCode.c_str();
        fragmentShaderSource = fragmentGLSLCode.c_str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR | SHADER: Could not read GLSL file.\n" << std::endl;
        return false;
    }
    return true;
}

// Private Functions:
bool Shader::Compile()
{
    // Vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR | SHADER | VERTEX: Could not compile GLSL code.\n" << infoLog << std::endl;
        return false;
    }
    // Fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR | SHADER | FRAGMENT: Could not compile GLSL code.\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::Link() 
{
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR | SHADER | PROGRAM | Linking failed.\n" << infoLog << std::endl;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

bool Shader::BindVertices()
{
    // <TEST>
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // </TEST>
    // Initialize Vertex Array and Vertex Buffer Objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffers.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind the Element Buffer Object
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Then configure vertex attributes(s).
    // Position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture Coordinates:
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // As we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);
    // Set the wireframe mode to be on or off
    if (wireFrameOn)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    return true;
}

bool Shader::LoadTexture()
{
    stbi_set_flip_vertically_on_load(true);
    // Texture 1:
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("../resources/textures/test2.jpg", &width, &height, &nrChannels, 0);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR | SHADER | TEXTURE: Failed to load texture1.\n" << std::endl;
        return false;
    }
    stbi_image_free(data);
    // Texture 2:
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("../resources/textures/test3.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR | SHADER | TEXTURE: Failed to load texture2.\n" << std::endl;
        return false;
    }
    stbi_image_free(data);
    return true;
}