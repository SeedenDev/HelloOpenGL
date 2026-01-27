#pragma once

#include "Spotlight.h"
#include "Camera.h"

class Flashlight : public Spotlight
{
private:
	Camera& m_TrackedCamera;

public:
	Flashlight(Camera& trackedCamera, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	//TODO: THAT DOESNT WORK AT ALL
	inline const glm::vec3& GetPosition() const { return m_TrackedCamera.GetPosition(); }
	inline const glm::vec3& GetDirection() const { return m_TrackedCamera.GetFront(); }
};