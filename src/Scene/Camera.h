#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
private:

    float m_AspectRatio;

    float m_HorizontalSpeed = 2.5f, m_VerticalSpeed = 1.5f;
    float m_Sensitivity = .25f, m_Yaw = -90.0f, m_Pitch = 0.0f;
    float m_FOV = 45.0f, m_Near = 0.005f, m_Far = 256.0f; 
    //TODO: with this low near we can now see objects from very close but there is still a problem being: depth buffer with objects a bit far (artefacts)

    glm::vec3 m_CamPos, m_CamFront, m_CamRight, m_CamUp;
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f), m_ProjMatrix = glm::mat4(1.0f);

    bool m_FirstCall = 1;
    double m_LastMouseX, m_LastMouseY;

public:
    static glm::vec3 s_WorldUp;

    Camera(float aspectRatio);

    ~Camera();

    void Update(double deltaTime);

    inline void SetAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
    }

    inline const void SetHorizontalSpeed(const float speed) { m_HorizontalSpeed = speed ; }
    inline const void SetVerticalSpeed(const float speed) { m_VerticalSpeed = speed; }

    inline const float GetHorizontalSpeed() const { return m_HorizontalSpeed; }
    inline const float GetVerticalSpeed() const { return m_VerticalSpeed; }

    inline const glm::vec3& GetPosition() const { return m_CamPos; }
    inline const glm::mat4& GetView() const { return m_ViewMatrix; }
    inline const glm::mat4& GetProj() const { return m_ProjMatrix; }
    inline const glm::vec3& GetFront() const { return m_CamFront; }

    inline const float GetFOV() const { return m_FOV; }
    inline const float GetYaw() const { return m_Yaw; }
    inline const float GetPitch() const { return m_Pitch; }

private:

};