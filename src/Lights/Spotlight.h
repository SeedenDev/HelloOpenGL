#pragma once

#include <glm/trigonometric.hpp>
#include "LightSource.h"

class Spotlight : public LightSource, public Common::HasPosition, public Common::HasDirection
{
private:
	float m_InnerCutOff = 12.5f; // angle in degree of the light cone
	float m_OuterCutOff = 17.5f; // same but upper for some fading between lit/unlit part (smooth edges)

public:
	Spotlight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	void SetInnerCutOff(const float cutOff)
	{
		m_InnerCutOff = cutOff;
	}
	float GetInnerCutOff() const { return m_InnerCutOff; }
	float GetComputedInnerCutOff() const { return glm::cos(glm::radians(m_InnerCutOff)); }

	void SetOuterCutOff(const float cutOff)
	{
		m_OuterCutOff = cutOff;
	}
	float GetOuterCutOff() const { return m_OuterCutOff; }
	float GetComputedOuterCutOff() const { return glm::cos(glm::radians(m_OuterCutOff)); }


	void ImGuiDebugDraw() override;

	LightType::LightType GetType() override
	{
		return LightType::SPOTLIGHT;
	}

protected:
	void OnPositionUpdate(const glm::vec3& position) override;
};