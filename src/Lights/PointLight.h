#pragma once

#include "GlobalLight.h"

class PointLight : public GlobalLight
{
private:
	// Attenuation parameters
	float m_Constant = 1.0f;
	float m_Linear = 0.225f;
	float m_Quadratic = 0.032f;

public:
	PointLight(const glm::vec3& position, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	void SetConstant(const float constant)
	{
		m_Constant = constant;
	}
	float GetConstant() const { return m_Constant; }

	void SetLinear(const float linear)
	{
		m_Linear = linear;
	}
	float GetLinear() const { return m_Linear; }

	void SetQuadratic(const float quadratic)
	{
		m_Quadratic = quadratic;
	}
	float GetQuadratic() const { return m_Quadratic; }

	void ImGuiDebugDraw() override;

	LightType::LightType GetType() override
	{
		return LightType::POINT;
	}
};