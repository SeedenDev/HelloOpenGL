#include "Material.h"

#include <glm/vec3.hpp>

Material::Material()
{
    std::cout << "material constructor" << std::endl;
}

Material::~Material()
{
    std::cout << "material destructor" << std::endl;
}

Material::Material(const Material& other)
{
	std::cout << "material copied" << std::endl;
}

Material::Material(Material&& other) noexcept
{
    m_AmbientColor = other.m_AmbientColor;
    m_DiffuseColor = other.m_DiffuseColor;
    m_SpecularColor = other.m_SpecularColor;
    m_EmissiveColor = other.m_EmissiveColor;
    m_SpecularShininess = other.m_SpecularShininess;
    m_SpecularStrength = other.m_SpecularStrength;
    m_DiffuseTexture = other.m_DiffuseTexture;
    m_SpecularTexture = other.m_SpecularTexture;
    m_EmissiveTexture = other.m_EmissiveTexture;
	std::cout << "material moved" << std::endl;
}

Material& Material::operator=(const Material& other) noexcept
{
	std::cout << "material moved with op=" << std::endl;
	if (this != &other)
	{
		std::cout << "effectively moved" << std::endl;
	}
	return *this;
}

void Material::BindTo(Shader& shader) const
{
    shader.SetUniformVec3f("u_Material.ambientColor", m_AmbientColor);
    shader.SetUniformVec3f("u_Material.diffuseColor", m_DiffuseColor);
    shader.SetUniformVec3f("u_Material.specularColor", m_SpecularColor);
    shader.SetUniformVec3f("u_Material.emissiveColor", m_EmissiveColor);
    shader.SetUniform1f("u_Material.shininess", m_SpecularShininess);
    shader.SetUniform1f("u_Material.specularStrength", m_SpecularStrength);

    shader.SetUniform1i("u_Material.hasDiffuse", HasDiffuseTexture());
    shader.SetUniform1i("u_Material.hasSpecular", HasSpecularTexture());
    shader.SetUniform1i("u_Material.hasEmissive", HasEmissiveTexture());
    // Warning: no uniform set for texture samplers here (bc we don't know the binding index)
}