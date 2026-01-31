#include "LightSource.h"

LightSource::LightSource(const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
	: m_AmbientColor(ambientColor), m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor)
{
	
}

void LightSource::DrawDebugCube(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const
{
	glm::mat4 model = m_DebugCube.GetModelMatrix();
	glm::mat4 MVP = projection * view * model;
	shader.SetUniformMat4f("u_View", view);
	shader.SetUniformMat4f("u_Projection", projection);
	shader.SetUniformMat4f("u_Model", model);
	shader.SetUniformMat4f("u_MVP", MVP);
	shader.SetUniformVec3f("u_LightColor", m_DiffuseColor);
	m_DebugCube.Draw();
}

void LightSource::ImGuiDebugDraw()
{
	ImGui::Checkbox("Toggle", &m_Toggle);
	ImGui::ColorEdit3("AmbientColor", &m_AmbientColor[0]);
	ImGui::ColorEdit3("DiffuseColor", &m_DiffuseColor[0]);
	ImGui::ColorEdit3("SpecularColor", &m_SpecularColor[0]);
}