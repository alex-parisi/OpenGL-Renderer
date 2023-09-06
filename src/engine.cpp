#include "engine.hpp"

/// 
/// Engine
/// 

// Constructor:
Engine::Engine()
{
    isSetup = false;
    window = nullptr;
    stbi_set_flip_vertically_on_load(true);
    InitializeGLFW();
    InitializeGLAD();
    if ((engineError.GetErrorCode("GLAD") == Error::NONE) && (engineError.GetErrorCode("WINDOW") == Error::NONE))
    {
        // Configure global OpenGL state:
        glEnable(GL_DEPTH_TEST);
        // Create a new viewport
        glViewport(0, 0, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT));
        // Map Callback functions 
        MapCallbacks();
        isSetup = true;
    }
        
}

// Destructor::
Engine::~Engine()
{
    // Try to put all necessary destruction code in the Terminate() function
}

// Internal functions:
void Engine::Execute()
{

}

void Engine::Terminate()
{
    glfwTerminate();
}

bool Engine::GetIsSetup()
{
    return isSetup;
}

void Engine::InitializeGLFW()
{
    // Init GLFW
    glfwInit();
    // Set Version to OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use 'core' profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Define need for Apple devices
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

void Engine::InitializeGLAD()
{
    // GLAD manages the way that drivers access OpenGL functions and their locations
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR | ENGINE | GLAD: Failed to initialize.\n" << std::endl;
        engineError.SetErrorCode("GLAD", Error::ERROR);
    }
    engineError.SetErrorCode("GLAD", Error::NONE);
}

void Engine::CreateWindow()
{
    // Create a new window instance
    window = glfwCreateWindow(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), "OpenGL + GLFW Renderer - Alex Parisi", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR | ENGINE | GLFW: Failed to create window.\n" << std::endl;
        glfwTerminate();
        engineError.SetErrorCode("WINDOW", Error::ERROR);
    }
    // Make this window active
    glfwMakeContextCurrent(window);
    engineError.SetErrorCode("WINDOW", Error::ERROR);
}

void Engine::MapCallbacks()
{

}

// Callback functions:
void Engine::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Resize the size of the viewport
    glViewport(0, 0, width, height);
}

/// 
/// Engine Error
///
 
// Constructor:
EngineError::EngineError()
{
    GLAD = NONE;
    WINDOW_CREATION = NONE;
}

// Destructor:
EngineError::~EngineError()
{

}

// External functions:
void EngineError::SetErrorCode(std::string type, Error newCode)
{
    if (type == "GLAD")
        GLAD = newCode;
    if (type == "WINDOW")
        WINDOW_CREATION = newCode;
}

Error EngineError::GetErrorCode(std::string type)
{
    if (type == "GLAD")
        return GLAD;
    if (type == "WINDOW")
        return WINDOW_CREATION;
}

