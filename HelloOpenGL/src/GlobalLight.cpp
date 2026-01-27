#include "GlobalLight.h"

GlobalLight::GlobalLight(const glm::vec3& position, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
	: LightSource(ambientColor, diffuseColor, specularColor)
{
	m_Position = position;
	m_DebugCube.SetPosition(position);
	m_DebugCube.SetScale(glm::vec3(0.75f));
}

void GlobalLight::OnPositionUpdate(const glm::vec3& position)
{
	m_DebugCube.SetPosition(position);
}

void GlobalLight::ImGuiDebugDraw()
{
	LightSource::ImGuiDebugDraw();
	if (ImGui::SliderFloat3("Position", &m_Position[0], -15.0f, 15.0f)) OnPositionUpdate(m_Position);
}