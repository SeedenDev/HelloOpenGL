#include "Material.h"

#include <glm/vec3.hpp>
#include <string>

Material::Material()
{
    std::cout << "material constructor" << std::endl;
}

Material::~Material()
{
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