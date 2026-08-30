#pragma once

#include "Spotlight.h"
#include "Scene/Camera.h"

class Flashlight : public Spotlight
{
private:
	Camera* m_TrackedCamera;

public:
	Flashlight(Camera* trackedCamera, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	inline const glm::vec3& GetPosition()
	{
		m_Position = m_TrackedCamera->GetPosition();
		return m_Position;
	}
	inline const glm::vec3& GetDirection() 
	{ 
		m_Direction = m_TrackedCamera->GetFront();
		return m_Direction;
	}

	//Note: cancel the cube drawing
	void DrawDebugCube(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const override { }

	const LightType::LightType GetType() override
	{
		return LightType::FLASHLIGHT;
	}
};