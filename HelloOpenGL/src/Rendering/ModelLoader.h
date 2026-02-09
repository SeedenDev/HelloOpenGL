#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <assimp/scene.h>

#include "Texture.h"
#include "Shader.h"
#include "Material.h"

struct MeshVertex
{
	glm::vec3 position, normal;
	glm::vec2 textureUV;
};

class Mesh
{
private:
	std::vector<MeshVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	Material m_Material;

	unsigned int m_Vao, m_Vbo, m_Ibo;

public:
	Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices, Material& material);
	~Mesh();
	Mesh(const Mesh& other);
	Mesh(Mesh&& other) noexcept;

	inline const Material& GetMaterial() const { return m_Material; }

	void Draw(Shader& shader);

private:
	void SetupGLData();
};

class Model
{
private:
	const std::string m_FilePath;
	const bool m_FlipUVOnLoad;
	std::string m_AssetPath;
	std::vector<Mesh> m_Meshes;
	std::vector<Texture> m_Textures;
	std::unordered_map<std::string, unsigned int> m_CachedTextures;

public:
	Model(const char* path, bool flipUVOnLoad=1);
	~Model();

	void Draw(Shader& shader);

private:
	void Load();
	void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform, std::vector<Mesh>& outMeshes);
	int GetMaterialTextureIndex(aiMaterial* mat, aiTextureType type);
	void LoadMaterialProperties(aiMaterial* mat, Material& outMaterial);
	void LoadEmbeddedTextures(const aiScene*);
};