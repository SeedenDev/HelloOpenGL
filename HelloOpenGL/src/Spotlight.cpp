#include "Spotlight.h"

Spotlight::Spotlight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
	: m_InnerCutOff(12.5f), m_OuterCutOff(17.5f), LightSource(ambientColor, diffuseColor, specularColor)
{
	m_Position = position;
	m_Direction = direction;
	m_DebugCube.SetPosition(position);
	m_DebugCube.SetScale(glm::vec3(0.25f));
}

void Spotlight::OnPositionUpdate(const glm::vec3& position)
{
	m_DebugCube.SetPosition(position);
}

void Spotlight::ImGuiDebugDraw()
{
	LightSource::ImGuiDebugDraw();
	if (ImGui::SliderFloat3("Position", &m_Position[0], -15.0f, 15.0f)) OnPositionUpdate(m_Position);
	ImGui::SliderFloat3("Direction", &m_Direction[0], -1.0f, 1.0f);
	ImGui::SliderFloat("InnerCutOff", &m_InnerCutOff, 0.0f, 180.0f);
	ImGui::SliderFloat("OuterCutOff", &m_OuterCutOff, 0.0f, 180.0f);
}