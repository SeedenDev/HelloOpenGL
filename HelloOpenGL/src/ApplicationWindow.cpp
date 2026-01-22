#include "ApplicationWindow.h"

#include <iostream>
#include "GLUtil.h"
#include "Input.h"

ApplicationWindow::ApplicationWindow(const std::string& title, int width, int height)
    : m_Width(width), m_Height(height), m_AspectRatio((float) width / (float) height)
{
    GLFWwindow* window;

    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    Input::SetWindowPointer(window);

    glfwSwapInterval(m_Vsync); 

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to init GLEW" << std::endl;
        return;
    }

#ifdef _DEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLUtil::DebugMessageCallback, nullptr); // available only since opengl 4.3 but seems to work in 3.3
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window, this);

    // Have to do this because GLFW is a C API lib = objects don't exist. Solutions: 1) have a function calling your method 2) this kind of lambda
    auto frameBufferCallback = [](GLFWwindow* window, int width, int height)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->FramebufferSizeCallback(window, width, height);
        };
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);

    auto cursorPosCallback = [](GLFWwindow* window, double mouseX, double mouseY)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->MousePosCallback(window, mouseX, mouseY);
        };
    glfwSetCursorPosCallback(window, cursorPosCallback);

    auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->ScrollCallback(window, xoffset, yoffset);
        };
    glfwSetScrollCallback(window, scrollCallback);

    auto mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->MouseButtonCallback(window, button, action, mods);
        };
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    m_Window = window;
}

ApplicationWindow::~ApplicationWindow()
{
    glfwTerminate();
}

void ApplicationWindow::Update()
{
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(m_Window, true);
}

// private
void ApplicationWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Resize(width, height);
}

void ApplicationWindow::MousePosCallback(GLFWwindow* window, double mouseX, double mouseY)
{

}

void ApplicationWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void ApplicationWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}