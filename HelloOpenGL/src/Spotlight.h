#pragma once

#include "LightSource.h"

class Spotlight : public LightSource, public Common::HasPosition, public Common::HasDirection
{
private:
	float m_InnerCutOff; // angle in degree of the light cone
	float m_OuterCutOff; // same but upper for some fading between lit/unlit part (smooth edges)

public:
	Spotlight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

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

	void OnPositionUpdate(const glm::vec3& position) override;

	void ImGuiDebugDraw() override;
};