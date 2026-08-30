#include "PointLight.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
	: m_Constant(1.0f), m_Linear(0.225f), m_Quadratic(0.032f), GlobalLight(position, ambientColor, diffuseColor, specularColor)
{
	m_DebugCube.SetScale(glm::vec3(0.1f));
}

void PointLight::ImGuiDebugDraw()
{
	GlobalLight::ImGuiDebugDraw();
	float* var[] = { &m_Linear, &m_Quadratic };
	ImGui::SliderFloat2("Attenuation", var[0], 0.0f, 2.0f);
}