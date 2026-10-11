#include "ApplicationWindow.h"

#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "GLUtil.h"
#include "Input.h"

ApplicationWindow* ApplicationWindow::s_Instance = nullptr;

ApplicationWindow::ApplicationWindow(const std::string& title, const int width, const int height)
    : m_Width(width), m_Height(height), m_AspectRatio((float) width / (float) height)
{
    s_Instance = this;

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // mandatory on macos, good for windows/linux to be sure i don't call deprecated gl stuff
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 0); // MSAA sample count for default window framebuffer

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(m_Width, m_Height, title.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
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

    glViewport(0, 0, m_Width, m_Height);
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

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        ApplicationWindow* appWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
        appWindow->KeyCallback(window, key, scancode, action, mods);
    });

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
    if (Input::isKeyDown(GLFW_KEY_ESCAPE))
    {
        if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) glfwSetWindowShouldClose(m_Window, true);
        else if (m_LastTimePausePressed <= glfwGetTime())
        {
            m_Paused = !m_Paused;
            glfwSetInputMode(m_Window, GLFW_CURSOR, m_Paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            m_LastTimePausePressed = glfwGetTime() + 0.2f;
        }
    }
}

// private
void ApplicationWindow::FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
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

void ApplicationWindow::WindowFocusCallback(GLFWwindow* window, const int focused)
{
    m_Focused = focused == GL_TRUE;
}

void ApplicationWindow::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

}