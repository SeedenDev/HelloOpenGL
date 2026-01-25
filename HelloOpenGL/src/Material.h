#pragma once

#include <glm/vec3.hpp>

class Material
{
private:
	/* Small notes
		For now, ambient-diffuse-specular are unused because diffuseMap & specularMap are instead of them.
		In an engine, material should have the options between plain colors or textures for these 3 parameters.
		But here, I'm learning opengl and not engineering an engine (I try at least)
	*/
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