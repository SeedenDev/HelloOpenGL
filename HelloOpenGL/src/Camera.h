#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
private:

    GLFWwindow* m_WindowPtr;
    float m_AspectRatio;

    float m_BaseCamSpeed = 2.5f;
    float m_Sensitivity = 0.1f, m_Yaw = -90.0f, m_Pitch = 0.0f;
    float m_FOV = 45.0f, m_Near = 0.1f, m_Far = 100.0f;

    glm::vec3 m_CamPos, m_CamFront, m_CamRight, m_CamUp;
    glm::mat4 m_ViewMatrix, m_ProjMatrix;

    bool m_FirstCall = 1;
    double m_LastMouseX, m_LastMouseY;

public:
    static glm::vec3 s_WorldUp;

    Camera(GLFWwindow* window, float aspectRatio);

    ~Camera();

    void Update(double deltaTime);

    inline void SetAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
    }

    inline const glm::vec3& GetPos() const { return m_CamPos; }
    inline const glm::mat4& GetView() const { return m_ViewMatrix; }
    inline const glm::mat4& GetProj() const { return m_ProjMatrix; }

private:

};