#include "Flashlight.h"

Flashlight::Flashlight(Camera* trackedCamera, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
	: m_TrackedCamera(trackedCamera), Spotlight(glm::vec3(0.0f), glm::vec3(0.0f), ambientColor, diffuseColor, specularColor)
{

}