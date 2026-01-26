#include "PointLight.h"

#include <glm/vec3.hpp>

PointLight::PointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
	: m_Constant(1.0f), m_Linear(0.225f), m_Quadratic(0.032f), LightSource(position, ambientColor, diffuseColor, specularColor)
{
	m_DebugCube.SetScale(glm::vec3(0.75f));
}

void PointLight::ImGuiDebugDraw()
{
	float* var[] = { &m_Linear, &m_Quadratic };
	ImGui::SliderFloat2("Attenuation", var[0], 0.0f, 2.0f);
	LightSource::ImGuiDebugDraw();
}