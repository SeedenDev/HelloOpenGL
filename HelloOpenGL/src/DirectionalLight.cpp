#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
	: LightSource(ambientColor, diffuseColor, specularColor)
{
	m_Direction = direction;
	m_DebugCube.SetPosition(direction * glm::vec3(10.0f));
	m_DebugCube.SetScale(glm::vec3(0.5f));
}

void DirectionalLight::OnDirectionUpdate(const glm::vec3& direction)
{
	m_DebugCube.SetPosition(direction * glm::vec3(10.0f));
}

void DirectionalLight::ImGuiDebugDraw()
{
	LightSource::ImGuiDebugDraw();
	if (ImGui::SliderFloat3("Direction", &m_Direction[0], -1.0f, 1.0f)) OnDirectionUpdate(m_Direction);
}