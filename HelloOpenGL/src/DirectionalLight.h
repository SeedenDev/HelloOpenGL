#pragma once

#include "LightSource.h"

class DirectionalLight : public LightSource, public Common::HasDirection
{
private:

public:
	DirectionalLight(const glm::vec3& direction, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	void OnDirectionUpdate(const glm::vec3& direction) override;

	void ImGuiDebugDraw() override;

	const LightType GetType() override
	{
		return LightType::DIRECTIONAL;
	}
};