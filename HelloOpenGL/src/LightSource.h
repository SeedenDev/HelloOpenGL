#pragma once

#include "ImGuiDebugInterface.h"
#include "Cube.h"
#include "Shader.h"

class LightSource : ImGuiDebugInterface
{
private:
	glm::vec3 m_Position; //TODO: remove from it and create a class Positionable/Directable

	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;

protected:
	Cube m_DebugCube;

public:
	LightSource(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);

	virtual void SetPosition(glm::vec3 position)
	{
		m_Position = position;
		m_DebugCube.SetPosition(position);
	}
	inline const glm::vec3& GetPosition() const { return m_Position; }

	inline const glm::vec3& GetAmbientColor() const { return m_AmbientColor; }
	inline const glm::vec3& GetDiffuseColor() const { return m_DiffuseColor; }
	inline const glm::vec3& GetSpecularColor() const { return m_SpecularColor; }

	void DrawDebugCube(Shader& shader, const glm::mat4 view, const glm::mat4 projection) const;

	void ImGuiDebugDraw() override;
};