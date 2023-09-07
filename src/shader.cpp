////////////////
// shader.cpp //
////////////////

#include "shader.hpp"

// Default constructor(s):
Shader::Shader()
{
    vertexShader = NULL;
    fragmentShader = NULL;
    geometryShader = NULL;
    geometryCodePath = nullptr;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    vertexShader = NULL;
    fragmentShader = NULL;
    geometryShader = NULL;
    geometryCodePath = nullptr;
    // Update Code file locations
    SetVertexPath(vertexPath);
    SetFragmentPath(fragmentPath);
    if (geometryPath != nullptr)
        SetGeometryPath(geometryPath);
    // Load the GLSL code and compile
    LoadGLSLCode();
    Compile();
    Link();
}

// Default Destructor:
Shader::~Shader()
{

}

// Public Functions:
void Shader::Use() const
{
    glUseProgram(ID);
}

// Utility Uniform Functions:
void Shader::SetBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::SetInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{ 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}

void Shader::SetVec2(const std::string &name, float x, float y) const
{ 
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{ 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}

void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
{ 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Private Functions:
void Shader::SetVertexPath(const char* vertexPath)
{
    vertexCodePath = vertexPath;
}

void Shader::SetFragmentPath(const char* fragmentPath)
{
    fragmentCodePath = fragmentPath;
}

void Shader::SetGeometryPath(const char* geometryPath)
{
    geometryCodePath = geometryPath;
}

bool Shader::LoadGLSLCode()
{
    // Ensure the file loaders can throw exceptions
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // Load the vertex and fragment shader files
    try
    {
        // Open the files
        vertexShaderFile.open(vertexCodePath);
        fragmentShaderFile.open(fragmentCodePath);
        if (geometryCodePath != nullptr)
            geometryShaderFile.open(geometryCodePath);
        // Read the files into streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        if (geometryCodePath != nullptr)
            geometryShaderStream << geometryShaderFile.rdbuf();
        // Close the file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        if (geometryCodePath != nullptr)
            geometryShaderFile.close();
        // Convert stream into string
        vertexGLSLCode = vertexShaderStream.str();
        fragmentGLSLCode = fragmentShaderStream.str();
        if (geometryCodePath != nullptr)
            geometryGLSLCode = geometryShaderStream.str();
        // Convert string into char array
        vertexShaderSource = vertexGLSLCode.c_str();
        fragmentShaderSource = fragmentGLSLCode.c_str();
        if (geometryCodePath != nullptr)
            geometryShaderSource = geometryGLSLCode.c_str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR | SHADER: Could not read GLSL file.\n" << std::endl;
        return false;
    }
    return true;
}

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
    // Geometry Shader
    if (geometryCodePath != nullptr)
    {
        geometryShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        // Check for compile errors
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::cout << "ERROR | SHADER | GEOMETRY: Could not compile GLSL code.\n" << infoLog << std::endl;
            return false;
        }
    }
    return true;
}

bool Shader::Link()
{
    // Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    if (geometryCodePath != nullptr)
        glAttachShader(ID, geometryShader);
    glLinkProgram(ID);
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR | SHADER | PROGRAM | Linking failed.\n" << infoLog << std::endl;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryCodePath != nullptr)
        glDeleteShader(geometryShader);
    return true;
}