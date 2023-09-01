#include "engine.hpp"

// Default Constructor:
Engine::Engine()
{
    // Prepare variables for initialization
    deltaTime = 0.0f;
    lastFrame = 0.0f;
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
    if (CreateWindow() && InitializeGLAD())
    {
        // Create the callback object, linking the camera and input manager
        callbackObj.camera = &camera;
        callbackObj.inputManager = &inputManager;
        // Associate the callback object pointer to the window
        glfwSetWindowUserPointer(window, &callbackObj);
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        // Create a new viewport
        glViewport(0, 0, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT));
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
    // Callback function called when a key is pressed, released, or repeated:
    glfwSetKeyCallback(window, KeyCallback);
}

void Engine::Execute()
{
    // If initialization passes, enter the rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Process inputs, render, and then handle global GLFW events
        ProcessInput(window);
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

void Engine::ProcessInput(GLFWwindow* window)
{
    // Close the window if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // Move the camera around when pressing the WASD keys
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    glm::vec3 cameraPos = camera.GetCameraPos();
    glm::vec3 cameraFront = camera.GetCameraFront();
    glm::vec3 cameraUp = camera.GetCameraUp();
    glm::vec3 newPos = cameraPos;
    if (inputManager.keyboard.GetKeyState(GLFW_KEY_W))
        newPos += cameraSpeed * cameraFront;
    if (inputManager.keyboard.GetKeyState(GLFW_KEY_S))
        newPos -= cameraSpeed * cameraFront;
    if (inputManager.keyboard.GetKeyState(GLFW_KEY_A))
        newPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (inputManager.keyboard.GetKeyState(GLFW_KEY_D))
        newPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (inputManager.keyboard.GetKeyState(GLFW_KEY_Q))
        newPos += cameraSpeed * cameraUp;
    if (inputManager.keyboard.GetKeyState(GLFW_KEY_E))
        newPos -= cameraSpeed * cameraUp;
    camera.SetCameraPos(newPos);
    // Unlock/Lock the camera when the Mouse Left is pressed/released
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        camera.locked = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        camera.locked = true;
        camera.SetFirstMouse(true);
    }
}

void Engine::Render()
{
    // Render the scene
    scene.Render(deltaTime, &camera, &inputManager);
    // Copy the buffer to the screen
    glfwSwapBuffers(window);
}

void Engine::HandleEvents()
{
    // Check for events, updates the window state, and calls any corresponding callback functions
    glfwPollEvents();
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
    window = glfwCreateWindow(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), "OpenGL + GLFW Renderer - Alex Parisi", NULL, NULL);
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
