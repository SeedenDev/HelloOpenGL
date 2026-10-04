#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
private:
    float m_AspectRatio;

    float m_HorizontalSpeed = 2.5f, m_VerticalSpeed = 1.5f;
    float m_Sensitivity = .25f, m_Yaw = -90.0f, m_Pitch = 0.0f;
    float m_FOV = 70.0f, m_Near = 0.005f, m_Far = 256.0f;
    //TODO: with this low near we can now see objects from very close but there is still a problem being: depth buffer with objects a bit far (artefacts)

    glm::vec3 m_CamPos = glm::vec3(0.0f), m_CamRight = glm::vec3(0.0f);
    glm::vec3 m_CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f), m_ProjMatrix = glm::mat4(1.0f);

    bool m_FirstCall = 1;
    double m_LastMouseX = 0, m_LastMouseY = 0;

public:
    static glm::vec3 s_WorldUp;

    explicit Camera(float aspectRatio);

    ~Camera();

    void Update(double deltaTime);

    void SetAspectRatio(const float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
    }

    void SetHorizontalSpeed(const float speed) { m_HorizontalSpeed = speed ; }
    void SetVerticalSpeed(const float speed) { m_VerticalSpeed = speed; }

    float GetHorizontalSpeed() const { return m_HorizontalSpeed; }
    float GetVerticalSpeed() const { return m_VerticalSpeed; }

    const glm::vec3& GetPosition() const { return m_CamPos; }
    const glm::mat4& GetView() const { return m_ViewMatrix; }
    const glm::mat4& GetProj() const { return m_ProjMatrix; }
    const glm::vec3& GetFront() const { return m_CamFront; }

    float GetFOV() const { return m_FOV; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }

private:

};