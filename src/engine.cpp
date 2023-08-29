#include "engine.hpp"

// Default Constructor:
Engine::Engine()
{
    // Prepare variables for initialization
    deltaTime = 0.0f;
    lastFrame = 0.0f;
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
        // Associate the camera pointer to the window
        glfwSetWindowUserPointer(window, &camera);
        glEnable(GL_DEPTH_TEST);
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
    // Callback function called when the mouse is moved:
    glfwSetCursorPosCallback(window, MouseCallback);
    // Callback function called when the scroll wheel is moved:
    glfwSetScrollCallback(window, ScrollCallback);
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.SetCameraPos(cameraPos + (cameraSpeed * cameraFront));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.SetCameraPos(cameraPos - (cameraSpeed * cameraFront));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.SetCameraPos(cameraPos - (glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.SetCameraPos(cameraPos + (glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed));
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
    // Test code: display simple color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw our first triangle
    scene.Render(deltaTime, camera);
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

// Callback functions:
void Engine::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Resize the size of the viewport
    glViewport(0, 0, width, height);
}

void Engine::MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    Camera *cam = (Camera *)glfwGetWindowUserPointer(window);
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (cam->GetFirstMouse())
    {
        cam->SetLastX(xPos);
        cam->SetLastY(yPos);
        cam->SetFirstMouse(false);
    }

    if (!cam->locked)
    {
        float xOffset = xPos - cam->GetLastX();
        float yOffset = cam->GetLastY() - yPos; // reversed since y-coordinates go from bottom to top
        cam->SetLastX(xPos);
        cam->SetLastY(yPos);
        float sensitivity = 0.1f; // change this value to your liking
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        cam->SetYaw(cam->GetYaw() + xOffset);
        cam->SetPitch(cam->GetPitch() + yOffset);
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (cam->GetPitch() > 89.0f)
            cam->SetPitch(89.0f);
        if (cam->GetPitch() < -89.0f)
            cam->SetPitch(-89.0f);
        glm::vec3 front;
        front.x = cos(glm::radians(cam->GetYaw())) * cos(glm::radians(cam->GetPitch()));
        front.y = sin(glm::radians(cam->GetPitch()));
        front.z = sin(glm::radians(cam->GetYaw())) * cos(glm::radians(cam->GetPitch()));
        cam->SetCameraFront(glm::normalize(front));
    }
}

void Engine::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    cam->SetFov(cam->GetFov() - (float)yOffset);
    if (cam->GetFov() < 1.0f)
        cam->SetFov(1.0f);
    if (cam->GetFov() > 45.0f)
        cam->SetFov(45.0f);
}