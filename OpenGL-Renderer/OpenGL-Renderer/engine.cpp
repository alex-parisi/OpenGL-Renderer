#include "engine.h"

// Default Constructor:
Engine::Engine()
{
    // Prepare variables for initialization
    window = NULL;
}

// Default Destructor:
Engine::~Engine()
{
    // All cleanup code should be in App::Terminate(), only use this if absolutely needed
}

// Public Functions:
bool Engine::Initialize()
{
    InitializeGLFW();
    if (CreateWindow() && InitializeGLAD() && CreateShader())
    {
        // Create a new viewport
        glViewport(0, 0, 800, 600);
        // Map Callback functions 
        MapCallbacks();
        return true;
    }
    return false;
}

void Engine::MapCallbacks()
{
    // Callback function called when the window is resized:
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
}

void Engine::Execute()
{
    if (Initialize())
    {
        // If initialization passes, enter the rendering loop
        while (!glfwWindowShouldClose(window))
        {
            ProcessInput(window);
            Render();
            HandleEvents();
        }
    }
    // Exit
    Terminate();
}

void Engine::Terminate()
{
    shader.Delete();
    glfwTerminate();
}

void Engine::ProcessInput(GLFWwindow* window)
{
    // Close the window if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Engine::Render()
{
    // Test code: display simple color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // draw our first triangle
    shader.Draw();
    // Copy the buffer to the screen
    glfwSwapBuffers(window);
}

void Engine::HandleEvents()
{
    // Check for events, updates the window state, and calls any corresponding callback functions
    glfwPollEvents();
}

void Engine::EnableWireFrameMode()
{
    shader.EnableWireframeMode();
}

void Engine::DisableWireFrameMode()
{
    shader.DisableWireframeMode();
}

// Private Functions:
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

bool Engine::InitializeGLAD()
{
    // GLAD manages the way that drivers access OpenGL functions and their locations
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR | ENGINE | GLAD: Failed to initialize.\n" << std::endl;
        return false;
    }
    return true;
}

bool Engine::CreateWindow()
{
    // Create a new window instance
    window = glfwCreateWindow(800, 600, "OpenGL + GLFW Renderer - Alex Parisi", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR | ENGINE | GLFW: Failed to create window.\n" << std::endl;
        glfwTerminate();
        return false;
    }
    // Make this window active
    glfwMakeContextCurrent(window);
    return true;
}

bool Engine::CreateShader()
{
    if (!shader.Initialize("shaders/shader.vs", "shaders/shader.fs"))
    {
        std::cout << "ERROR | ENGINE | SHADER: Failed to initialize.\n" << std::endl;
        return false;
    }
    return true;
}

// Callback functions:
void Engine::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Resize the size of the viewport
    glViewport(0, 0, width, height);
}