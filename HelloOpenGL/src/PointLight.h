#pragma once

#include <glm/vec3.hpp>

#include "LightSource.h"

class PointLight : public LightSource
{
private:
	// Attenuation parameters
	float m_Constant;
	float m_Linear;
	float m_Quadratic;

public:
	PointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);

	void SetConstant(float constant)
	{
		m_Constant = constant;
	}
	inline const float GetConstant() const { return m_Constant; }

	void SetLinear(float linear)
	{
		m_Linear = linear;
	}
	inline const float GetLinear() const { return m_Linear; }

	void SetQuadratic(float quadratic)
	{
		m_Quadratic = quadratic;
	}
	inline const float GetQuadratic() const { return m_Quadratic; }

	void ImGuiDebugDraw() override;
};