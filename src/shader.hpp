#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Shader
{
    public:
        // Constructor & Destructor
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();
        // Public Functions:
        void Use() const;
        // Utility Uniform Functions:
        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetVec2(const std::string &name, const glm::vec2 &value) const;
        void SetVec2(const std::string &name, float x, float y) const;
        void SetVec3(const std::string &name, const glm::vec3 &value) const;
        void SetVec3(const std::string &name, float x, float y, float z) const;
        void SetVec4(const std::string &name, const glm::vec4 &value) const;
        void SetVec4(const std::string &name, float x, float y, float z, float w) const;
        void SetMat2(const std::string &name, const glm::mat2 &mat) const;
        void SetMat3(const std::string &name, const glm::mat3 &mat) const;
        void SetMat4(const std::string &name, const glm::mat4 &mat) const;
        // Public attributes:
        unsigned int ID;
        unsigned int vertexShader, fragmentShader;
    private:
        // Private Functions:
        void SetVertexPath(const char* vertexPath);
        void SetFragmentPath(const char* fragmentPath);
        bool LoadGLSLCode();
        bool Compile();
        bool Link();
        // Private attributes:
        const char* vertexCodePath;
        const char* fragmentCodePath;
        std::string vertexGLSLCode, fragmentGLSLCode;
        std::ifstream vertexShaderFile, fragmentShaderFile;
        std::stringstream vertexShaderStream, fragmentShaderStream;
        const char* vertexShaderSource;
        const char* fragmentShaderSource;
};