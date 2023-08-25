#include "engine.h"

// Default Constructor:
App::App()
{

}

// Default Destructor:
App::~App()
{

}

// Public Functions:
bool App::Initialize()
{
    InitializeGLFW();
    if (CreateWindow() && InitializeGLAD())
    {
        // Create a new viewport
        glViewport(0, 0, 800, 600);
        // Map Callback functions 
        MapCallbacks();
        return true;
    }
    return false;
}

void App::MapCallbacks()
{
    // Callback function called when the window is resized:
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
}

void App::Execute()
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

void App::Terminate()
{
    glfwTerminate();
}

void App::ProcessInput(GLFWwindow* window)
{
    // Close the window if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void App::Render()
{
    // Test code: display simple color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Copy the buffer to the screen
    glfwSwapBuffers(window);
}

void App::HandleEvents()
{
    // Check for events, updates the window state, and calls any corresponding callback functions
    glfwPollEvents();
}

// Private Functions:
void App::InitializeGLFW()
{
    // Init GLFW
    glfwInit();
    // Set Version to OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use 'core' profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool App::InitializeGLAD()
{
    // GLAD manages the way that drivers access OpenGL functions and their locations
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

bool App::CreateWindow()
{
    // Create a new window instance
    window = glfwCreateWindow(800, 600, "OpenGL + GLFW Renderer - Alex Parisi", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    // Make this window active
    glfwMakeContextCurrent(window);
    return true;
}

// Misc functions:
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Resize the size of the viewport
    glViewport(0, 0, width, height);
}