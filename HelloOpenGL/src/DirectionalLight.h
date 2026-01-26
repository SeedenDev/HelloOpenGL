#pragma once

#include <glm/vec3.hpp>

#include "LightSource.h"

class DirectionalLight : public LightSource
{
private:
	glm::vec3 m_Direction;

public:
	DirectionalLight(glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);

	//TODO: remove bc only for testing
	void SetPosition(glm::vec3 position) override
	{
		LightSource::SetPosition(position);
		m_Direction = position;
	}

	void SetDirection(glm::vec3 direction)
	{
		m_Direction = direction;
		LightSource::SetPosition(direction*glm::vec3(2.0f)); //TODO: remove bc only for testing
	}
	inline const glm::vec3& GetDirection() const { return m_Direction; }

	void ImGuiDebugDraw() override;
};