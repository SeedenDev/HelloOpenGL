#include "Spotlight.h"

#include <glm/vec3.hpp>

Spotlight::Spotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
	: m_Direction(direction), m_InnerCutOff(12.5f), m_OuterCutOff(17.5f), LightSource(position, ambientColor, diffuseColor, specularColor)
{
	m_DebugCube.SetScale(glm::vec3(0.1f));
}

void Spotlight::ImGuiDebugDraw()
{
	ImGui::SliderFloat("InnerCutOff", &m_InnerCutOff, 0.0f, 180.0f);
	ImGui::SliderFloat("OuterCutOff", &m_OuterCutOff, 0.0f, 180.0f);
	ImGui::SliderFloat3("Direction", &m_Direction[0], -1.0f, 1.0f);
	LightSource::ImGuiDebugDraw();
}