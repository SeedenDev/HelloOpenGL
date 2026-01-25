#pragma once

#include "Cube.h"

class LightSource : public Cube
{
private:
	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;

public:
	// For now header only
	LightSource(glm::vec3 pos, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
		: m_AmbientColor(ambientColor), m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor), Cube(pos, diffuseColor)
	{

	}

	inline /*const*/ glm::vec3& GetAmbientColor() /*const */{ return m_AmbientColor; }
	inline /*const */glm::vec3& GetDiffuseColor() /*const */{ return m_DiffuseColor; }
	inline /*const */glm::vec3& GetSpecularColor() /*const */{ return m_SpecularColor; }
};