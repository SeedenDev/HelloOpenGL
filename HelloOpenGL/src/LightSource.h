#pragma once

#include <type_traits>

#include "ImGuiDebugInterface.h"
#include "Cube.h"
#include "Shader.h"

enum LightType {
	GLOBAL, DIRECTIONAL, POINT, SPOTLIGHT, FLASHLIGHT
};

class LightSource : ImGuiDebugInterface
{
private:
	bool m_Toggle = 1;

	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;

protected:
	Cube m_DebugCube = Cube(glm::vec3(0.0f));

public:
	LightSource(const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	const bool IsToggled() const { return m_Toggle; }

	inline const glm::vec3& GetAmbientColor() const { return m_AmbientColor; }
	inline const glm::vec3& GetDiffuseColor() const { return m_DiffuseColor; }
	inline const glm::vec3& GetSpecularColor() const { return m_SpecularColor; }

	void DrawDebugCube(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const;

	void ImGuiDebugDraw() override;

	virtual const LightType GetType() = 0;
};