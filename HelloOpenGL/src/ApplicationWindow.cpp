#include "ApplicationWindow.h"

#include <iostream>
#include "GLUtil.h"

ApplicationWindow::ApplicationWindow(const std::string& title, int width, int height)
    : m_Width(width), m_Height(height), m_DefaultRatio((float) width / (float) height), m_LastMouseX(width/2), m_LastMouseY(height/2)
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

    glfwSetWindowUserPointer(window, this);

    // Have to do this because GLFW is a C API lib = objects don't exist. Solutions: 1) have a function calling your method 2) this kind of lambda
    auto frameBufferCallback = [](GLFWwindow* window, int width, int height)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->FramebufferSizeCallback(window, width, height);
        };
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);

    // Tell GLFW to hide the cursor and capture it once focused
    //NOTE: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    auto cursorPosCallback = [](GLFWwindow* window, double mouseX, double mouseY)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->MousePosCallback(window, mouseX, mouseY);
        };
    glfwSetCursorPosCallback(window, cursorPosCallback);

    m_Window = window;
}

ApplicationWindow::~ApplicationWindow()
{
    glfwTerminate();
}

void ApplicationWindow::HandleKeyInput()
{
    //TODO: change with a util isKeyPressed(GLFW_KEY_)
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);
}

// private

void ApplicationWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    m_Width = width;
    m_Height = height;
}

void ApplicationWindow::MousePosCallback(GLFWwindow* window, double mouseX, double mouseY)
{
    double offsetX = mouseX - m_LastMouseX;
    double offsetY = m_LastMouseY - mouseY;
    m_LastMouseX = mouseX;
    m_LastMouseY = mouseY;

    //NOTE: send to Camera#rotate?
}