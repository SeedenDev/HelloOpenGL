#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Input.h"

glm::vec3 Camera::s_WorldUp(0.0f, 1.0f, 0.0f);

Camera::Camera(GLFWwindow* window, float aspectRatio)
    : m_WindowPtr(window), m_AspectRatio(aspectRatio), m_CamPos(0.0f), m_CamFront(0.0f, 0.0f, -1.0f), m_CamUp(0.0f, 1.0f, 0.0f)
{

    // Tell GLFW to hide the cursor and capture it once focused
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Camera::~Camera(){}

void Camera::Update(double deltaTime)
{
    //TODO: everything is still working without focus on the window!!!!

    // Mouse XY / Cam Yaw;Pitch
    glm::vec2 mousePos = Input::GetMousePos();
    double mouseX = mousePos.x, mouseY = mousePos.y;
    if (m_FirstCall)
    {
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
        m_FirstCall = 0;
    }
    double offsetX = mouseX - m_LastMouseX;
    double offsetY = m_LastMouseY - mouseY;
    m_LastMouseX = mouseX;
    m_LastMouseY = mouseY;
    m_Yaw += offsetX * m_Sensitivity;
    m_Pitch += offsetY * m_Sensitivity;

    if (m_Pitch > 89.0f) m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    glm::vec3 camDir(
        cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
        sin(glm::radians(m_Pitch)),
        sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
    );
    m_CamFront = glm::normalize(camDir);
    m_CamRight = glm::normalize(glm::cross(m_CamFront, s_WorldUp));
    m_CamUp = glm::normalize(glm::cross(m_CamRight, m_CamFront));

    // Camera position
    bool lShiftPressed = glfwGetKey(m_WindowPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    float camSpeed = m_BaseCamSpeed;
    if (lShiftPressed)
    {
        camSpeed *= 2;
    }
    camSpeed *= deltaTime;

    //Issue: Z/Q are anyway W/A, but if I set W/A it is W/A
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_W) == GLFW_PRESS) m_CamPos += camSpeed * m_CamFront;
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_S) == GLFW_PRESS) m_CamPos -= camSpeed * m_CamFront;
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_A) == GLFW_PRESS) m_CamPos -= camSpeed * m_CamRight;
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_D) == GLFW_PRESS) m_CamPos += camSpeed * m_CamRight;
    // maybe need a up/down key

    m_ViewMatrix = glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);

    // Camera settings

    //Issue: Too fast????? (especially fov & far)
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        if (lShiftPressed) m_Near += 0.1f;
        else m_Far += 0.1f;
    }
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        if (lShiftPressed) m_Near -= 0.1f;
        else m_Far -= 0.1f;
    }
    if (m_Near < 0.1f) m_Near = 0.1f;
    if (m_Near > 10.0f) m_Near = 10.0f;
    if (m_Far < 10.0f) m_Far = 10.0f;
    if (m_Far > 1000.0f) m_Far = 1000.0f;

    if (glfwGetKey(m_WindowPtr, GLFW_KEY_HOME) == GLFW_PRESS) m_FOV -= 0.5f;
    if (glfwGetKey(m_WindowPtr, GLFW_KEY_END) == GLFW_PRESS) m_FOV += 0.5f;
    if (m_FOV < 1.0f) m_FOV = 1.0f;
    if (m_FOV > 100.0f) m_FOV = 100.0f;

    m_ProjMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
}