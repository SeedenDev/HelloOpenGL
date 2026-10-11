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
	glm::vec3 position;
	glm::vec2 textureUV;
	glm::vec3 normal;
};

class Mesh
{
private:
	std::vector<MeshVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	Material m_Material;

	unsigned int m_Vao = 0, m_Vbo = 0, m_Ibo = 0;

public:
	Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices, const Material& material);
	~Mesh();

	const Material& GetMaterial() const { return m_Material; }

	void Draw(Shader& shader) const;

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
	explicit Model(const std::string& filePath, bool flipUVOnLoad=1);
	~Model();

	void Draw(Shader& shader);

private:
	void Load();
	void ProcessNode(const aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
	void ProcessMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform, std::vector<Mesh>& outMeshes);
	int GetMaterialTextureIndex(const aiMaterial* mat, aiTextureType type);
	void LoadMaterialProperties(const aiMaterial* mat, Material& outMaterial);
	void LoadEmbeddedTextures(const aiScene*);
};