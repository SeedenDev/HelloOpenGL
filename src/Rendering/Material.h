#pragma once

#include <glm/vec3.hpp>

#include "Rendering/Shader.h"

class Material
{
private:
	glm::vec3 m_AmbientColor = glm::vec3(.0f, .0f, .0f);
	glm::vec3 m_DiffuseColor = glm::vec3(.0f, .0f, .0f);
	glm::vec3 m_SpecularColor = glm::vec3(.0f, .0f, .0f);
	glm::vec3 m_EmissiveColor = glm::vec3(.0f, .0f, .0f);
	float m_SpecularShininess = 64.0f; // specular exponent
	float m_SpecularStrength = 1.0f; // scales specular color
	//TODO: reference to the texture stored in an asset system with all the loaded textures so no duplicated if loading a model twice.
	int m_DiffuseTexture = -1; // also used for ambient
	int m_SpecularTexture = -1;
	int m_EmissiveTexture = -1;
	//TODO: shininessMap, normal/bumpMap, etc..
	float m_Alpha = 1.f;
	float m_Reflectivity = 0.f;
	float m_IOR = 1.f;

public:
	Material();
	~Material();

	void SetAmbientColor(const glm::vec3& ambientColor) { m_AmbientColor = ambientColor; }
	void SetDiffuseColor(const glm::vec3& diffuseColor) { m_DiffuseColor = diffuseColor; }
	void SetSpecularColor(const glm::vec3& specularColor) { m_SpecularColor = specularColor; }
	void SetEmissiveColor(const glm::vec3& emissiveColor) { m_EmissiveColor = emissiveColor; }
	void SetSpecularShininess(const float specularShininess) { m_SpecularShininess = specularShininess; }
	void SetSpecularStrength(const float specularStrength) { m_SpecularStrength = specularStrength; }
	void SetDiffuseTexture(const int diffuseTexture) { m_DiffuseTexture = diffuseTexture; }
	void SetSpecularTexture(const int specularTexture) { m_SpecularTexture = specularTexture; }
	void SetEmissiveTexture(const int emissiveTexture) { m_EmissiveTexture = emissiveTexture; }
	void SetAlpha(const float alpha) { m_Alpha = alpha; }
	void SetReflectivity(const float reflectivity) { m_Reflectivity = reflectivity; }
	void SetIOR(const float ior) { m_IOR = ior; }

	const glm::vec3& GetAmbientColor() const { return m_AmbientColor; }
	const glm::vec3& GetDiffuseColor() const { return m_DiffuseColor; }
	const glm::vec3& GetSpecularColor() const { return m_SpecularColor; }
	const glm::vec3& GetEmissiveColor() const { return m_EmissiveColor; }
	float GetSpecularShininess() const { return m_SpecularShininess; }
	float GetSpecularStrength() const { return m_SpecularStrength; }
	unsigned int GetDiffuseTexture() const { return m_DiffuseTexture; }
	unsigned int GetSpecularTexture() const { return m_SpecularTexture; }
	unsigned int GetEmissiveTexture() const { return m_EmissiveTexture; }
	bool HasDiffuseTexture() const { return m_DiffuseTexture >= 0; }
	bool HasSpecularTexture() const { return m_SpecularTexture >= 0; }
	bool HasEmissiveTexture() const { return m_EmissiveTexture >= 0; }
	float GetAlpha() const { return m_Alpha; }
	float GetReflectivity() const { return m_Reflectivity; }
	float GetIOR() const { return m_IOR; }

	void BindTo(Shader& shader) const;

private:
	//methods
};