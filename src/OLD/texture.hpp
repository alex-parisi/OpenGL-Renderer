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

class Texture
{
    public:
        // Constructor & Destructor:
        Texture();
        ~Texture();
        // Public Functions:
        bool LoadRGB(const char* texturePath);
        bool LoadRGB(const char* texturePath, int TEXTURE_WRAPPING, int TEXTURE_FILTERING);
        bool LoadRGBA(const char* texturePath);
        bool LoadRGBA(const char* texturePath, int TEXTURE_MAPPING, int TEXTURE_FILTERING);
        bool LoadSRGB(const char* texturePath);
        bool LoadSRGB(const char* texturePath, int TEXTURE_WRAPPING, int TEXTURE_FILTERING);
        bool LoadSRGBA(const char* texturePath);
        bool LoadSRGBA(const char* texturePath, int TEXTURE_WRAPPING, int TEXTURE_FILTERING);
        // Public Attributes:
        unsigned int texture;
};