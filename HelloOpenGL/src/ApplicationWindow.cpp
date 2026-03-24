#include "ApplicationWindow.h"

#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "GLUtil.h"
#include "Input.h"

ApplicationWindow* ApplicationWindow::s_Instance = nullptr;

ApplicationWindow::ApplicationWindow(const std::string& title, int width, int height)
    : m_Width(width), m_Height(height), m_AspectRatio((float) width / (float) height)
{
    s_Instance = this;
    GLFWwindow* window;

    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 16); // MSAA sample count for default window framebuffer

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

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
    glfwSwapInterval(m_Vsync);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callbacks
    glfwSetWindowUserPointer(window, this);

    // Have to do this because GLFW is a C API lib = objects don't exist. Solutions: 1) have a function calling your method 2) this kind of lambda
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->FramebufferSizeCallback(window, width, height);
        }
    );

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double mouseX, double mouseY)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->MousePosCallback(window, mouseX, mouseY);
        }
    );

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->ScrollCallback(window, xoffset, yoffset);
        }
    );

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->MouseButtonCallback(window, button, action, mods);
        }
    );

    glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
        {
            ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
            appWindow->WindowFocusCallback(window, focused);
        }
    );


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    m_Window = window;
}

ApplicationWindow::~ApplicationWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void ApplicationWindow::Update()
{
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) glfwSetWindowShouldClose(m_Window, true);
        else if (m_LastTimePausePressed <= glfwGetTime())
        {
            m_Paused = !m_Paused;
            glfwSetInputMode(m_Window, GLFW_CURSOR, m_Paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            m_LastTimePausePressed = glfwGetTime() + 0.2f;
        }
    }
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

void ApplicationWindow::WindowFocusCallback(GLFWwindow* window, int focused)
{
    m_Focused = focused == GL_TRUE;
}