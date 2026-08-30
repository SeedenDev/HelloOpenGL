#pragma once

#include "LightSource.h"

class GlobalLight : public LightSource, public Common::HasPosition
{
private:

public:
	GlobalLight(const glm::vec3& position, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	void OnPositionUpdate(const glm::vec3& position) override;

	void ImGuiDebugDraw() override;

	const LightType::LightType GetType() override
	{
		return LightType::GLOBAL;
	}
};