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

	void SetAmbientColor(glm::vec3 ambientColor) { m_AmbientColor = ambientColor; }
	void SetDiffuseColor(glm::vec3 diffuseColor) { m_DiffuseColor = diffuseColor; }
	void SetSpecularColor(glm::vec3 specularColor) { m_SpecularColor = specularColor; }
	void SetEmissiveColor(glm::vec3 emissiveColor) { m_EmissiveColor = emissiveColor; }
	void SetSpecularShininess(float specularShininess) { m_SpecularShininess = specularShininess; }
	void SetSpecularStrength(float specularStrength) { m_SpecularStrength = specularStrength; }
	void SetDiffuseTexture(int diffuseTexture) { m_DiffuseTexture = diffuseTexture; }
	void SetSpecularTexture(int specularTexture) { m_SpecularTexture = specularTexture; }
	void SetEmissiveTexture(int emissiveTexture) { m_EmissiveTexture = emissiveTexture; }
	void SetAlpha(float alpha) { m_Alpha = alpha; }
	void SetReflectivity(float reflectivity) { m_Reflectivity = reflectivity; }
	void SetIOR(float ior) { m_IOR = ior; }

	inline const glm::vec3& GetAmbientColor() const { return m_AmbientColor; }
	inline const glm::vec3& GetDiffuseColor() const { return m_DiffuseColor; }
	inline const glm::vec3& GetSpecularColor() const { return m_SpecularColor; }
	inline const glm::vec3& GetEmissiveColor() const { return m_EmissiveColor; }
	inline const float GetSpecularShininess() const { return m_SpecularShininess; }
	inline const float GetSpecularStrength() const { return m_SpecularStrength; }
	inline const unsigned int GetDiffuseTexture() const { return m_DiffuseTexture; }
	inline const unsigned int GetSpecularTexture() const { return m_SpecularTexture; }
	inline const unsigned int GetEmissiveTexture() const { return m_EmissiveTexture; }
	inline const bool HasDiffuseTexture() const { return m_DiffuseTexture >= 0; }
	inline const bool HasSpecularTexture() const { return m_SpecularTexture >= 0; }
	inline const bool HasEmissiveTexture() const { return m_EmissiveTexture >= 0; }
	inline const float GetAlpha() const { return m_Alpha; }
	inline const float GetReflectivity() const { return m_Reflectivity; }
	inline const float GetIOR() const { return m_IOR; }

	void BindTo(Shader& shader) const;

private:
	//methods
};