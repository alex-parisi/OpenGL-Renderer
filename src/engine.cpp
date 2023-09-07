////////////////
// engine.cpp //
////////////////

#include "engine.hpp"

// Constructor:
Engine::Engine()
{
    m_window = nullptr;   
}

// Destructor::
Engine::~Engine()
{
    // Try to put all necessary destruction code in the Terminate() function
}

// Internal functions:
bool Engine::Initialize()
{
    stbi_set_flip_vertically_on_load(true);
    if (InitializeGLFW() && CreateWindow() && InitializeGLAD())
    {
        // Reset input manager
        inputManager.m_keyboard.Reset();
        inputManager.m_mouse.Reset();
        // Configure global OpenGL state:
        glEnable(GL_DEPTH_TEST);
        // Create a new viewport
        glViewport(0, 0, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT));
        // Map Callback functions 
        MapCallbacks();
        // Associate the input manager with the window:
        glfwSetWindowUserPointer(m_window, &inputManager);
        return true;
    }
    return false;
}

void Engine::Execute()
{
    // If initialization passes, enter the rendering loop
    while (!glfwWindowShouldClose(m_window))
    {
        // Process inputs, render, and then handle global GLFW events
        ProcessInput(m_window);
        Render();
        HandleEvents();
    }
    // Exit
    Terminate();
}

void Engine::Terminate()
{
    glfwTerminate();
}

void Engine::SetLightingShader(Shader& lightingShader)
{
    m_scene.SetLightingShader(lightingShader);
}

void Engine::SetShadowShader(Shader& shadowShader)
{
    m_scene.SetShadowShader(shadowShader);
}

void Engine::AddObjectToScene(Object& object)
{
    m_scene.AddObject(object);
}

// Internal functions:
bool Engine::InitializeGLFW()
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
    return true;
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
    m_window = glfwCreateWindow(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), "OpenGL + GLFW Renderer - Alex Parisi", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "ERROR | ENGINE | GLFW: Failed to create window.\n" << std::endl;
        glfwTerminate();
        return false;
    }
    // Make this window active
    glfwMakeContextCurrent(m_window);
    return true;
}

void Engine::MapCallbacks()
{
    // Callback function called when the window is resized:
    glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallback);
}

void Engine::ProcessInput(GLFWwindow* window)
{
    // Close the window if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Engine::Render()
{
    // Clear the scene
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render the scene
    m_scene.Render();
    // Copy the buffer to the screen
    glfwSwapBuffers(m_window);
}

void Engine::HandleEvents()
{
    // Check for events, updates the window state, and calls any corresponding callback functions
    glfwPollEvents();
}

// Callback functions:
void Engine::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Resize the size of the viewport
    glViewport(0, 0, width, height);
}

void Engine::MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    CallbackObj* obj = (CallbackObj*)glfwGetWindowUserPointer(window);
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (obj->camera->GetFirstMouse())
    {
        obj->camera->SetLastX(xPos);
        obj->camera->SetLastY(yPos);
        obj->camera->SetFirstMouse(false);
    }

    if (!obj->camera->locked)
    {
        float xOffset = xPos - obj->camera->GetLastX();
        float yOffset = obj->camera->GetLastY() - yPos; // reversed since y-coordinates go from bottom to top
        obj->camera->SetLastX(xPos);
        obj->camera->SetLastY(yPos);
        float sensitivity = 0.1f; // change this value to your liking
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        obj->camera->SetYaw(obj->camera->GetYaw() + xOffset);
        obj->camera->SetPitch(obj->camera->GetPitch() + yOffset);
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (obj->camera->GetPitch() > 89.0f)
            obj->camera->SetPitch(89.0f);
        if (obj->camera->GetPitch() < -89.0f)
            obj->camera->SetPitch(-89.0f);
        glm::vec3 front;
        front.x = cos(glm::radians(obj->camera->GetYaw())) * cos(glm::radians(obj->camera->GetPitch()));
        front.y = sin(glm::radians(obj->camera->GetPitch()));
        front.z = sin(glm::radians(obj->camera->GetYaw())) * cos(glm::radians(obj->camera->GetPitch()));
        obj->camera->SetCameraFront(glm::normalize(front));
    }
}

void Engine::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    CallbackObj* obj = (CallbackObj*)glfwGetWindowUserPointer(window);
    obj->camera->SetFov(obj->camera->GetFov() - (float)yOffset);
    if (obj->camera->GetFov() < 1.0f)
        obj->camera->SetFov(1.0f);
    if (obj->camera->GetFov() > 45.0f)
        obj->camera->SetFov(45.0f);
}

void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    CallbackObj* obj = (CallbackObj*)glfwGetWindowUserPointer(window);
    // Set the keyboard and mouse managers based on the key input
    if (obj->inputManager->keyboard.CheckKey(key))
        obj->inputManager->keyboard.SetKeyState(key, action);
    if (obj->inputManager->mouse.CheckKey(key))
        obj->inputManager->mouse.SetKeyState(key, action);
}
