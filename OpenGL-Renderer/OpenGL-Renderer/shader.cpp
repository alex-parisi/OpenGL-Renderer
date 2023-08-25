#include "shader.h"

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
    wireFrameOn = false;
}

// Default destructor:
Shader::~Shader()
{

}

// Public Functions:
bool Shader::Initialize(const char* vertexPath, const char* fragmentPath)
{
    if (LoadGLSLCode(vertexPath, fragmentPath) && Compile() && Link() && BindVertices())
    {
        return true;
    }
    return false;
}

void Shader::Delete()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Shader::Draw()
{
    glUseProgram(shaderProgram);

    // Update shader info:

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Shader::EnableWireframeMode()
{
    wireFrameOn = true;
}

void Shader::DisableWireframeMode()
{
    wireFrameOn = false;
}

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
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };
    // </TEST>
    // Initialize Vertex Array and Vertex Buffer Objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffers.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Then configure vertex attributes(s).
    // Position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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