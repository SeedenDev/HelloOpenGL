#include "DirectionalLight.h"

#include <glm/vec3.hpp>

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
	: m_Direction(direction), LightSource(glm::vec3(0.0f), ambientColor, diffuseColor, specularColor)
{
	m_DebugCube.SetScale(glm::vec3(0.25f));
}

void DirectionalLight::ImGuiDebugDraw()
{
	if (ImGui::SliderFloat3("Direction", &m_Direction[0], -1.0f, 1.0f)) SetPosition(m_Direction); //TODO: remove this (only for some tests)
	LightSource::ImGuiDebugDraw();
}