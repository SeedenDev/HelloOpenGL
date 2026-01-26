#pragma once

#include <glm/vec3.hpp>

#include "LightSource.h"

class Spotlight : public LightSource
{
private:
	glm::vec3 m_Direction;
	float m_InnerCutOff; // angle in degree of the light cone
	float m_OuterCutOff; // same but upper for some fading between lit/unlit part (smooth edges)

public:
	Spotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);

	void SetDirection(glm::vec3 direction)
	{
		m_Direction = direction;
	}
	inline const glm::vec3& GetDirection() const { return m_Direction; }

	void SetInnerCutOff(float cutOff)
	{
		m_InnerCutOff = cutOff;
	}
	inline const float GetInnerCutOff() const { return m_InnerCutOff; }
	inline const float GetComputedInnerCutOff() const { return glm::cos(glm::radians(m_InnerCutOff)); }

	void SetOuterCutOff(float cutOff)
	{
		m_OuterCutOff = cutOff;
	}
	inline const float GetOuterCutOff() const { return m_OuterCutOff; }
	inline const float GetComputedOuterCutOff() const { return glm::cos(glm::radians(m_OuterCutOff)); }

	void ImGuiDebugDraw() override;
};