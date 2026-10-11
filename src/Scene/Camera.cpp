#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "ApplicationWindow.h"
#include "Input.h"

glm::vec3 Camera::s_WorldUp(0.0f, 1.0f, 0.0f);

Camera::Camera(float aspectRatio)
    : m_AspectRatio(aspectRatio)
{

}

Camera::~Camera(){}

// Custom lookAt matrix
glm::mat4 LookAt(const glm::vec3& camPos, const glm::vec3& camTarget, const glm::vec3& camUp)
{
    const glm::vec3 camDir(glm::normalize(camPos - camTarget));
    const glm::vec3 camRight(glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camDir)));
    // glm::vec3 camUpUp(glm::normalize(glm::cross(camDir, camRight)));

    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -camPos);
    // Column major. 4 elements = a column.
    const glm::mat4 rotationMatrix = glm::mat4(camRight.x, camUp.x, camDir.x, 0,    camRight.y, camUp.y, camDir.y, 0,   camRight.z, camUp.z, camDir.z, 0,   0, 0, 0, 1);

    return rotationMatrix * translationMatrix;
}

void Camera::Update(const double deltaTime)
{
    const bool lShiftPressed = Input::isKeyDown(GLFW_KEY_LEFT_SHIFT);
    // Proj matrix
    if (Input::isKeyDown(GLFW_KEY_PAGE_UP))
    {
        if (lShiftPressed) m_Near += 0.1f;
        else m_Far += 1.0f;
    }
    if (Input::isKeyDown(GLFW_KEY_PAGE_DOWN))
    {
        if (lShiftPressed) m_Near -= 0.1f;
        else m_Far -= 1.0f;
    }
    //if (m_Near < 0.1f) m_Near = 0.1f;
    if (m_Near > 10.0f) m_Near = 10.0f;
    if (m_Far < 10.0f) m_Far = 10.0f;
    if (m_Far > 100000.0f) m_Far = 100000.0f;

    if (Input::isKeyDown(GLFW_KEY_HOME)) m_FOV -= 0.1f;
    if (Input::isKeyDown(GLFW_KEY_END)) m_FOV += 0.1f;
    if (m_FOV < 1.0f) m_FOV = 1.0f;
    if (m_FOV > 100.0f) m_FOV = 100.0f;

    m_ProjMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);

    // Mouse XY/Offset
    const auto [mouseX, mouseY] = Input::GetMousePos();
    if (m_FirstCall)
    {
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
        m_FirstCall = 0;
    }
    const double offsetX = mouseX - m_LastMouseX;
    const double offsetY = m_LastMouseY - mouseY;
    m_LastMouseX = mouseX;
    m_LastMouseY = mouseY;

    // Don't move the camera around of not focused/if paused
    if (!ApplicationWindow::Get().IsFocused() || ApplicationWindow::Get().IsPaused()) return;

    // Yaw;Pitch
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
    float camSpeedH = m_HorizontalSpeed;
    float camSpeedV = m_VerticalSpeed;
    if (lShiftPressed)
    {
        camSpeedH *= 2;
        camSpeedV *= 2;
    }
    camSpeedH *= deltaTime;
    camSpeedV *= deltaTime;

    if (Input::isKeyDown(GLFW_KEY_W)) m_CamPos += camSpeedH * m_CamFront;
    if (Input::isKeyDown(GLFW_KEY_S)) m_CamPos -= camSpeedH * m_CamFront;
    if (Input::isKeyDown(GLFW_KEY_A)) m_CamPos -= camSpeedH * m_CamRight;
    if (Input::isKeyDown(GLFW_KEY_D)) m_CamPos += camSpeedH * m_CamRight;
    if (Input::isKeyDown(GLFW_KEY_SPACE)) m_CamPos.y += camSpeedV;
    if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL)) m_CamPos.y -= camSpeedV;

    m_ViewMatrix = glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);
}