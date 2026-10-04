#pragma once

#include <string>

#include "ImGuiDebugInterface.h"
#include "Scene/Cube.h"
#include "Rendering/Shader.h"

namespace LightType
{
	enum LightType {
		GLOBAL, DIRECTIONAL, POINT, SPOTLIGHT, FLASHLIGHT
	};

	inline const std::string typeNames[] = { "Global", "Directional", "Point", "Spotlight", "Flashlight" };

	inline const std::string& GetTypeString(const LightType type)
	{
		return typeNames[type];
	}
}

class LightSource : ImGuiDebugInterface
{
private:
	bool m_Toggle = 0;

	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;

protected:
	Cube m_DebugCube = Cube(glm::vec3(0.0f));

public:
	LightSource(const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);

	bool IsToggled() const { return m_Toggle; }

	const glm::vec3& GetAmbientColor() const { return m_AmbientColor; }
	const glm::vec3& GetDiffuseColor() const { return m_DiffuseColor; }
	const glm::vec3& GetSpecularColor() const { return m_SpecularColor; }

	virtual void DrawDebugCube(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const;

	void ImGuiDebugDraw() override;

	virtual LightType::LightType GetType() = 0;
};