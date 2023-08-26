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
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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
    // Callback function called when the mouse is moved:
    glfwSetCursorPosCallback(window, MouseCallback);
    // Callback function called when the scroll wheel is moved:
    glfwSetScrollCallback(window, ScrollCallback);
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
    // Move the camera around when pressing the WASD keys
    float cameraSpeed = static_cast<float>(2.5 * shader.GetDeltaTime());
    glm::vec3 cameraPos = shader.GetCameraPos();
    glm::vec3 cameraFront = shader.GetCameraFront();
    glm::vec3 cameraUp = shader.GetCameraUp();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        shader.SetCameraPos(cameraPos + (cameraSpeed * cameraFront));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        shader.SetCameraPos(cameraPos - (cameraSpeed * cameraFront));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        shader.SetCameraPos(cameraPos - (glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        shader.SetCameraPos(cameraPos + (glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed));
}

void Engine::Render()
{
    // Test code: display simple color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL + GLFW Renderer - Alex Parisi", NULL, NULL);
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
    glEnable(GL_DEPTH_TEST);
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

void Engine::MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{

}

void Engine::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}