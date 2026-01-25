#pragma once

#include <glm/vec3.hpp>

class Material
{
private:
	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;
	float m_SpecularShininess;

public:

	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float specularShininess);
	~Material();

	inline /*const */glm::vec3& GetAmbientColor() /*const */{ return m_AmbientColor; }
	inline /*const */glm::vec3& GetDiffuseColor() /*const*/ { return m_DiffuseColor; }
	inline /*const*/ glm::vec3& GetSpecularColor() /*const*/ { return m_SpecularColor; }
	inline /*const */float&/*remove the &*/ GetSpecularShininess() /*const */{ return m_SpecularShininess; }

private:
	//methods
};