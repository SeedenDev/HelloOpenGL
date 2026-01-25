#include "Material.h"

#include <glm/vec3.hpp>

Material::Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float specularShininess)
	: m_AmbientColor(ambientColor), m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor), m_SpecularShininess(specularShininess)
{

}

Material::~Material()
{

}