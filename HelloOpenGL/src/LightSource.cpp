#include "LightSource.h"

LightSource::LightSource(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
	: m_Position(position), m_AmbientColor(ambientColor), m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor), m_DebugCube(Cube(glm::vec3(0.0f)))
{
	
}

void LightSource::DrawDebugCube(Shader& shader, const glm::mat4 view, const glm::mat4 projection) const
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
	if (ImGui::SliderFloat3("Position", &m_Position[0], -15.0f, 15.0f)) SetPosition(m_Position); //TODO: =update the model matrix.. (have to change the way it works really I hate this Cube class)
	ImGui::ColorEdit3("AmbientColor", &m_AmbientColor[0]);
	ImGui::ColorEdit3("DiffuseColor", &m_DiffuseColor[0]);
	ImGui::ColorEdit3("SpecularColor", &m_SpecularColor[0]);
}
